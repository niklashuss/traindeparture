#include "trainannouncementdownloader.h"
#include "file.h"
#include "trainannouncementparser.h"
#include "webclient.h"
#include <chrono>

static bool loadQuery(const char* pPath, std::string& query, std::string& authenticateKey) {
    printf("Loading query\n");
    if (!File::exists(pPath)) {
        query = "File not found";
        printf("No query file found\n");
        return false;
    }

    long size = File::size(pPath);
    char buffer[size + 100];
    memset(buffer, 0, size + 100);

    File f;
    if (!f.open(pPath, FileMode::Read)) {
        query = "Unable to open file";
        printf("Unable to open file\n");
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

TrainAnnouncementDownloader::TrainAnnouncementDownloader(std::string& authenticateKey, IDownloadCallback* pCallback)
        : m_authenticateKey(authenticateKey)
        , m_pDownloadCallback(pCallback)
        , m_isDownloading(false)
        , m_thread(&TrainAnnouncementDownloader::run, this) {
}

TrainAnnouncementDownloader::~TrainAnnouncementDownloader() {
    printf("TrainAnnouncementDownloader::stopping download thread\n");
    m_waitForDownload.notify_one();
    m_doLoop = false;
    m_thread.join();
    printf("TrainAnnouncementDownloader::download thread is finished\n");
}

void TrainAnnouncementDownloader::download() {
    printf("TrainAnnouncementDownloader::download()\n");
    if (m_isDownloading) {
        printf("Already a download in queue.\n");
        return;
    }
    m_isDownloading = true;
    downloadTrainAnnouncement();
    m_isDownloading = false;
    //    m_waitForDownload.notify_one();
}

void TrainAnnouncementDownloader::run() {
  /*
    while(m_doLoop) {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_waitForDownload.wait_for(lock, std::chrono::milliseconds(100));
        if (!m_isDownloading) {
            continue;
        }
        m_isDownloading = true;
        downloadTrainAnnouncement();
        m_isDownloading = false;
    }
  */
}

bool TrainAnnouncementDownloader::downloadTrainAnnouncement() {
    printf("Downloading new train announcement\n");
    if (m_pDownloadCallback == nullptr) {
        printf("TrainAnnouncementDownloader:: No callback set\n");
        m_isDownloading = false;
        return false;
    }

    const unsigned short PORT = 80;
    const char* ADDRESS = "api.trafikinfo.trafikverket.se";
    std::string content;
    loadQuery("../res/query_departures_frovi.txt", content, m_authenticateKey);

    WebClient webClient;
    WebClient::Status status = webClient.connect(ADDRESS, PORT);
    switch (status) {
        case WebClient::Status::SocketFailed: {
            std::string message = "TrainAnnouncementDownloader:: SocketFailed";
            m_pDownloadCallback->onDownloadFailed(message);
            m_isDownloading = false;
            return false;
        }

        case WebClient::Status::ConnectionFailed: {
            std::string message = "TrainAnnouncementDownloader:: ConnectionFailed";
            m_pDownloadCallback->onDownloadFailed(message);
            m_isDownloading = false;
            return false;
        }
    }

    std::string response;
    webClient.sendRequest(content, response);

    TrainAnnouncementParser parser(response);

    auto filter = [](const TrainAnnouncement& announcement) {
        std::string to = announcement.toLocation;
        if (to == "Lå" ||
            to == "Hpbg" ||
            to == "Öb" ||
            to == "My") {
            return true;
        }
        return false;
    };
    std::vector<TrainAnnouncement> announcements = parser.parse(filter);

    printf("TrainAnnouncementDownloader::download finished\n");
    m_pDownloadCallback->onDownloadFinished(announcements);
    m_isDownloading = false;
}
