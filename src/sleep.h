#pragma once

class Sleep {
public:
    static void sleep(long milliseconds);

private:
    Sleep() = delete;
    Sleep(const Sleep& sleep) = delete;
    ~Sleep() = delete;
};
