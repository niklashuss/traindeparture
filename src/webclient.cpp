#include "webclient.h"
#include <vector>

WebClient::Status remapStatusCode(Client::Status& clientStatus) {
    WebClient::Status webStatus;
    switch (clientStatus) {
        case Client::Status::SocketFailed:
            webStatus = WebClient::Status::SocketFailed;
            break;
        case Client::Status::ConnectionFailed:
            webStatus = WebClient::Status::ConnectionFailed;
            break;
        case Client::Status::Uninitialized:
            webStatus = WebClient::Status::Uninitialized;
            break;
        case Client::Status::Success:
            webStatus = WebClient::Status::Success;
            break;
        default:
            printf("WebClient: remapStatusCode not handled: %d\n\n", static_cast<int>(clientStatus));
            break;
    }
    return webStatus;
}

WebClient::Status WebClient::connect(const char* pAddress, unsigned short port) {
    Client::Status clientStatus = m_client.connect(pAddress, port);
    return remapStatusCode(clientStatus);
}

const int MAX_RESULT_BUFFER_SIZE = 1024 * 1024;
char resultBuffer[MAX_RESULT_BUFFER_SIZE];

WebClient::Status WebClient::sendRequest(std::string& request, std::string& response) {
    const char* NEW_LINE = "\r\n";
    std::string header;
    header += "POST /v1.3/data.xml HTTP/1.0";
    header += NEW_LINE;
    header += "Host: 192.168.1.160";
    header += NEW_LINE;
    header += "Content-Type: text/xml";
    header += NEW_LINE;
    header += "Content-Length: ";
    header += std::to_string(request.size());
    header += NEW_LINE;
    header += NEW_LINE;

    std::string all = header;
    all += request;
    int length = m_client.send(all.c_str(), all.size());
    printf("Sent %d bytes\n", length);

    memset(resultBuffer, 0, MAX_RESULT_BUFFER_SIZE);

    printf("Recieving data\n");
    int size = 0;
    if (length > 0) {
        while ((length = m_client.recieve(resultBuffer + size, MAX_RESULT_BUFFER_SIZE)) > 0) {
            size += length;
        }
    }
    printf("[%s]\n", resultBuffer);

    std::vector<std::string> lines2;
    int start = 0;
    int i = 0;
    while(1) {
        char c = resultBuffer[i];
        if (c == 13) {
            resultBuffer[i] = 0;
        }
        if (c == 10 ) {
            resultBuffer[i] = 0;
            lines2.push_back(std::string(&resultBuffer[start]));
            start = i + 1;
        }
        if (i == size) {
            lines2.push_back(std::string(&resultBuffer[start]));
            break;
        }
        i++;
    }

    std::vector<std::string> responseHeader;
    bool inHeader = true;
    for (std::string& line : lines2) {
        if (line.length() == 0) {
            inHeader = false;
            continue;
        }
        if (inHeader) {
            responseHeader.push_back(line);
        } else {
            response = line;
            break;
        }
    }

    return Status::Success;
}