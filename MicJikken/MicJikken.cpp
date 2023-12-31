﻿#include <windows.h>
#include <mmdeviceapi.h>
#include <functiondiscoverykeys.h>
#include <endpointvolume.h>
#include <string>

#include "CMMNotificationClient.h"
#include "CVolumeNotification.h"

int main()
{
    HRESULT hr;
    IMMDeviceEnumerator* pEnum = NULL;
    IMMDeviceCollection* pCollection = NULL;
    UINT deviceCount = 0;

    IMMDevice* pEndpoint[8] = { NULL };
    IPropertyStore* pProperties[8] = { NULL };

    IAudioEndpointVolume* pAudioEndVol[8] = { NULL };
    CVolumeNotification* vn[8] = { NULL };

    // COMの初期化(COMのお作法)
    hr = CoInitializeEx(0, COINIT_MULTITHREADED);

    // COMからMMDeviceEnumeratorを取ってくる
    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, IID_PPV_ARGS(&pEnum));

    auto nc = new CMMNotificationClient(pEnum);
    pEnum->RegisterEndpointNotificationCallback(nc);


    while (1)
    {
        {
            // デフォルト(OSが現在使用している)オーディオエンドポイントを取る（今回はCapture(マイク)EP）
            IMMDevice* pDefEndpoint = NULL;
            hr = pEnum->GetDefaultAudioEndpoint(EDataFlow::eRender, ERole::eMultimedia, &pDefEndpoint);

            LPWSTR id = NULL;
            hr = pDefEndpoint->GetId(&id);

            // デバイスのプロパティを取る
            IPropertyStore* pDefProperties = { NULL };
            pDefEndpoint->OpenPropertyStore(STGM_READ, &pDefProperties);

            // 取ったプロパティから、値を指定して取得する
            PROPVARIANT vDefName;
            PropVariantInit(&vDefName);
            pDefProperties->GetValue(PKEY_Device_FriendlyName, &vDefName);

            wprintf(L"default Mic Device\r\n");
            wprintf(L"\r\n");
            wprintf(L"  %s\r\n", vDefName.bstrVal);
            wprintf(L"    %s\r\n", id);
        }

        {
            // すべてのCaptureデバイスを列挙する
            wprintf(L"\r\n");
            wprintf(L"List all Capture(mic) Device.\r\n");
            wprintf(L"\r\n");

            // オーディオエンドポイントの列挙を実行
            hr = pEnum->EnumAudioEndpoints(EDataFlow::eRender, DEVICE_STATE_ACTIVE, &pCollection);

            // とれた数を数える
            hr = pCollection->GetCount(&deviceCount);

            for (int i = 0; i < deviceCount; i++)
            {
                // デバイスの情報を取る
                pCollection->Item(i, &pEndpoint[i]);

                LPWSTR id = NULL;
                hr = pEndpoint[i]->GetId(&id);

                // デバイスのプロパティを取る
                pEndpoint[i]->OpenPropertyStore(STGM_READ, &pProperties[i]);

                // 取ったプロパティから、値を指定して取得する
                PROPVARIANT vName;
                PropVariantInit(&vName);
                pProperties[i]->GetValue(PKEY_Device_FriendlyName, &vName);

                // エンドポイントの情報を取る
                hr = pEndpoint[i]->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL, (void**)&pAudioEndVol[i]);

                // 音量・ミュート状態の変更検知
                vn[i] = new CVolumeNotification(vName.pwszVal);
                pAudioEndVol[i]->RegisterControlChangeNotify(vn[i]);

                //// エンドポイントに指示を出す（「IAudioEndpointVolume」のVolumeは、マイクの怨霊という意味の「ボリューム」とは別の意味っぽい）
                float getVolume = 0.0f;
                pAudioEndVol[i]->GetMasterVolumeLevelScalar(&getVolume);				// 音量を取得
                float setVolume = 0.25f;
                pAudioEndVol[i]->SetMasterVolumeLevelScalar(setVolume, &GUID_NULL);	// 音量を設定

                std::wstring outString = std::to_wstring(i) + L":" + vName.pwszVal + std::to_wstring(getVolume);
                wprintf(L"  %s\r\n", outString.c_str());
                wprintf(L"    %s\r\n", id);
                OutputDebugString(outString.c_str());

                // 後処理系
                if (pProperties)
                    pProperties[i]->Release();
            }
        }

        Sleep(1000);
        std::system("cls");
    }

    // 後処理系
    for (size_t i = 0; i < sizeof(pAudioEndVol) / sizeof(pAudioEndVol[0]); i++)
    {
        if (pAudioEndVol[i] != NULL)
            pAudioEndVol[i]->Release();
        if (pEndpoint[i] != NULL)
            pEndpoint[i]->Release();
    }
    if (pCollection)
        pCollection->Release();
    if (pEnum)
        pEnum->Release();

    // COMの後処理(COMのお作法)
    CoUninitialize();
}





/*
参考：
https://learn.microsoft.com/ja-jp/windows/win32/coreaudio/device-events

メモ：
OnDeviceAddedやOnDeviceRemovedは、オーディオデバイスを抜き差し（例えばUSBヘッドセットをUSBの穴に抜き差し）したときに来るのではなく、
デバイスのドライバをWindowsにインストールしたときにくるっぽい。
（その動作(デバイスインストール時にここを通る)を確認は出来てないのだが、USBヘッドセットを抜き差ししても、OnDeviceAddedやOnDeviceRemovedを通らなかった。）

Comの参考
https://www.keicode.com/winprimer/com-basics1-1-iunknown.php

*/

