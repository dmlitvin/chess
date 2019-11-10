//
// Created by Dmitro LITVINENKO on 2019-11-10.
//

#include "Transporter.hpp"

#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>

#include <stdexcept>
#include <iostream>

Transporter::~Transporter()
{
    close(_endpoint);
}

Transporter::Transporter(Transporter::Role role, const int port) : _port(port), _role(role)
{
    _endpoint = socket(AF_INET, SOCK_STREAM, 0);

    if (_endpoint == -1) {
        perror("error creating endpoint");
        throw std::runtime_error("error creating transporter");
    }

    if (role == Transporter::Role::SERVER) {
        sockaddr_in sockaddrIn;
        bzero(&sockaddrIn, sizeof(sockaddr_in));

        sockaddrIn.sin_family = AF_INET;
        sockaddrIn.sin_port = htons(port);
        sockaddrIn.sin_addr.s_addr = INADDR_ANY;

        if (bind(_endpoint, (sockaddr*)&sockaddrIn, sizeof(sockaddr_in)) == -1) {
            perror("error binding sockaddr");
            throw std::runtime_error("error creating transporter::SERVER");
        }

        if (listen(_endpoint, 2) == -1) {
            perror("error on listen");
            throw std::runtime_error("error creating transporter::SERVER");
        }

        std::cout << "transporter::SERVER created successfully" << std::endl;
    }
    else if (role == Transporter::Role::CLIENT) {
        std::cout << "transporter::CLIENT created successfully" << std::endl;
    }
    else {
        throw std::invalid_argument("unknown transporter role");
    }
}

void Transporter::connect(const std::string & hostName)
{
    if (_role != Transporter::Role::CLIENT) {
        throw std::runtime_error("can't connect to " + hostName + ", transporter isn't client");
    }

    hostent *hostPtr = gethostbyname(hostName.c_str());

    if (!hostPtr) {
        perror("error resolving hostName");
        throw std::runtime_error("error connecting to " + hostName);
    }

    sockaddr_in sockaddrConnect;
    bzero(&sockaddrConnect, sizeof(sockaddr_in));

    sockaddrConnect.sin_family = AF_INET;
    sockaddrConnect.sin_port = htons(_port);
    sockaddrConnect.sin_addr.s_addr = ((in_addr*)hostPtr->h_addr)->s_addr;

    if (::connect(_endpoint, (sockaddr*)&sockaddrConnect, sizeof(sockaddr_in)) == -1) {
        perror("error connecting to server");
        throw std::runtime_error("error connecting to " + hostName);
    }

    std::cout << "transporter connected to " << hostName << std::endl;
}

Transporter::endpoint_t Transporter::acceptClient()
{
    if (_role != Transporter::Role::SERVER) {
        throw std::runtime_error("can't accept client, transporter isn't server");
    }

    sockaddr_in sockaddrIn;
    socklen_t socklen;

    endpoint_t endpoint = accept(_endpoint, (sockaddr*)&sockaddrIn, &socklen);

    if (_endpoint == -1) {
        perror("error accepting client");
        throw std::runtime_error("couldn't accept client");
    }

    return endpoint;
}

std::vector<char> Transporter::receiveMessage() {
    return receiveMessage(_endpoint);
}

void Transporter::sendMessage(const ChessMessage & message) {
    sendMessage(message, _endpoint);
}

void checkIOResult(ssize_t IOResult)
{
    if (IOResult == -1) {
        perror("IO operation error");
        throw std::runtime_error("error sending or receiving message");
    }

    if (!IOResult) {
        throw Transporter::closed_connection_exception();
    }
}

void Transporter::sendMessage(const ChessMessage & message, Transporter::endpoint_t endpoint)
{
    std::vector<char> serializedMsg = message.serialize();

    uint32_t serializedMsgLen = htonl(static_cast<uint32_t>(serializedMsg.size()));

    std::vector<char> messageBuffer(serializedMsg.size() + sizeof(serializedMsgLen));

    // TODO: change to std::copy (may be)

    memcpy(const_cast<char*>(messageBuffer.data()), &serializedMsgLen, sizeof(serializedMsgLen));
    memcpy(const_cast<char*>(messageBuffer.data()) + sizeof(serializedMsgLen), serializedMsg.data(), serializedMsg.size());

    ssize_t IOResult = write(endpoint, messageBuffer.data(), messageBuffer.size());

    checkIOResult(IOResult);
}

std::vector<char> Transporter::receiveMessage(Transporter::endpoint_t endpoint)
{
    uint32_t serializedMsgLen;

    ssize_t IOResult = read(endpoint, &serializedMsgLen, sizeof(serializedMsgLen));

    checkIOResult(IOResult);

    serializedMsgLen = ntohl(serializedMsgLen);

    std::vector<char> messageBuffer(serializedMsgLen);

    IOResult = read(endpoint, messageBuffer.data(), serializedMsgLen);

    checkIOResult(IOResult);

    return messageBuffer;
}

Transporter::endpoint_t Transporter::getEndpoint() const {
    return _endpoint;
}
