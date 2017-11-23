#pragma once

#include <stdio.h>
#include <unistd.h>
#include <string.h> // memset
#include <netdb.h>

class Client {
public:
    enum class Status {
        Success,
        SocketFailed,
        ConnectionFailed,
        Uninitialized
    };

    Client();

    ~Client();

    Status connect(const char* pAddress, unsigned short port);
    int recieve(char* pBuffer, int length);
    int send(const char* pBuffer, int length);
    void close();

private:
    Status m_status = Status::Uninitialized;
    int m_socketDescriptor = -1;
    sockaddr_in m_address;
};
