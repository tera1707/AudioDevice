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
        auto log = std::wstring(L"All Muted? : " + std::wstring(allMuted == 0 ? L"All unmute." : (allMuted == 1 ? L"All mute." : L"Mix.")) + L"\r\n");
    }

    system("pause"); //一時停止

    {
        MicManager micManager;
        micManager.Initialize();
        micManager.StoreMics();
        auto allMuted = micManager.GetAllMicMute();
        auto log = std::wstring(L"All Muted? : " + std::wstring(allMuted == 0 ? L"All unmute." : (allMuted == 1 ? L"All mute." : L"Mix.")) + L"\r\n");
    }

    system("pause"); //一時停止
}
