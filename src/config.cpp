#include <format>
#include "config.h"
#include "hooks.h"
#include "hiterror.h"
#include "gui.h"

#define TS(x) std::to_string(x)
#define TS_16(x) std::format("{:06X}", x)


void config::LoadConfig()
{
    if (file.read(ini)) {
        hooks::mirror_hook_enabled = ini["hooks"]["mirror"] == "true" ? true : false;

        hiterror::width			= stoi(ini["hit_error"]["width"]);
        hiterror::height		= stoi(ini["hit_error"]["height"]);
        hiterror::thickness		= stoi(ini["hit_error"]["thickness"]);
        hiterror::using_ema		= ini["hit_error"]["ema"] == "true" ? true : false;
        hiterror::lines			= stoi(ini["hit_error"]["lines"]);

        hiterror::colors::ema	 = stoi(ini["colors"]["ema"], nullptr, 16);
        hiterror::colors::pgreat = stoi(ini["colors"]["pgreat"], nullptr, 16);
        hiterror::colors::great	 = stoi(ini["colors"]["great"], nullptr, 16);
        hiterror::colors::good   = stoi(ini["colors"]["good"], nullptr, 16);
        hiterror::colors::cb     = stoi(ini["colors"]["cb"], nullptr, 16);

    }
    else {
        config::SaveConfig();
    }
}

void config::SaveConfig() {
    ini["hooks"]["mirror"] = hooks::mirror_hook ? "true" : "false";

    ini["hit_error"]["width"]	  = TS(hiterror::width);
    ini["hit_error"]["height"]	  = TS(hiterror::height);
    ini["hit_error"]["thickness"] = TS(hiterror::thickness);
    ini["hit_error"]["ema"]		  = hiterror::using_ema ? "true" : "false";
    ini["hit_error"]["lines"]	  = TS(hiterror::lines);

    ini["colors"]["ema"]          = TS_16(hiterror::colors::ema);
    ini["colors"]["pgreat"]       = TS_16(hiterror::colors::pgreat);
    ini["colors"]["great"]        = TS_16(hiterror::colors::great);
    ini["colors"]["good"]         = TS_16(hiterror::colors::good);
    ini["colors"]["cb"]           = TS_16(hiterror::colors::cb);

    file.write(ini, true);
}

#undef TS