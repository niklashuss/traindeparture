#include "stopwatch.h"
#include <stdio.h>

using namespace std::chrono;

void Stopwatch::start() {
    m_start = steady_clock::now();
}

void Stopwatch::stop() {
    m_stop = steady_clock::now();
}

long Stopwatch::diff() {
    return duration_cast<seconds>(m_stop - m_start).count();
}
