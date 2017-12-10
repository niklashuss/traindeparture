#include "backlighthandler.h"
#include <chrono>
#include "backlight.h"

using namespace std::chrono;

namespace {
    int getCurrentHour() {
        system_clock::time_point now = system_clock::now();
        time_t tt = system_clock::to_time_t(now);
        tm local_tm = *localtime(&tt);
        return local_tm.tm_hour;
    }
}
BacklightHandler::BacklightHandler(int updateInterval)
    : m_updateInterval(updateInterval) {
}

void BacklightHandler::init() {
    m_stopwatch.start();
}

void BacklightHandler::update() {
    m_stopwatch.stop();
    if (m_stopwatch.diff() > m_updateInterval) {
        int hour = getCurrentHour();
        if (hour >= 21 || hour <= 5 || (hour >= 9 && hour <= 17)) {
            Backlight::off();
        } else {
            Backlight::on();
        }
        m_stopwatch.start();
    }
}
