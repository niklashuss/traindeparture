#pragma once

#include <functional>
#include <thread>
#include "blockingqueue.h"

typedef std::function<void()> ActionFunction_t;

class ActionObject {
public:
    ActionObject();
    ~ActionObject();

    void addMessage(ActionFunction_t func);
private:
    std::thread m_thread;
    bool m_shouldLoop = true;
    BlockingQueue<ActionFunction_t> m_queue;
};
