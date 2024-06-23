#pragma once
#include <windows.h>
#include <mmdeviceapi.h>
#include <functiondiscoverykeys.h>
#include <endpointvolume.h>
#include <vector>

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

private:

	IMMDeviceEnumerator* pEnum = NULL;
	CMMNotificationClient* nc = NULL;

	std::vector<MicData> Mics;
};

