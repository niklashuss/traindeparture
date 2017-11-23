#pragma once

#include <queue>
#include <mutex>

template<class T>
class BlockingQueue {
public:
    void addMessage(T message) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_messages.push(message);
    }
    T getMessage() {
        while (1) {
            std::unique_lock<std::mutex> lock(m_mutex);
            if (!m_messages.empty()) {
                lock.unlock();
                break;
            }
            lock.unlock();
        }
        T first = m_messages.front();
        m_messages.pop();
        return first;
    }
private:
    std::queue<T> m_messages;
    std::mutex m_mutex;
};

