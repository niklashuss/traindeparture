#include "application.h"
#include <thread>
#include "renderer.h"
#include "font.h"
#include "file.h"
#include <vector>
#include "actionobject.h"
#include "trainannouncementdownloader.h"
#include "image.h"
#include <chrono>
#include "texture.h"
#include <sstream>

using namespace std::chrono;

struct Text
{
    Image grey;
    Image rgb;
    Texture* pTexture;
    float x, y;
    int color;
};

void setup_time_texture(Text& time, int width, int height, int color, Application* pApplication) {
    time.grey = image_create(width, height, 1);
    time.rgb = image_create(width, height, 4);
    time.color = color;
    time.pTexture = pApplication->createStreamingTexture(width, height);
}

void clear_time(Text& time) {
    image_clear(time.grey);
    image_clear(time.rgb);
}

void update_time(Font& font, Text& time, const char* text, Application* pApplication)
{
    font_render(font, time.grey, text);
    image_copy(time.grey, time.rgb);
    image_make_transparent(time.rgb, time.color);
    time.pTexture->update(time.rgb);
}

const char* FONT_NAME = "../res/FreeSansBold.ttf";
const int UPDATE_TIME = 5 * 60;

class Game : public Application {
public:
    void onInit() {
        m_newTrainAnnouncementFlag = false;
        File f;
        int size = f.size("../res/auth_key.txt");
        char buffer[size + 1];
        memset(buffer, 0, size + 1);
        f.open("../res/auth_key.txt", FileMode::Read);
        f.read(buffer, size);
        f.close();
        std::string authKey = buffer;
        m_pDownloader = new TrainAnnouncementDownloader(authKey);
        m_pDownloader->download(m_newTrainAnnouncement, &m_newTrainAnnouncementFlag);

        renderer_init();

        font_load(m_largeFont, FONT_NAME, 75);
        font_load(m_mediumFont, FONT_NAME, 55);
        font_load(m_countDownFont, FONT_NAME, 48);

        int startX = 150;
        int startY = 80;
        int spaceX = 200;
        int spaceY = 15;
        m_advertisedTexts[0] = createText(startX + 0,     startY + 0,            256, 128, 0xff00f, m_largeFont, "n/a");
        m_estimatedTexts[0] = createText(startX + spaceX, startY + spaceY + 0,   256, 128, 0xff00f, m_mediumFont, "n/a");
        m_advertisedTexts[1] = createText(startX + 0,     startY + 100,          256, 128, 0xff00f, m_largeFont, "n/a");
        m_estimatedTexts[1] = createText(startX + spaceX, startY + spaceY + 100, 256, 128, 0xff00f, m_mediumFont, "n/a");
        m_advertisedTexts[2] = createText(startX + 0,     startY + 200,          256, 128, 0xff00f, m_largeFont, "n/a");
        m_estimatedTexts[2] = createText(startX + spaceX, startY + spaceY + 200, 256, 128, 0xff00f, m_mediumFont, "n/a");

        m_pCurrentTimeText = createText(595, 10, 256, 128, 0xff00f, m_mediumFont, "n/a");
        m_startTime = std::chrono::steady_clock::now();
    }

    void refreshTime() {
        for (int i = 0; i < 3; i++) {
            clear_time(*m_advertisedTexts[i]);
            clear_time(*m_estimatedTexts[i]);
        }

        int count = 3;
        int vecSize = m_currentTrainAnnouncement.size();
        if (vecSize < count) {
            count = vecSize;
        }

        for (int i = 0 ; i < count; i++) {
            const char* advTime = m_currentTrainAnnouncement[i].advertisedTime.c_str();
            const char* estTime = m_currentTrainAnnouncement[i].estimatedTime.c_str();
            update_time(m_largeFont, *m_advertisedTexts[i], advTime, this);
            if (strlen(estTime) == 0) {
                estTime = "i tid";
            }
            update_time(m_mediumFont, *m_estimatedTexts[i], estTime, this);
        }
    }

