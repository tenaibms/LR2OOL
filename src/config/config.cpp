#include <format>
#include "config.h"

#include "hooks/replayfix.h"
#include "hooks/mirror.h"
#include "hooks/drawnum.h"

#include "features/hiterror.h"

/* macro_hell */
#define READ(section, label, variable_to_set, formatter) if(ini[section].has(label)) variable_to_set = formatter
#define READ_BOOL(section, label, variable_to_set) READ(section, label, variable_to_set, ini[section][label] == "true" ? true : false)
#define READ_INT(section, label, variable_to_set) READ(section, label, variable_to_set, stoi(ini[section][label]))
#define READ_INT_16(section, label, variable_to_set) READ(section, label, variable_to_set, stoi(ini[section][label], nullptr, 16))

#define TS_BOOL(x) x ? "true" : "false"
#define TS_INT(x) std::to_string(x)
#define TS_INT_16(x) std::format("{:06X}", x)

#define SET_BOOL(section, label, variable) ini[section][label] = TS_BOOL(variable)
#define SET_INT(section, label, variable) ini[section][label] = TS_INT(variable)
#define SET_INT_16(section, label, variable) ini[section][label] = TS_INT_16(variable)

void config::LoadConfig()
{
    if (file.read(ini)) {
        if(ini.has("hooks")) {
            READ_BOOL("hooks", "mirror", hooks::mirror::enabled);
            READ_BOOL("hooks", "replay", hooks::replayfix::enabled);
        }

        if(ini.has("hit_error")) {
            READ_INT("hit_error", "width", hiterror::width);
            READ_INT("hit_error", "height", hiterror::height);
            READ_INT("hit_error", "thickness", hiterror::thickness);
            READ_INT("hit_error", "lines", hiterror::lines);
            READ_BOOL("hit_error", "ema", hiterror::using_ema);
            READ_BOOL("hit_error", "enabled", hiterror::enabled);
            READ_BOOL("hit_error", "smoothed", hiterror::smooth_ema);
        }
        
        if(ini.has("colors")) {
            READ_INT_16("colors", "ema", hiterror::colors::ema);
            READ_INT_16("colors", "pgreat", hiterror::colors::pgreat);
            READ_INT_16("colors", "great", hiterror::colors::great);
            READ_INT_16("colors", "good", hiterror::colors::good);
            READ_INT_16("colors", "cb", hiterror::colors::cb);
        }

        if (ini.has("skin_tweaks")) {
            READ_BOOL("reactive fast/slow position", "fs", drawnum::fs_toggle);
            READ_BOOL("reactive pacemaker position", "pacemaker", drawnum::pacemaker_toggle);
        }
    }
    else {
        config::SaveConfig();
    }
}

void config::SaveConfig() {
    SET_BOOL("hooks", "mirror", hooks::mirror::enabled);
    SET_BOOL("hooks", "mirror", hooks::replayfix::enabled);

    SET_INT("hit_error", "width", hiterror::width);
    SET_INT("hit_error", "height", hiterror::height);
    SET_INT("hit_error", "thickness", hiterror::thickness);
    SET_INT("hit_error", "lines", hiterror::lines);
    SET_BOOL("hit_error", "ema", hiterror::using_ema);
    SET_BOOL("hit_error", "smoothed", hiterror::smooth_ema);
    SET_BOOL("hit_error", "enabled", hiterror::enabled);

    SET_INT_16("colors", "ema", hiterror::colors::ema);
    SET_INT_16("colors", "pgreat", hiterror::colors::pgreat);
    SET_INT_16("colors", "great", hiterror::colors::great);
    SET_INT_16("colors", "good", hiterror::colors::good);
    SET_INT_16("colors", "cb", hiterror::colors::cb);

    SET_BOOL("reactive fast/slow position", "fs", drawnum::fs_toggle);
    SET_BOOL("reactive pacemaker position", "pacemaker", drawnum::pacemaker_toggle);

    file.write(ini, true);
}

#undef READ
#undef READ_BOOL
#undef READ_INT
#undef READ_INT_16

#undef TS_BOOL
#undef TS_INT
#undef TS_INT_16

#undef SET_BOOL
#undef SET_INT
#undef SET_INT_16