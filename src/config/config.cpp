#include <format>
#include "config.h"

#include "hooks/replayfix.h"
#include "hooks/mirror.h"

#include "features/hiterror.h"

/* macro_hell */
#define IF_HAS_THEN_READ(section, label, variable_to_set, formatter) if(ini[section].has(label)) variable_to_set = formatter
#define IF_HAS_THEN_READ_BOOL(section, label, variable_to_set) IF_HAS_THEN_READ(section, label, variable_to_set, ini[section][label] == "true" ? true : false)
#define IF_HAS_THEN_READ_INT(section, label, variable_to_set) IF_HAS_THEN_READ(section, label, variable_to_set, stoi(ini[section][label]))
#define IF_HAS_THEN_READ_INT_16(section, label, variable_to_set) IF_HAS_THEN_READ(section, label, variable_to_set, stoi(ini[section][label], nullptr, 16))

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
            IF_HAS_THEN_READ_BOOL("hooks", "mirror", hooks::mirror::enabled);
            IF_HAS_THEN_READ_BOOL("hooks", "replay", hooks::replayfix::enabled);
        }

        if(ini.has("hit_error")) {
            IF_HAS_THEN_READ_INT("hit_error", "width", hiterror::width);
            IF_HAS_THEN_READ_INT("hit_error", "height", hiterror::height);
            IF_HAS_THEN_READ_INT("hit_error", "thickness", hiterror::thickness);
            IF_HAS_THEN_READ_BOOL("hit_error", "ema", hiterror::using_ema);
            IF_HAS_THEN_READ_BOOL("hit_error", "smooth", hiterror::smooth_ema);
            IF_HAS_THEN_READ_INT("hit_error", "lines", hiterror::lines);
        }
        
        if(ini.has("colors")) {
            IF_HAS_THEN_READ_INT_16("colors", "ema", hiterror::colors::ema);
            IF_HAS_THEN_READ_INT_16("colors", "pgreat", hiterror::colors::pgreat);
            IF_HAS_THEN_READ_INT_16("colors", "great", hiterror::colors::great);
            IF_HAS_THEN_READ_INT_16("colors", "good", hiterror::colors::good);
            IF_HAS_THEN_READ_INT_16("colors", "cb", hiterror::colors::cb);
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

    SET_INT_16("colors", "ema", hiterror::colors::ema);
    SET_INT_16("colors", "pgreat", hiterror::colors::ema);
    SET_INT_16("colors", "great", hiterror::colors::ema);
    SET_INT_16("colors", "good", hiterror::colors::ema);
    SET_INT_16("colors", "cb", hiterror::colors::ema);

    file.write(ini, true);
}

#undef IF_HAS_THEN_READ
#undef IF_HAS_THEN_READ_BOOL
#undef IF_HAS_THEN_READ_INT
#undef IF_HAS_THEN_READ_INT_16

#undef TS_BOOL
#undef TS_INT
#undef TS_INT_16

#undef SET_BOOL
#undef SET_INT
#undef SET_INT_16