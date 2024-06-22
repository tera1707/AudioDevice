#include <windows.h>
#include <string>

#include "MicManager.h"

int main()
{
    MicManager micManager;

    micManager.Initialize();

    micManager.StoreMics();

    system("pause"); //一時停止
}
