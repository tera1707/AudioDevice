#pragma once
#include <windows.h>
#include <mmdeviceapi.h>
#include <functiondiscoverykeys.h>
#include <endpointvolume.h>
#include <string>

class CVolumeNotification : public IAudioEndpointVolumeCallback
{
    ULONG _cRef;
    std::wstring targetEndpointName;
public:
    CVolumeNotification(std::wstring targetEndpointName);
    ~CVolumeNotification();

    ULONG STDMETHODCALLTYPE AddRef();
    ULONG STDMETHODCALLTYPE Release();
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, VOID** ppvInterface);
    HRESULT STDMETHODCALLTYPE OnNotify(PAUDIO_VOLUME_NOTIFICATION_DATA pNotify);
};