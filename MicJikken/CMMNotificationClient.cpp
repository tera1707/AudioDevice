#include "CMMNotificationClient.h"

std::string WStringToString(std::wstring oWString);

#define SAFE_RELEASE(punk)  \
              if ((punk) != NULL)  \
                { (punk)->Release(); (punk) = NULL; }

CMMNotificationClient::CMMNotificationClient(IMMDeviceEnumerator* pEnum) : _cRef(1)
{
    _pEnumerator = pEnum;
}

CMMNotificationClient::~CMMNotificationClient()
{
    //SAFE_RELEASE(_pEnumerator)
}

////////////////////////////////////////////////////////////////
// IUnknown methods -- AddRef, Release, and QueryInterface
////////////////////////////////////////////////////////////////

ULONG STDMETHODCALLTYPE CMMNotificationClient::AddRef()
{
    return InterlockedIncrement(&_cRef);
}

ULONG STDMETHODCALLTYPE CMMNotificationClient::Release()
{
    ULONG ulRef = InterlockedDecrement(&_cRef);
    if (0 == ulRef)
    {
        delete this;
    }
    return ulRef;
}

HRESULT STDMETHODCALLTYPE CMMNotificationClient::QueryInterface(REFIID riid, VOID** ppvInterface)
{
    if (IID_IUnknown == riid)
    {
        AddRef();
        *ppvInterface = (IUnknown*)this;
    }
    else if (__uuidof(IMMNotificationClient) == riid)
    {
        AddRef();
        *ppvInterface = (IMMNotificationClient*)this;
    }
    else
    {
        *ppvInterface = NULL;
        return E_NOINTERFACE;
    }
    return S_OK;
}

////////////////////////////////////////////////////////////////
// Callback methods for device-event notifications.
////////////////////////////////////////////////////////////////

HRESULT STDMETHODCALLTYPE CMMNotificationClient::OnDefaultDeviceChanged(EDataFlow flow, ERole role, LPCWSTR pwstrDeviceId)
{
    if (pwstrDeviceId != NULL)
    {
        auto devName = GetDevName(pwstrDeviceId);
        printf(devName.c_str());
    }

    std::string pszFlow = LPSTR("?????");
    std::string pszRole = LPSTR("?????");

    switch (flow)
    {
    case eRender: pszFlow = "eRender"; break;
    case eCapture: pszFlow = "eCapture"; break;
    }

    switch (role)
    {
    case eConsole: pszRole = "eConsole"; break;
    case eMultimedia: pszRole = "eMultimedia"; break;
    case eCommunications: pszRole = "eCommunications"; break;
    }

    printf("  -->New default device: flow = %s, role = %s\n", pszFlow.c_str(), pszRole.c_str());
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CMMNotificationClient::OnDeviceAdded(LPCWSTR pwstrDeviceId)
{
    auto devName = GetDevName(pwstrDeviceId);
    printf(devName.c_str());
    printf("  -->Added device\n");
    return S_OK;
};

HRESULT STDMETHODCALLTYPE CMMNotificationClient::OnDeviceRemoved(LPCWSTR pwstrDeviceId)
{
    auto devName = GetDevName(pwstrDeviceId);
    printf(devName.c_str());
    printf("  -->Removed device\n");
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CMMNotificationClient::OnDeviceStateChanged(LPCWSTR pwstrDeviceId, DWORD dwNewState)
{
    auto devName = GetDevName(pwstrDeviceId);
    printf(devName.c_str());

    std::string pszState = "?????";

    switch (dwNewState)
    {
    case DEVICE_STATE_ACTIVE: pszState = "ACTIVE"; break;
    case DEVICE_STATE_DISABLED: pszState = "DISABLED"; break;
    case DEVICE_STATE_NOTPRESENT: pszState = "NOTPRESENT"; break;
    case DEVICE_STATE_UNPLUGGED: pszState = "UNPLUGGED"; break;
    }

    printf("  -->New device state is DEVICE_STATE_%s (0x%8.8x)\n", pszState.c_str(), dwNewState);

    return S_OK;
}

HRESULT STDMETHODCALLTYPE CMMNotificationClient::OnPropertyValueChanged(LPCWSTR pwstrDeviceId, const PROPERTYKEY key)
{
    auto devName = GetDevName(pwstrDeviceId);
    printf(devName.c_str());

    printf("  -->Changed device property "
        "{%8.8x-%4.4x-%4.4x-%2.2x%2.2x-%2.2x%2.2x%2.2x%2.2x%2.2x%2.2x}#%d\n",
        key.fmtid.Data1, key.fmtid.Data2, key.fmtid.Data3,
        key.fmtid.Data4[0], key.fmtid.Data4[1],
        key.fmtid.Data4[2], key.fmtid.Data4[3],
        key.fmtid.Data4[4], key.fmtid.Data4[5],
        key.fmtid.Data4[6], key.fmtid.Data4[7],
        key.pid);
    return S_OK;
}

std::string CMMNotificationClient::GetDevName(LPCWSTR pwstrDeviceId)
{
    IMMDevice* device = NULL;
    _pEnumerator->GetDevice(pwstrDeviceId, &device);

    IPropertyStore* pProperties;
    device->OpenPropertyStore(STGM_READ, &pProperties);
    PROPVARIANT vName;
    PropVariantInit(&vName);
    pProperties->GetValue(PKEY_Device_FriendlyName, &vName);
    return WStringToString(std::wstring(vName.pwszVal == NULL ? L"" : vName.pwszVal));
}

// ------------------------------------------------------------------------

std::string WStringToString(std::wstring oWString)
{
    // wstring → SJIS
    int iBufferSize = WideCharToMultiByte(CP_OEMCP, 0, oWString.c_str()
        , -1, (char*)NULL, 0, NULL, NULL);

    // バッファの取得
    CHAR* cpMultiByte = new CHAR[iBufferSize];

    // wstring → SJIS
    WideCharToMultiByte(CP_OEMCP, 0, oWString.c_str(), -1, cpMultiByte
        , iBufferSize, NULL, NULL);

    // stringの生成
    std::string oRet(cpMultiByte, cpMultiByte + iBufferSize - 1);

    // バッファの破棄
    delete[] cpMultiByte;

    // 変換結果を返す
    return(oRet);
}