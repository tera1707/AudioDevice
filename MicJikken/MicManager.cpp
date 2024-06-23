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

// -1:�}�C�N���Ȃ� 0:�S���A���~���[�g 1:�S���~���[�g 2:�������Ă� 
int MicManager::GetAllMicMute()
{
    if (Mics.size() == 0)
        return -1;

    int muteCount = 0;

    // �~���[�g��Ԃ̃}�C�N�̐����m�F
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

    // �S�}�C�N��mute��ݒ�
    for (int i = 0; i < Mics.size(); i++)
    {
        Mics[i].SetMute(muteAll);
    }
}
