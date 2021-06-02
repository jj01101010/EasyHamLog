#pragma once

#include <string>

namespace EasyHamLog {
    struct QSO {
        std::string callsign;
        std::string name;
        std::string time;
        std::string date;
        std::string opmode;
        std::string band;
        std::string freq;
        std::string rst;
        std::string locator;
        std::string country;    // TODO: Make Seperate Struct with icon
        std::string contest_number;
        std::string contest_info;
    };

    struct Callsign_Prefix {
        std::string prefix;
        std::string country_name;
        int cq_region = 0;
        int itu_region = 0;
    };

    struct Preferences {
        bool useQRZ;
        std::string callsign;
        std::string locator;
    };
}
