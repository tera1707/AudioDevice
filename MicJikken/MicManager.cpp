#include <cstdlib>
#include <memory>
#include <iostream>

#include "MicManager.h"
#include "CMMNotificationClient.h"

MicManager::MicManager()
{
    // COMの初期化(COMのお作法)
    auto hr = CoInitializeEx(0, COINIT_MULTITHREADED);
}

MicManager::~MicManager()
{
    // COMの後処理(COMのお作法)
    CoUninitialize();
}

void MicManager::Initialize()
{
    // マイクを探すための準備
    auto hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, IID_PPV_ARGS(&pEnum));

    // 通知を受け取る準備
    nc = new CMMNotificationClient(pEnum);
    pEnum->RegisterEndpointNotificationCallback(nc);
}

void MicManager::StoreMics()
{
    IMMDeviceCollection* pCollection = NULL;
    UINT deviceCount = 0;

    // マイク(オーディオエンドポイント)の列挙を実行
    auto hr = pEnum->EnumAudioEndpoints(EDataFlow::eCapture, DEVICE_STATE_ACTIVE, &pCollection);

    // とれた数を数える
    hr = pCollection->GetCount(&deviceCount);

    Mics.clear();

    // 関数をMicDataに渡すための準備
    auto a = std::bind(&MicManager::OnMuteChanged, this, std::placeholders::_1, std::placeholders::_2);

    for (int i = 0; i < deviceCount; i++)
    {
        IMMDevice* pEndpoint;

        // デバイスの情報を取る
        pCollection->Item(i, &pEndpoint);
        auto mc = std::make_unique<MicData>(pEndpoint, a);
        Mics.push_back(std::move(mc));
    }

    if (pCollection)
        pCollection->Release();
}

// -1:マイクがない 0:全部アンミュート 1:全部ミュート 2:混ざってる 
MuteState MicManager::GetAllMicMute()
{
    if (Mics.size() == 0)
        return MuteState::NoMic;

    int muteCount = 0;

    // ミュート状態のマイクの数を確認
    for (int i = 0 ; i < Mics.size(); i++)
    {
        if (Mics[i].get()->GetMute() != FALSE)
            muteCount++;
    }

    if (muteCount == Mics.size())
        return MuteState::AllMuted;
    if (muteCount == 0)
        return MuteState::AllUnmuted;
    else
        return MuteState::Mix;

}

void MicManager::SetAllNucNute(BOOL muteAll)
{
    if (Mics.size() == 0)
        return;

    // 全マイクのmuteを設定
    for (int i = 0; i < Mics.size(); i++)
    {
        Mics[i].get()->SetMute(muteAll);
    }
}

void MicManager::OnMuteChanged(std::wstring micName, bool isMuted)
{
    OutputDebugString((L"micName = " + micName + L"\r\n").c_str()); OutputDebugString((L"muted = " + std::to_wstring(isMuted) + L"\r\n").c_str());
}
