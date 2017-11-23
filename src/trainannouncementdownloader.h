#pragma once

#include "actionobject.h"
#include "trainannouncement.h"
#include <vector>

class TrainAnnouncementDownloader {
public:
    TrainAnnouncementDownloader(std::string& authenticateKey);

    void download(std::vector<TrainAnnouncement>& newTrainAnnouncement, bool* pNewTrainAnnouncementFlag);

private:
    std::string m_authenticateKey;
    ActionObject m_actionObject;
    void downloadTrainAnnouncement(std::vector<TrainAnnouncement>& newTrainAnnouncement, bool* pNewTrainAnnouncementFlag);
};
