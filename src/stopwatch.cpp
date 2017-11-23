#include "stopwatch.h"

using namespace std::chrono;

void StopWatch::start()
{
    m_start = high_resolution_clock::now();
}

void StopWatch::stop()
{
    m_stop = high_resolution_clock::now();
}

long StopWatch::resultAsSeconds()
{
    return duration_cast<seconds>(m_stop - m_start).count();
}

long StopWatch::resultAsMilliseconds()
{
    return duration_cast<milliseconds>(m_stop - m_start).count();
}

long StopWatch::resultAsNanoseconds()
{
    return duration_cast<nanoseconds>(m_stop - m_start).count();
}
