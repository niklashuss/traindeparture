#pragma once

#include <chrono>

class Stopwatch {
public:
    void start();
    long stop();
    long diff();

private:
    std::chrono::steady_clock::time_point m_start;
    std::chrono::steady_clock::time_point m_stop;
};

