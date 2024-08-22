#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <safetyhook.hpp>
#include <chrono>

#include "graphics/gui.h"
#include "graphics/dx9.h"
#include "hooks/hooks.h"
#include "hooks/judgement.h"
#include "config/config.h"
#include "overlay/hiterror.h"

void Setup(HMODULE hModule)
{
    try {
        gui::Setup();
        dx9::Setup();
    }
    catch (const std::exception& error) {
        MessageBox(0, error.what(), "Error Occured", MB_OK | MB_ICONEXCLAMATION);
        goto cleanup;
    }

    /*
     * checks for f / s patch
     * eventually should move away from requiring it..
     * however it makes life much easier
     */
    if (*(int*)(0x44FAE3) == 0xCCCCCCCC) {
        MessageBoxA(0, "F/S patch is required", "Incompatible Version", MB_OK | MB_ICONERROR);
        goto cleanup;
    }

    config::LoadConfig();
    hooks::Setup();

    while(!GetAsyncKeyState(VK_END)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

cleanup:
    dx9::Destroy();
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
            reinterpret_cast<LPTHREAD_START_ROUTINE>(Setup),
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

