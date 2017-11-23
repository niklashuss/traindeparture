#pragma once

#include <string>
#include <vector>
#include <functional>
#include "trainannouncement.h"

typedef std::function<bool(const TrainAnnouncement&)> FilterFunction_t;

class TrainAnnouncementParser {
public:
    TrainAnnouncementParser(std::string& text);
    void parse(std::vector<TrainAnnouncement>& trainAnnouncements, FilterFunction_t filterFunction);

private:
    std::string m_text;
};
