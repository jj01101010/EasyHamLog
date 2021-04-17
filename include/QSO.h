#pragma once

#include <string>

namespace HamLog {
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
    };

    struct Callsign_Prefix {
        std::string prefix;
        std::string country_name;
        int cq_region;
        int itu_region;
    };
}
