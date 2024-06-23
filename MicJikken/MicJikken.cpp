#include <windows.h>
#include <string>

#include "MicManager.h"

int main()
{
    MicManager micManager;

    micManager.Initialize();

    micManager.StoreMics();

    auto allMuted = micManager.GetAllMicMute();


    auto mute = FALSE;
    for (int i = 0; i < 10; i++)
    {
        micManager.SetAllNucNute(mute);
        
        auto allMuted = micManager.GetAllMicMute();
        
        auto log = std::wstring(L"All Muted? : " + std::to_wstring(allMuted) + L"\r\n");
        wprintf(log.c_str());

        mute = !mute;

        Sleep(3000);
    }

    system("pause"); //一時停止
}
