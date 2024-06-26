#include "MicData.h"

MicData::MicData(IMMDevice* mic, std::function<void(std::wstring micName, bool isMute)> onMicNotify)
{
	OnMicNotify = onMicNotify;
	IMMDevice* pEndpoint = mic;

	IPropertyStore* pProperties;

	LPWSTR id = NULL;
	auto hr = pEndpoint->GetId(&id);

	// デバイスのプロパティを取る
	pEndpoint->OpenPropertyStore(STGM_READ, &pProperties);

	// 取ったプロパティから、値を指定して取得する
	PROPVARIANT vName;
	PropVariantInit(&vName);
	pProperties->GetValue(PKEY_Device_FriendlyName, &vName);

	// エンドポイントの情報を取る
	hr = pEndpoint->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL, (void**)&pAudioEndVol);

	// 音量・ミュート状態の変更検知
	vn = new CVolumeNotification(vName.pwszVal, OnMicNotify);
	pAudioEndVol->RegisterControlChangeNotify(vn);

#ifdef _DEBUG
	std::wstring outString = vName.pwszVal;
	wprintf(L"  %s\r\n", outString.c_str());
	wprintf(L"    %s\r\n", id);
	OutputDebugString(outString.c_str());
#endif
	// 後処理系
	if (pProperties)
		pProperties->Release();

	if (pEndpoint)
		pEndpoint->Release();
}

MicData::~MicData()
{
	if (pAudioEndVol)
		pAudioEndVol->Release();

	if (vn)
		vn->Release();
	
	wprintf(L"~MicData ---------------\r\n");
}

BOOL MicData::GetMute()
{
	if (pAudioEndVol == NULL)
		return FALSE;

	BOOL mute = FALSE;

	pAudioEndVol->GetMute(&mute);

	return mute;
}

void MicData::SetMute(BOOL mute)
{
	if (pAudioEndVol == NULL)
		return;

	pAudioEndVol->SetMute(mute, NULL);
}
