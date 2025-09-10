#pragma once
#include <ini.h>
#include <unordered_map>

namespace config {
    inline mINI::INIFile file("lr2ool.ini");
    inline mINI::INIStructure ini;

    void LoadConfig();
    void SaveConfig();
}