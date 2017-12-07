#include "client.h"

Client::Client() {
}

Client::~Client() {
    close();
}

Client::Status Client::connect(const char* pAddress, unsigned short port) {
    printf("Client: setting up socket\n");
    m_socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socketDescriptor < 0) {
        m_status = Status::SocketFailed;
        return m_status;
    }

    memset(&m_address, '0', sizeof(m_address));

    addrinfo hints;
    memset(&hints, 0, sizeof(addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    addrinfo* addrResult = nullptr;
    int t = getaddrinfo(pAddress, "http", &hints, &addrResult);
    if (t < 0) {
        m_status = Status::ConnectionFailed;
        return m_status;
    }

    m_address.sin_port = port;

    printf("Client: trying to connect\n");
    if (::connect(m_socketDescriptor, addrResult->ai_addr, addrResult->ai_addrlen) < 0)
    {
        m_status = Status::ConnectionFailed;
        return m_status;
    }

    freeaddrinfo(addrResult);
    printf("Client: connect\n");
    m_status = Status::Success;
    return m_status;
}

int Client::recieve(char* pBuffer, int length) {
    if (m_status != Status::Success) {
        return -1;
    }

    printf("Client: recieve\n");
    return ::read(m_socketDescriptor, pBuffer, length);
}

int Client::send(const char* pBuffer, int length) {
    if (m_status != Status::Success) {
        return -1;
    }
    printf("Client: send\n");
    return ::write(m_socketDescriptor, pBuffer, length);
}

void Client::close() {
    if (m_socketDescriptor > -1) {
        ::close(m_socketDescriptor);
        m_socketDescriptor = -1;
    }
    m_status == Client::Status::Uninitialized;
}
