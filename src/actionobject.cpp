
#include "actionobject.h"

ActionObject::ActionObject() : m_thread([=]{
    while (m_shouldLoop) {
        auto func = m_queue.getMessage();
        func();
    }
}) {
}

ActionObject::~ActionObject() {
    m_queue.addMessage([this]{m_shouldLoop = false;});
    m_thread.join();
}

void ActionObject::addMessage(ActionFunction_t func) {
    m_queue.addMessage(func);
}