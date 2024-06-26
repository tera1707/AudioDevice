#include <windows.h>
#include <string>

#include "MicManager.h"

int main()
{
    {
        MicManager micManager;
        micManager.Initialize();
        micManager.StoreMics();
        auto allMuted = micManager.GetAllMicMute();
        auto log = std::wstring(L"All Muted? : " + std::wstring(allMuted == MuteState::AllUnmuted ? L"All unmute." : (allMuted == MuteState::AllMuted ? L"All mute." : L"Mix.")) + L"\r\n");
        wprintf(log.c_str());
    }

    system("pause"); //一時停止

    {
        MicManager micManager;
        micManager.Initialize();
        micManager.StoreMics();
        auto allMuted = micManager.GetAllMicMute();
        auto log = std::wstring(L"All Muted? : " + std::wstring(allMuted == MuteState::AllUnmuted ? L"All unmute." : (allMuted == MuteState::AllMuted ? L"All mute." : L"Mix.")) + L"\r\n");
        wprintf(log.c_str());
    }

    system("pause"); //一時停止
}
