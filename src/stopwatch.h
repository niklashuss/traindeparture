#pragma once

#include <chrono>

class StopWatch
{
public:
    void start();
    void stop();
    long resultAsSeconds();
    long resultAsMilliseconds();
    long resultAsNanoseconds();

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_stop;
};
