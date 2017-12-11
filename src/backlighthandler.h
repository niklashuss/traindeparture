#pragma once

#include "stopwatch.h"

class BacklightHandler {
public:
    BacklightHandler(int updateInterval);

    void init();
    void update();
    void shutdown();

private:
    int m_updateInterval;
    Stopwatch m_stopwatch;
};
