#pragma once

#include <string>
#include <functional>
#include "client.h"

class WebClient {
public:
    enum class Status {
        Success,
        SocketFailed,
        ConnectionFailed,
        Uninitialized,
    };

    Status connect(const char* pAddress, unsigned short port);
    Status sendRequest(std::string& request, std::string& response);

private:
    Client m_client;
};
