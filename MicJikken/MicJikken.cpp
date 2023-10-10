#include <windows.h>
#include <mmdeviceapi.h>
#include <functiondiscoverykeys.h>
#include <endpointvolume.h>
#include <string>

/*
参考：
https://learn.microsoft.com/ja-jp/windows/win32/coreaudio/device-events

メモ：
OnDeviceAddedやOnDeviceRemovedは、オーディオデバイスを抜き差し（例えばUSBヘッドセットをUSBの穴に抜き差し）したときに来るのではなく、
デバイスのドライバをWindowsにインストールしたときにくるっぽい。
（その動作(デバイスインストール時にここを通る)を確認は出来てないのだが、USBヘッドセットを抜き差ししても、OnDeviceAddedやOnDeviceRemovedを通らなかった。）


*/


#define SAFE_RELEASE(punk)  \
              if ((punk) != NULL)  \
                { (punk)->Release(); (punk) = NULL; }

class CMMNotificationClient : public IMMNotificationClient
{
    LONG _cRef;
    IMMDeviceEnumerator* _pEnumerator;

public:
    CMMNotificationClient() :
        _cRef(1),
        _pEnumerator(NULL)
    {
    }

    ~CMMNotificationClient()
    {
        SAFE_RELEASE(_pEnumerator)
    }

    // IUnknown methods -- AddRef, Release, and QueryInterface

    ULONG STDMETHODCALLTYPE AddRef()
    {
        return InterlockedIncrement(&_cRef);
    }

    ULONG STDMETHODCALLTYPE Release()
    {
        ULONG ulRef = InterlockedDecrement(&_cRef);
        if (0 == ulRef)
        {
            delete this;
        }
        return ulRef;
    }

    HRESULT STDMETHODCALLTYPE QueryInterface(
        REFIID riid, VOID** ppvInterface)
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

    // Callback methods for device-event notifications.

    HRESULT STDMETHODCALLTYPE OnDefaultDeviceChanged(
        EDataFlow flow, ERole role,
        LPCWSTR pwstrDeviceId)
    {
        char* pszFlow = LPSTR("?????");
        char* pszRole = LPSTR("?????");

        //_PrintDeviceName(pwstrDeviceId);

        switch (flow)
        {
        case eRender:
           // pszFlow = "eRender";
            break;
        case eCapture:
           // pszFlow = "eCapture";
            break;
        }

        switch (role)
        {
        case eConsole:
            //pszRole = "eConsole";
            break;
        case eMultimedia:
           // pszRole = "eMultimedia";
            break;
        case eCommunications:
            //pszRole = "eCommunications";
            break;
        }

        printf("  -->New default device: flow = %s, role = %s\n",
            pszFlow, pszRole);
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE OnDeviceAdded(LPCWSTR pwstrDeviceId)
    {
        //_PrintDeviceName(pwstrDeviceId);

        printf("  -->Added device\n");
        return S_OK;
    };

    HRESULT STDMETHODCALLTYPE OnDeviceRemoved(LPCWSTR pwstrDeviceId)
    {
        //_PrintDeviceName(pwstrDeviceId);

        printf("  -->Removed device\n");
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE OnDeviceStateChanged(
        LPCWSTR pwstrDeviceId,
        DWORD dwNewState)
    {
        //char* pszState = "?????";

        //_PrintDeviceName(pwstrDeviceId);

        switch (dwNewState)
        {
        case DEVICE_STATE_ACTIVE:
           // pszState = "ACTIVE";
            break;
        case DEVICE_STATE_DISABLED:
            //pszState = "DISABLED";
            break;
        case DEVICE_STATE_NOTPRESENT:
            //pszState = "NOTPRESENT";
            break;
        case DEVICE_STATE_UNPLUGGED:
            //pszState = "UNPLUGGED";
            break;
        }

       // printf("  -->New device state is DEVICE_STATE_%s (0x%8.8x)\n",
           // pszState, dwNewState);

        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE OnPropertyValueChanged(
        LPCWSTR pwstrDeviceId,
        const PROPERTYKEY key)
    {
        //_PrintDeviceName(pwstrDeviceId);

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
};

int main()
{
	HRESULT hr;
	IMMDeviceEnumerator* pEnum = NULL;
	IMMDeviceCollection* pCollection = NULL;
	UINT deviceCount = 0;

	// COMの初期化(COMのお作法)
	hr = CoInitializeEx(0, COINIT_MULTITHREADED);

	// COMからMMDeviceEnumeratorを取ってくる
	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, IID_PPV_ARGS(&pEnum));


	auto nc = new CMMNotificationClient();
	pEnum->RegisterEndpointNotificationCallback(nc);

	// オーディオエンドポイントの列挙を実行
	hr = pEnum->EnumAudioEndpoints(EDataFlow::eCapture, DEVICE_STATE_ACTIVE , &pCollection);

	// とれた数を数える
	hr = pCollection->GetCount(&deviceCount);

	for (int i = 0; i < deviceCount; i++)
	{
		// デバイスの情報を取る
		IMMDevice* pEndpoint = NULL;
		pCollection->Item(i, &pEndpoint);

		// デバイスのプロパティを取る
		IPropertyStore* pProperties;
		pEndpoint->OpenPropertyStore(STGM_READ, &pProperties);

		// 取ったプロパティから、値を指定して取得する
		PROPVARIANT vName;
		PropVariantInit(&vName);
		pProperties->GetValue(PKEY_Device_FriendlyName, &vName);

		// エンドポイントの情報を取る
		IAudioEndpointVolume* pAudioEndVol = NULL;
		hr = pEndpoint->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL, (void**)&pAudioEndVol);

		// エンドポイントに指示を出す（「IAudioEndpointVolume」のVolumeは、マイクの怨霊という意味の「ボリューム」とは別の意味っぽい）
		float getVolume = 0.0f;
		pAudioEndVol->GetMasterVolumeLevelScalar(&getVolume);				// 音量を取得
		float setVolume = 0.25f;
		pAudioEndVol->SetMasterVolumeLevelScalar(setVolume, &GUID_NULL);	// 音量を設定

		std::wstring outString = std::to_wstring(i) + L":" + vName.pwszVal + std::to_wstring(getVolume) + L"\r\n";
		OutputDebugString(outString.c_str());

		// 後処理系
		if (pAudioEndVol)
			pAudioEndVol->Release();
		if (pProperties)
			pProperties->Release();
		if (pEndpoint)
			pEndpoint->Release();
	}

    Sleep(100000);


	// 後処理系
	if (pCollection)
		pCollection->Release();
	if (pEnum)
		pEnum->Release();

	// COMの後処理(COMのお作法)
	CoUninitialize();
}

