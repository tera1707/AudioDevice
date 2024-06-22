#include <cstdlib>
#include <memory>
#include <iostream>

#include "MicManager.h"
#include "CMMNotificationClient.h"

MicManager::MicManager()
{
    // COM�̏�����(COM�̂���@)
    auto hr = CoInitializeEx(0, COINIT_MULTITHREADED);
}

MicManager::~MicManager()
{
    // COM�̌㏈��(COM�̂���@)
    CoUninitialize();
}

void MicManager::Initialize()
{
    // COM����MMDeviceEnumerator������Ă���
    auto hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, IID_PPV_ARGS(&pEnum));

    nc = new CMMNotificationClient(pEnum);
    pEnum->RegisterEndpointNotificationCallback(nc);
}

void MicManager::StoreMics()
{
    IMMDeviceCollection* pCollection = NULL;
    UINT deviceCount = 0;

    // �I�[�f�B�I�G���h�|�C���g�̗񋓂����s
    auto hr = pEnum->EnumAudioEndpoints(EDataFlow::eCapture, DEVICE_STATE_ACTIVE, &pCollection);

    // �Ƃꂽ���𐔂���
    hr = pCollection->GetCount(&deviceCount);

    Mics.clear();

    for (int i = 0; i < deviceCount; i++)
    {
        IMMDevice* pEndpoint;

        // �f�o�C�X�̏������
        pCollection->Item(i, &pEndpoint);
        MicData mc(pEndpoint, [](std::wstring micName, bool isMuted) -> void { OutputDebugString((L"micName = " + micName + L"\r\n").c_str()); OutputDebugString((L"muted = " + std::to_wstring(isMuted) + L"\r\n").c_str()); });
        Mics.push_back(mc);
    }
}
