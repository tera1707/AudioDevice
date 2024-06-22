#include "MicData.h"

MicData::MicData(IMMDevice* mic, std::function<void(std::wstring micName, bool isMute)> onMicNotify)
{
	OnMicNotify = onMicNotify;
	pEndpoint = mic;

	LPWSTR id = NULL;
	auto hr = pEndpoint->GetId(&id);

	// �f�o�C�X�̃v���p�e�B�����
	pEndpoint->OpenPropertyStore(STGM_READ, &pProperties);

	// ������v���p�e�B����A�l���w�肵�Ď擾����
	PROPVARIANT vName;
	PropVariantInit(&vName);
	pProperties->GetValue(PKEY_Device_FriendlyName, &vName);

	// �G���h�|�C���g�̏������
	hr = pEndpoint->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL, (void**)&pAudioEndVol);

	// ���ʁE�~���[�g��Ԃ̕ύX���m
	vn = new CVolumeNotification(vName.pwszVal, OnMicNotify);
	pAudioEndVol->RegisterControlChangeNotify(vn);

#ifdef _DEBUG
	std::wstring outString = vName.pwszVal;
	wprintf(L"  %s\r\n", outString.c_str());
	wprintf(L"    %s\r\n", id);
	OutputDebugString(outString.c_str());
#endif
	// �㏈���n
	if (pProperties)
		pProperties->Release();
}
