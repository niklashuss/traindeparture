#include "downloader.h"
#include <stdio.h>
#include "eventmanager.h"
#include "threadcontext.h"
#include "jobmanager.h"
#include "globalevents.h"
#include "file.h"
#include <memory.h>
#include "timetableparser.h"
#include "webclient.h"

static ThreadContext threadContext;

static void dl_loop() {
    while(threadContext.loop) {
        std::unique_lock<std::mutex> lock(threadContext.mutex);
        printf("Wake me up when needed.\n");
        threadContext.condition.wait(lock);
        Job* job = nullptr;
        while ((job = jm_get_job()) != nullptr) {
            job->func(job->data);
            jm_job_done();
        }
    }
}

void dl_shutdown_request() {
    printf("FileLoading: shutdown request.\n");
    threadContext.loop = false;
    threadContext.condition.notify_one();
}


static bool loadQuery(const char* pPath, std::string& query, const char* authenticateKey) {
    printf("Loading query\n");
    if (!File::exists(pPath)) {
        query = "File not found";
        printf("No query file found\n");
        return false;
    }

    long size = File::size(pPath);
    char buffer[size + 100];
    memset(buffer, 0, sizeof(buffer));

    File f;
    if (!f.open(pPath, FileMode::Read)) {
        query = "Unable to open file";
        printf("Unable to open file\n");
        return false;
    }

    int t = 0;
    const char* p = authenticateKey;
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



static bool downloadDepartures(const char* authKey) {
    printf("Downloading new train announcement\n");

    const unsigned short PORT = 80;
    const char* ADDRESS = "api.trafikinfo.trafikverket.se";
    std::string content;
    loadQuery("../res/query_departures_frovi.txt", content, authKey);

    printf("Connecting to server\n");
    WebClient webClient;
    WebClient::Status status = webClient.connect(ADDRESS, PORT);
    switch (status) {
        case WebClient::Status::SocketFailed: {
            std::string message = "TimeTableDownloader:: SocketFailed\n";
            printf("%s", message.c_str());
            return false;
        }

        case WebClient::Status::ConnectionFailed: {
            std::string message = "TimeTableDownloader:: ConnectionFailed\n";
            printf("%s", message.c_str());
            return false;
        }
    }

    printf("Sending request\n");
    std::string response;
    webClient.sendRequest(content, response);

    TimeTableParser parser(response);

    auto filter = [](const Departure& departure) {
        std::string to = departure.toLocation;
        if (to == "Lå" ||
            to == "Hpbg" ||
            to == "Öb" ||
            to == "My") {
            return true;
        }
        return false;
    };
    printf("Running filter\n");
    std::vector<Departure> announcements = parser.parse(filter);

    printf("TimeTableDownloader::download finished\n");
}


const char* AUTH_KEY_NAME = "../res/auth_key.txt";
const int AUTH_KEY_NAME_LENGTH = 32;

void dl_load_file(void* path) {

    downloadDepartures(AUTH_KEY_NAME);

    em_post_event(GLOBAL_EVENT_DOWNLOAD_FINISHED, nullptr);
}

void dl_callback(int id, void* data) {
    if (id == GLOBAL_EVENT_SHUTDOWN) {
        dl_shutdown_request();
    } else if (id == GLOBAL_EVENT_LOAD_FILE) {
        jm_add_job(1, dl_load_file, data);
        threadContext.condition.notify_one();
    }
}

bool dl_setup() {
    if (!File::exists(AUTH_KEY_NAME)) {
        printf("Error: Missing authentication key file!\n");
        return false;
    }

    char buffer[AUTH_KEY_NAME_LENGTH + 1];
    File f;
    memset(buffer, 0, sizeof(buffer));
    f.open(AUTH_KEY_NAME, FileMode::Read);
    f.read(buffer, AUTH_KEY_NAME_LENGTH);
    f.close();

    jm_setup();
    threadContext.loop = true;
    threadContext.thread = std::thread(dl_loop);
    em_add_listener(GLOBAL_EVENT_SHUTDOWN, dl_callback);
    em_add_listener(GLOBAL_EVENT_LOAD_FILE, dl_callback);
    return true;
}

void dl_wait_for_shutdown() {
    printf("FileLoading: Waiting for thread to end.\n");
    threadContext.thread.join();
}
