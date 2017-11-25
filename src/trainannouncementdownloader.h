#pragma once

#include "trainannouncement.h"
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

class IDownloadCallback {
public:
    virtual void onDownloadFinished(std::vector<TrainAnnouncement>& announcements) = 0;
    virtual void onDownloadFailed(std::string& message) = 0;

private:
};

class TrainAnnouncementDownloader {
public:
    TrainAnnouncementDownloader(std::string& authenticateKey, IDownloadCallback* pDownloadCallback);
    ~TrainAnnouncementDownloader();
    void download();

private:
    std::string m_authenticateKey;
    IDownloadCallback* m_pDownloadCallback = nullptr;
    std::atomic_bool m_isDownloading;
    std::condition_variable m_waitForDownload;
    std::thread m_thread;
    bool m_doLoop = true;
    std::mutex m_mutex;
    bool downloadTrainAnnouncement();
    void run();
};
