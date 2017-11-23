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

class XmlVisitorImpl : public XMLVisitor {
public:
    XmlVisitorImpl(FilterFunction_t& filterFunction) : m_filterFunction(filterFunction) {
    }

    virtual bool VisitEnter( const XMLElement& element, const XMLAttribute*  pAttribute) {

        if (strncmp(TRAIN_ANNOUNCEMENT, element.Value(), strlen(TRAIN_ANNOUNCEMENT)) == 0) {
            m_currentAnnouncement.advertisedTime = "";
            m_currentAnnouncement.estimatedTime = "";
            m_currentAnnouncement.fromLocation = "";
            m_currentAnnouncement.toLocation = "";
        } else if (strncmp(ADVERTISED_TIME, element.Value(), strlen(ADVERTISED_TIME)) == 0) {
            std::string time = element.GetText();
            m_currentAnnouncement.advertisedTime = time.substr(11, 5);
        } else if (strncmp(ESTIMATED_TIME, element.Value(), strlen(ESTIMATED_TIME)) == 0) {
            std::string time = element.GetText();
            m_currentAnnouncement.estimatedTime = time.substr(11, 5);
        } else if (strncmp(FROM_LOCATION, element.Value(), strlen(FROM_LOCATION)) == 0) {
            m_currentAnnouncement.fromLocation = element.FirstChildElement(LOCATION_NAME)->GetText();
        } else if (strncmp(TO_LOCATION, element.Value(), strlen(TO_LOCATION)) == 0) {
            m_currentAnnouncement.toLocation = element.FirstChildElement(LOCATION_NAME)->GetText();
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
