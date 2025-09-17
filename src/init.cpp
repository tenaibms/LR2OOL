#include <Windows.h>
#include <chrono>

#include <LR2Bindings.hpp>

#include "graphics/gui.h"
#include "graphics/dx9.h"
#include "hooks/hooks.h"
#include "hooks/judgement.h"
#include "config/config.h"
#include "features/hiterror.h"

#include <thread>

void Init()
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

    while (!GetAsyncKeyState(VK_END) || !GetAsyncKeyState(VK_ESCAPE)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

cleanup:
    config::SaveConfig();
    dx9::Destroy();
    gui::Destroy();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: {
        LR2::Init();

        using namespace hooks;

        replay_fix.Init();
        src_number.Init();
        judgement_processing.Init();
        mirror.Init();
        skin_misc.Init();
        cursor.Init();
        game_state.Init();
        input.Init();
        random.Init();

        std::thread(Init).detach();

        break;
    }
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        config::SaveConfig();
        break;
    }
    return TRUE;
}

