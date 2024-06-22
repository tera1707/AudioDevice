#pragma once
#include <windows.h>
#include <mmdeviceapi.h>
#include <functiondiscoverykeys.h>
#include <endpointvolume.h>
#include <string>
#include <functional>

class CVolumeNotification : public IAudioEndpointVolumeCallback
{
private:
    ULONG _cRef;
    std::wstring targetEndpointName;
    std::function<void(std::wstring micName, bool isMute)> OnMicNotify;

public:
    CVolumeNotification(std::wstring targetEndpointName, std::function<void(std::wstring micName, bool isMute)> onMicNotify);
    ~CVolumeNotification();

    ULONG STDMETHODCALLTYPE AddRef();
    ULONG STDMETHODCALLTYPE Release();
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, VOID** ppvInterface);
    HRESULT STDMETHODCALLTYPE OnNotify(PAUDIO_VOLUME_NOTIFICATION_DATA pNotify);
};