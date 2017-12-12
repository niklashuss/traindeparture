#include "backlighthandler.h"
#include "backlight.h"
#include <cstdio>

using namespace std::chrono;

namespace {

    int getCurrentHour() {
    system_clock::time_point now = system_clock::now();
    time_t tt = system_clock::to_time_t(now);
    tm local_tm = *localtime(&tt);
    return local_tm.tm_hour;
}

}

void BacklightHandler::addOffHours(const TimeSpan& timeSpan) {
    if (timeSpan.from > timeSpan.to) {
        printf("BacklightHandler: Error: the off hours submitted is faulty: %d > %d\n", timeSpan.from, timeSpan.to);
        return;
    }
    m_offHours.push_back(timeSpan);
}

BacklightHandler::BacklightHandler(int updateInterval)
    : m_updateInterval(updateInterval)
    , m_on(true) {
}

void BacklightHandler::init() {
    m_stopwatch.start();
}

void BacklightHandler::shutdown() {
    Backlight::on();
    Backlight::brightness(100);
    m_on = true;
}

bool isBacklightOn(std::vector<BacklightHandler::TimeSpan>& spans, int currentHour) {
    bool isOn = true;
    for(const auto& span : spans) {
        if (span.from >= currentHour && span.to <= currentHour) {
            isOn = false;
            break;
        }
    }
    return isOn;
}
void BacklightHandler::update() {
    if (m_stopwatch.diff() > m_updateInterval) {
        int hour = getCurrentHour();
        m_on = isBacklightOn(m_offHours, hour);
        if (m_on) {
            Backlight::on();
        } else {
            Backlight::off();
        }
        m_stopwatch.start();
    }
}

bool BacklightHandler::isOn() {
    return m_on;
}