    void onUpdate() {
        clear_time(*m_pCurrentTimeText);
        updateCurrentTime();
        update_time(m_mediumFont, *m_pCurrentTimeText, m_currentTime.c_str(), this);

        auto currentTime = std::chrono::steady_clock::now();
        long timeDiff = std::chrono::duration_cast<std::chrono::seconds>(currentTime - m_startTime).count();
        if (timeDiff > UPDATE_TIME) {
            m_startTime = std::chrono::steady_clock::now();
            m_newTrainAnnouncementFlag = false;
            m_pDownloader->download(m_newTrainAnnouncement, &m_newTrainAnnouncementFlag);
            printf("Count of array = %lu\n", m_newTrainAnnouncement.size());
        }
        if (m_newTrainAnnouncementFlag) {
            m_lastDownloadTime = std::chrono::steady_clock::now();
            m_currentTrainAnnouncement.clear();
            for (TrainAnnouncement& announcement : m_newTrainAnnouncement) {
                m_currentTrainAnnouncement.push_back(announcement);
            }
            refreshTime();
            m_newTrainAnnouncement.clear();
            m_newTrainAnnouncementFlag = false;
        }

        renderer_set_clear_color(1.0f, 0.5f, 0.1f, 1.0f);
    }

    void onRender() {
        renderer_clear();
        renderer_execute();
        for (int i = 0; i < 3; i++) {
            Text* pText = m_advertisedTexts[i];
            int x = static_cast<int>(pText->x + 0.5f);
            int y = static_cast<int>(pText->y + 0.5f);
            render(pText->pTexture, x, y);
            pText = m_estimatedTexts[i];
            x = static_cast<int>(pText->x + 0.5f);
            y = static_cast<int>(pText->y + 0.5f);
            render(pText->pTexture, x, y);
        }

        int x = static_cast<int>(m_pCurrentTimeText->x + 0.5f);
        int y = static_cast<int>(m_pCurrentTimeText->y + 0.5f);
        render(m_pCurrentTimeText->pTexture, x, y);
    }

    void onShutdown() {
        font_release(m_largeFont);
        font_release(m_mediumFont);
        font_release(m_countDownFont);
        for (int i = 0; i < 3; i++) {
            delete m_advertisedTexts[i];
            m_advertisedTexts[i] = nullptr;
            delete m_estimatedTexts[i];
            m_estimatedTexts[i] = nullptr;
        }
        delete m_pDownloader;
        m_pDownloader = nullptr;
    }

    void onKeyEvent(int key, bool pressed)
    {
    }

private:
    Text* m_advertisedTexts[3];
    Text* m_estimatedTexts[3];
    Text* m_pCurrentTimeText;

    Font m_largeFont;
    Font m_mediumFont;
    Font m_countDownFont;
    std::vector<TrainAnnouncement> m_currentTrainAnnouncement;
    std::vector<TrainAnnouncement> m_newTrainAnnouncement;
    TrainAnnouncementDownloader* m_pDownloader;
    steady_clock::time_point m_startTime;
    steady_clock::time_point m_lastUpdate;
    steady_clock::time_point m_lastDownloadTime;
    bool m_newTrainAnnouncementFlag = false;
    std::string m_currentTime;

    Text* createText(int x, int y, int width, int height, int color, Font& font, const char* text) {
        Text* pText = new Text;
        setup_time_texture(*pText, width, height, color, this);
        update_time(font, *pText, text, this);
        pText->x = x;
        pText->y = y;
        return pText;
    }

    void updateCurrentTime() {
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        time_t tt = std::chrono::system_clock::to_time_t(now);
        tm localTime = *localtime(&tt);
        std::stringstream ss;
        int hour = localTime.tm_hour;
        if (hour < 10) {
            ss << "0";
        }
        ss << hour;
        ss << ":";
        int min = localTime.tm_min;
        if (min < 10) {
            ss << "0";
        }
        ss << min;
        ss << ":";
        int sec = localTime.tm_sec;
        if (sec < 10) {
            ss << "0";
        }
        ss << sec;
        m_currentTime = ss.str();
    }
};

int main(int argc, char *args[]) {
    printf("Hello\n");
    Game game;
    Game::Status status = game.create(800, 480);

    printf("status = %d\n", static_cast<int>(status));
    if (status == Game::Status::Success) {
        game.execute();
    }

    return 0;
}
