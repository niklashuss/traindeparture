#include "sleep.h"
#include <chrono>
#include <thread>

using namespace std::chrono;
using namespace std::this_thread;

void Sleep::sleep(long millisec)
{
    sleep_for(milliseconds(millisec));
}
