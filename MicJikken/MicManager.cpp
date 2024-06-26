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
    // �}�C�N��T�����߂̏���
    auto hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, IID_PPV_ARGS(&pEnum));

    // �ʒm���󂯎�鏀��
    nc = new CMMNotificationClient(pEnum);
    pEnum->RegisterEndpointNotificationCallback(nc);
}

void MicManager::StoreMics()
{
    IMMDeviceCollection* pCollection = NULL;
    UINT deviceCount = 0;

    // �}�C�N(�I�[�f�B�I�G���h�|�C���g)�̗񋓂����s
    auto hr = pEnum->EnumAudioEndpoints(EDataFlow::eCapture, DEVICE_STATE_ACTIVE, &pCollection);

    // �Ƃꂽ���𐔂���
    hr = pCollection->GetCount(&deviceCount);

    Mics.clear();

    // �֐���MicData�ɓn�����߂̏���
    auto a = std::bind(&MicManager::OnMuteChanged, this, std::placeholders::_1, std::placeholders::_2);

    for (int i = 0; i < deviceCount; i++)
    {
        IMMDevice* pEndpoint;

        // �f�o�C�X�̏������
        pCollection->Item(i, &pEndpoint);
        auto mc = std::make_unique<MicData>(pEndpoint, a);
        Mics.push_back(std::move(mc));
    }

    if (pCollection)
        pCollection->Release();
}

// -1:�}�C�N���Ȃ� 0:�S���A���~���[�g 1:�S���~���[�g 2:�������Ă� 
MuteState MicManager::GetAllMicMute()
{
    if (Mics.size() == 0)
        return MuteState::NoMic;

    int muteCount = 0;

    // �~���[�g��Ԃ̃}�C�N�̐����m�F
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

    // �S�}�C�N��mute��ݒ�
    for (int i = 0; i < Mics.size(); i++)
    {
        Mics[i].get()->SetMute(muteAll);
    }
}

void MicManager::OnMuteChanged(std::wstring micName, bool isMuted)
{
    OutputDebugString((L"micName = " + micName + L"\r\n").c_str()); OutputDebugString((L"muted = " + std::to_wstring(isMuted) + L"\r\n").c_str());
}
