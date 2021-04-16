#pragma once

#include <string>

namespace HamLog {
    struct QSO {
        std::string callsign;
        std::string name;
        std::string time;
        std::string date;
        std::string band;
        std::string freq;
        std::string locator;
        std::string country;    // TODO: Make Seperate Struct with icon
    };
}
