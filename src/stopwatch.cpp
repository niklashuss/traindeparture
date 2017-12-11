#include "stopwatch.h"
#include <stdio.h>

using namespace std::chrono;

void Stopwatch::start() {
    m_start = steady_clock::now();
}

long Stopwatch::stop() {
    m_stop = steady_clock::now();
    return duration_cast<seconds>(m_stop - m_start).count();
}

long Stopwatch::diff() {
    auto now = steady_clock::now();
    return duration_cast<seconds>(now - m_start).count();
}
