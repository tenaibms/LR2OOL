#include <Windows.h>
#include <chrono>

#include <LR2Bindings.hpp>

#include "graphics/gui.h"
#include "graphics/dx9.h"
#include "hooks/hooks.h"
#include "hooks/judgement.h"
#include "config/config.h"
#include "features/hiterror.h"

void Setup(HMODULE hModule)
{
    try {
        gui::Setup();
        while (!LR2::isInit) Sleep(1);
        dx9::Setup();
    }
    catch (const std::exception& error) {
        MessageBox(0, error.what(), "Error Occured", MB_OK | MB_ICONEXCLAMATION);
        goto cleanup;
    }
    config::LoadConfig();
    hooks::Setup();

    while (!GetAsyncKeyState(VK_END)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

cleanup:
    dx9::Destroy();
    gui::Destroy();
    hooks::Destroy();

    FreeLibraryAndExitThread(hModule, 0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: {
        LR2::Init();

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

