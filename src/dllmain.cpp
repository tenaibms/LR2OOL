#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <safetyhook.hpp>
#include <chrono>
#include "gui.h"
#include "dx9.h"
#include "hooks.h"
#include "config.h"
#include "hiterror.h"

void setup(HMODULE hModule)
{
    bool error_occured = false;

    try {
        gui::Setup();
        dx9::Setup();
    }
    catch (const std::exception& error) {
        MessageBox(0, error.what(), "Error Occured", MB_OK | MB_ICONEXCLAMATION);
        error_occured = true;
    }

    config::LoadConfig();
    hooks::Setup();

    while(!GetAsyncKeyState(VK_END) || error_occured) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    dx9::Destroy();
    hooks::Destroy();
    gui::Destroy();

    FreeLibraryAndExitThread(hModule, 0);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: {
        const HANDLE thread = CreateThread(
            NULL,
            0,
            reinterpret_cast<LPTHREAD_START_ROUTINE>(setup),
            hModule,
            NULL,
            NULL
        );

        if (thread)
            CloseHandle(thread);
        break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

