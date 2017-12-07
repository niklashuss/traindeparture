#include "trainannouncementparser.h"
#include <tinyxml2.h>
#include <functional>

using namespace tinyxml2;

const char* TRAIN_ANNOUNCEMENT = "TrainAnnouncement";
const char* ADVERTISED_TIME = "AdvertisedTimeAtLocation";
const char* ESTIMATED_TIME = "EstimatedTimeAtLocation";
const char* FROM_LOCATION = "FromLocation";
const char* TO_LOCATION = "ToLocation";
const char* LOCATION_NAME = "LocationName";
const char* CANCELED = "Canceled";

bool stringCompare(const char* a, const char* b) {
    return strncmp(a, b, strlen(a)) == 0;
}

class XmlVisitorImpl : public XMLVisitor {
public:
    XmlVisitorImpl(FilterFunction_t& filterFunction) : m_filterFunction(filterFunction) {
    }

    virtual bool VisitEnter( const XMLElement& element, const XMLAttribute*  pAttribute) {

        if (stringCompare(TRAIN_ANNOUNCEMENT, element.Value())) {
            m_currentAnnouncement.advertisedTime = "";
            m_currentAnnouncement.estimatedTime = "";
            m_currentAnnouncement.fromLocation = "";
            m_currentAnnouncement.toLocation = "";
        } else if (stringCompare(ADVERTISED_TIME, element.Value())) {
            std::string time = element.GetText();
            m_currentAnnouncement.advertisedTime = time.substr(11, 5);
        } else if (stringCompare(ESTIMATED_TIME, element.Value())) {
            std::string time = element.GetText();
            m_currentAnnouncement.estimatedTime = time.substr(11, 5);
        } else if (stringCompare(FROM_LOCATION, element.Value())) {
            m_currentAnnouncement.fromLocation = element.FirstChildElement(LOCATION_NAME)->GetText();
        } else if (stringCompare(TO_LOCATION, element.Value())) {
            m_currentAnnouncement.toLocation = element.FirstChildElement(LOCATION_NAME)->GetText();
        } else if (stringCompare(CANCELED, element.Value())) {
            m_currentAnnouncement.canceled = false;
            if (stringCompare("true", element.GetText())) {
                m_currentAnnouncement.canceled = true;
            }
        }
        return true;
    }

    virtual bool VisitExit( const XMLElement& element ) {
        if (strncmp(TRAIN_ANNOUNCEMENT, element.Value(), strlen(TRAIN_ANNOUNCEMENT)) == 0) {
            bool keep = m_filterFunction(m_currentAnnouncement);
            if (keep) {
/*
                printf("-------------------\n");
                printf(" adv: %s\n", m_currentAnnouncement.advertisedTime.c_str());
                printf(" est: %s\n", m_currentAnnouncement.estimatedTime.c_str());
                printf(" to: %s\n", m_currentAnnouncement.toLocation.c_str());
                printf(" from: %s\n", m_currentAnnouncement.fromLocation.c_str());
*/
                m_announcements.push_back(m_currentAnnouncement);
            }
        }
        return true;
    }

public:
    std::vector<TrainAnnouncement> getAnnouncements() {
        return m_announcements;
    }

private:
    TrainAnnouncement m_currentAnnouncement;
    std::vector<TrainAnnouncement> m_announcements;
    FilterFunction_t m_filterFunction;
};

TrainAnnouncementParser::TrainAnnouncementParser(std::string& text) : m_text(text) {
}

void TrainAnnouncementParser::parse(std::vector<TrainAnnouncement>& trainAnnouncements, std::function<bool(const TrainAnnouncement&)> filterFunction) {
    tinyxml2::XMLDocument doc;
    doc.Parse(m_text.c_str(), m_text.size());
    XMLElement* el = doc.FirstChildElement("RESPONSE");
    doc.SaveFile("response.txt", false);
    XmlVisitorImpl visitor(filterFunction);
    el->Accept(&visitor);
    trainAnnouncements = visitor.getAnnouncements();
}
