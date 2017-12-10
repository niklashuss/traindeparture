#pragma once

#include <string>
#include <vector>
#include <functional>
#include "departure.h"

typedef std::function<bool(const Departure&)> FilterFunction_t;

class TimeTableParser {
public:
    TimeTableParser(std::string& text);
    std::vector<Departure> parse(FilterFunction_t filterFunction);

private:
    std::string m_text;
};
