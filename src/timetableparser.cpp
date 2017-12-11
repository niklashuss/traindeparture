#include "timetableparser.h"
#include <tinyxml2.h>
#include <functional>

using namespace tinyxml2;

const char* TRAIN_ANNOUNCEMENT = "TrainAnnouncement";
const char* ADVERTISED_TIME = "AdvertisedTimeAtLocation";
const char* ESTIMATED_TIME = "EstimatedTimeAtLocation";
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
            m_currentDeparture.advertisedTime = "";
            m_currentDeparture.estimatedTime = "";
            m_currentDeparture.toLocation = "";
            m_currentDeparture.late = false;
            m_currentDeparture.canceled = false;
        } else if (stringCompare(ADVERTISED_TIME, element.Value())) {
            std::string time = element.GetText();
            m_currentDeparture.advertisedTime = time.substr(11, 5);
        } else if (stringCompare(ESTIMATED_TIME, element.Value())) {
            std::string time = element.GetText();
            if (time.size() > 0) {
                m_currentDeparture.late = true;
            }
            m_currentDeparture.estimatedTime = time.substr(11, 5);
        } else if (stringCompare(TO_LOCATION, element.Value())) {
            m_currentDeparture.toLocation = element.FirstChildElement(LOCATION_NAME)->GetText();
        } else if (stringCompare(CANCELED, element.Value())) {
            m_currentDeparture.canceled = false;
            if (stringCompare("true", element.GetText())) {
                m_currentDeparture.canceled = true;
            }
        }
        return true;
    }

    virtual bool VisitExit( const XMLElement& element ) {
        if (stringCompare(TRAIN_ANNOUNCEMENT, element.Value())) {
            bool keep = m_filterFunction(m_currentDeparture);
            if (keep) {
/*
                printf("-------------------\n");
                printf(" adv: %s\n", m_currentDeparture.advertisedTime.c_str());
                printf(" est: %s\n", m_currentDeparture.estimatedTime.c_str());
                printf(" to: %s\n", m_currentDeparture.toLocation.c_str());
*/
                m_departures.push_back(m_currentDeparture);
            }
        }
        return true;
    }

public:
    std::vector<Departure> getDepartures() {
        return m_departures;
    }

private:
    Departure m_currentDeparture;
    std::vector<Departure> m_departures;
    FilterFunction_t m_filterFunction;
};

TimeTableParser::TimeTableParser(std::string& text) : m_text(text) {
}

std::vector<Departure> TimeTableParser::parse(std::function<bool(const Departure&)> filterFunction) {
    tinyxml2::XMLDocument doc;
    doc.Parse(m_text.c_str(), m_text.size());
    XMLElement* el = doc.FirstChildElement("RESPONSE");
    doc.SaveFile("response.txt", false);
    XmlVisitorImpl visitor(filterFunction);
    el->Accept(&visitor);
    return visitor.getDepartures();
}
