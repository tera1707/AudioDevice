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
    // COMからMMDeviceEnumeratorを取ってくる
    auto hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, IID_PPV_ARGS(&pEnum));

    nc = new CMMNotificationClient(pEnum);
    pEnum->RegisterEndpointNotificationCallback(nc);
}

void MicManager::StoreMics()
{
    IMMDeviceCollection* pCollection = NULL;
    UINT deviceCount = 0;

    // オーディオエンドポイントの列挙を実行
    auto hr = pEnum->EnumAudioEndpoints(EDataFlow::eCapture, DEVICE_STATE_ACTIVE, &pCollection);

    // とれた数を数える
    hr = pCollection->GetCount(&deviceCount);

    Mics.clear();

    for (int i = 0; i < deviceCount; i++)
    {
        IMMDevice* pEndpoint;

        // デバイスの情報を取る
        pCollection->Item(i, &pEndpoint);
        MicData mc(pEndpoint, [](std::wstring micName, bool isMuted) -> void { OutputDebugString((L"micName = " + micName + L"\r\n").c_str()); OutputDebugString((L"muted = " + std::to_wstring(isMuted) + L"\r\n").c_str()); });
        Mics.push_back(mc);
    }
}

// -1:マイクがない 0:全部アンミュート 1:全部ミュート 2:混ざってる 
int MicManager::GetAllMicMute()
{
    if (Mics.size() == 0)
        return -1;

    int muteCount = 0;

    // ミュート状態のマイクの数を確認
    for (int i = 0 ; i < Mics.size(); i++)
    {
        if (Mics[i].GetMute() != FALSE)
            muteCount++;
    }

    if (muteCount == Mics.size())
        return 1;
    if (muteCount == 0)
        return 0;
    else
        return 2;

}

void MicManager::SetAllNucNute(BOOL muteAll)
{
    if (Mics.size() == 0)
        return;

    // 全マイクのmuteを設定
    for (int i = 0; i < Mics.size(); i++)
    {
        Mics[i].SetMute(muteAll);
    }
}
