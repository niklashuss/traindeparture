#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>

struct ThreadContext {
    bool loop;
    std::thread thread;
    std::condition_variable condition;
    std::mutex mutex;
};
