#include <format>
#include "config.h"

#include "hooks/replayfix.h"
#include "hooks/mirror.h"

#include "overlay/hiterror.h"

/* macro_hell */
#define IF_HAS_THEN_READ(section, label, variable_to_set, formatter) if(ini[section].has(label)) variable_to_set = formatter
#define IF_HAS_THEN_READ_BOOL(section, label, variable_to_set) IF_HAS_THEN_READ(section, label, variable_to_set, ini[section][label] == "true" ? true : false)
#define IF_HAS_THEN_READ_INT(section, label, variable_to_set) IF_HAS_THEN_READ(section, label, variable_to_set, stoi(ini[section][label]))
#define IF_HAS_THEN_READ_INT_16(section, label, variable_to_set) IF_HAS_THEN_READ(section, label, variable_to_set, stoi(ini[section][label], nullptr, 16))
#define TS_INT(x) std::to_string(x)
#define TS_16(x) std::format("{:06X}", x)
#define TS_BOOL(x) x ? "true" : "false"



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
    ini["hooks"]["mirror"] = TS_BOOL(hooks::mirror::enabled);
    ini["hooks"]["replay"] = TS_BOOL(hooks::replayfix::enabled);


    ini["hit_error"]["width"]	  = TS_INT(hiterror::width);
    ini["hit_error"]["height"]	  = TS_INT(hiterror::height);
    ini["hit_error"]["thickness"] = TS_INT(hiterror::thickness);
    ini["hit_error"]["ema"]		  = TS_BOOL(hiterror::using_ema);
    ini["hit_error"]["lines"]	  = TS_INT(hiterror::lines);

    ini["colors"]["ema"]          = TS_16(hiterror::colors::ema);
    ini["colors"]["pgreat"]       = TS_16(hiterror::colors::pgreat);
    ini["colors"]["great"]        = TS_16(hiterror::colors::great);
    ini["colors"]["good"]         = TS_16(hiterror::colors::good);
    ini["colors"]["cb"]           = TS_16(hiterror::colors::cb);

    file.write(ini, true);
}

#undef TS