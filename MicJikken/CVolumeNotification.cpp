#include <windows.h>
#include <mmdeviceapi.h>
#include <functiondiscoverykeys.h>
#include <endpointvolume.h>
#include <string>

#include "CVolumeNotification.h"

CVolumeNotification::CVolumeNotification(std::wstring targetEndpointName) : _cRef(1) 
{
    this->targetEndpointName = targetEndpointName;
}

CVolumeNotification::~CVolumeNotification() {}

ULONG STDMETHODCALLTYPE CVolumeNotification::AddRef()
{
    return InterlockedIncrement(&_cRef);
}

ULONG STDMETHODCALLTYPE CVolumeNotification::Release()
{
    ULONG ulRef = InterlockedDecrement(&_cRef);
    if (0 == ulRef)
    {
        delete this;
    }
    return ulRef;
}

HRESULT STDMETHODCALLTYPE CVolumeNotification::QueryInterface(REFIID riid, VOID** ppvInterface)
{
    if (IID_IUnknown == riid)
    {
        AddRef();
        *ppvInterface = (IUnknown*)this;
    }
    else if (__uuidof(IAudioEndpointVolumeCallback) == riid)
    {
        AddRef();
        *ppvInterface = (IAudioEndpointVolumeCallback*)this;
    }
    else
    {
        *ppvInterface = NULL;
        return E_NOINTERFACE;
    }

    return S_OK;
}

HRESULT STDMETHODCALLTYPE CVolumeNotification::OnNotify(PAUDIO_VOLUME_NOTIFICATION_DATA pNotify)
{
    if (!pNotify)
    {
        return E_INVALIDARG;
    }

    auto mute = pNotify->bMuted;
    auto volume = pNotify->fMasterVolume;

    OutputDebugString((targetEndpointName + L" mute:" + std::to_wstring(mute) + L" volume:" + std::to_wstring(volume) + L"\r\n").c_str());

    return S_OK;
}
