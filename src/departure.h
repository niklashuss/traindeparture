#pragma once

#include <string>

struct Departure {
    std::string advertisedTime;
    std::string estimatedTime;
    std::string toLocation;
    bool canceled;
};
