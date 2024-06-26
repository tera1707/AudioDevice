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

// -1:�}�C�N���Ȃ� 0:�S���A���~���[�g 1:�S���~���[�g 2:�������Ă� 
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

