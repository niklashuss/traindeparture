#include "trainannouncementdownloader.h"

#include "actionobject.h"
#include "file.h"
#include "trainannouncementparser.h"
#include "webclient.h"

static bool loadQuery(const char* pPath, std::string& query, std::string& authenticateKey) {
    printf("Path: %s\n", pPath);
    printf("key: %s\n", authenticateKey.c_str());
    if (!File::exists(pPath)) {
        query = "File not found";
        return false;
    }

    long size = File::size(pPath);
    char buffer[size + 100];
    memset(buffer, 0, size + 100);

    File f;
    if (!f.open(pPath, FileMode::Read)) {
        query = "Unable to open file";
        return false;
    }

    int t = 0;
    const char* p = authenticateKey.c_str();
    for (int i = 0; i < size; i++) {
        int c = f.getChar();
        if (c == '@') {
            for (int k = 0; k < 32; k++) {
                buffer[t] = p[k];
                t++;
            }
        } else {
            buffer[t] = c;
            t++;
        }
    }
    f.close();
    query = buffer;
    return true;
}

TrainAnnouncementDownloader::TrainAnnouncementDownloader(std::string& authenticateKey) : m_authenticateKey(authenticateKey) {
}

void TrainAnnouncementDownloader::download(std::vector<TrainAnnouncement>& newTrainAnnouncement, bool* pNewTrainAnnouncementFlag) {
    m_actionObject.addMessage([this, &newTrainAnnouncement, pNewTrainAnnouncementFlag]() {
        downloadTrainAnnouncement(newTrainAnnouncement, pNewTrainAnnouncementFlag);
    });
}

void TrainAnnouncementDownloader::downloadTrainAnnouncement(std::vector<TrainAnnouncement>& newTrainAnnouncement, bool* pNewTrainAnnouncementFlag) {
    printf("Downloading new train announcement\n");
    const unsigned short PORT = 80;
    const char* ADDRESS = "api.trafikinfo.trafikverket.se";
    std::string content;
    loadQuery("../res/query_departures_frovi.txt", content, m_authenticateKey);

    WebClient webClient;
    webClient.connect(ADDRESS, PORT);
    std::string response;
    WebClient::Status status = webClient.sendRequest(content, response);

    std::vector<TrainAnnouncement> announcements;
    TrainAnnouncementParser parser(response);

    int counter = 0;
    auto filter = [&counter](const TrainAnnouncement& announcement) {
        if (counter == 4) {
            return false;
        }

        std::string to = announcement.toLocation;
        if (to == "Lå" ||
            to == "Hpbg" ||
            to == "Öb" ||
            to == "My") {
            counter++;
            return true;
        }
        return false;
    };
    parser.parse(announcements, filter);
    for (const TrainAnnouncement& announcement :  announcements) {
//            printf("Advertised: %s\n", announcement.advertisedTime.c_str());
//            printf("Estimated: %s\n", announcement.estimatedTime.c_str());
//            printf("From location: %s\n", announcement.fromLocation.c_str());
//            printf("To location: %s\n", announcement.toLocation.c_str());
        newTrainAnnouncement.push_back(announcement);
    }
    *pNewTrainAnnouncementFlag = true;
}
