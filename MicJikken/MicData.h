#pragma once
#include <functional>
#include "CMMNotificationClient.h"
#include "CVolumeNotification.h"

class MicData
{
public:
	MicData(IMMDevice* mic, std::function<void(std::wstring micName, bool isMute)> onMicNotify);

private:
	IMMDevice* pEndpoint;
	IPropertyStore* pProperties;
	IAudioEndpointVolume* pAudioEndVol;
	CVolumeNotification* vn;

	std::function<void(std::wstring micName, bool isMute)> OnMicNotify;
};
