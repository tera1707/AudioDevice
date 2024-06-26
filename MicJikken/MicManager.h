#pragma once
#include <windows.h>
#include <mmdeviceapi.h>
#include <functiondiscoverykeys.h>
#include <endpointvolume.h>
#include <vector>
#include <memory>

#include "CMMNotificationClient.h"
#include "CVolumeNotification.h"
#include "MicData.h"

class MicManager
{
public:
	MicManager();
	~MicManager();
	void Initialize();
	void StoreMics();	

	int GetAllMicMute();
	void SetAllNucNute(BOOL muteAll);

	void OnMuteChanged(std::wstring micName, bool isMuted);
private:

	IMMDeviceEnumerator* pEnum = NULL;
	CMMNotificationClient* nc = NULL;

	std::vector<std::unique_ptr<MicData>> Mics;

	std::function<void(std::wstring micName, bool isMuted)> a;
};

