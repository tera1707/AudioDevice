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

// -1:マイクがない 0:全部アンミュート 1:全部ミュート 2:混ざってる 
enum MuteState
{
	NoMic,
	AllUnmuted,
	AllMuted,
	Mix,
};


class MicManager
{
public:
	MicManager();
	~MicManager();
	void Initialize();
	void StoreMics();	

	MuteState GetAllMicMute();
	void SetAllNucNute(BOOL muteAll);

	void OnMuteChanged(std::wstring micName, bool isMuted);
private:

	IMMDeviceEnumerator* pEnum = NULL;
	CMMNotificationClient* nc = NULL;

	std::vector<std::unique_ptr<MicData>> Mics;

	std::function<void(std::wstring micName, bool isMuted)> a;
};

