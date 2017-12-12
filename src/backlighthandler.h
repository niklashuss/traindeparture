#pragma once

#include "stopwatch.h"
#include <vector>

class BacklightHandler {
public:
    struct TimeSpan {
        TimeSpan(int from, int to) {
            this->from = from;
            this->to = to;
        }
        int from;
        int to;
    };

    BacklightHandler(int updateInterval);
    void addOffHours(const TimeSpan& timeSpan);
    void init();
    void update();
    void shutdown();
    bool isOn();

private:
    int m_updateInterval;
    Stopwatch m_stopwatch;
    std::vector<TimeSpan> m_offHours;
    bool m_on;
};
