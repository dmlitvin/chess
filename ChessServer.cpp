//
// Created by Dmitro LITVINENKO on 2019-11-10.
//

#include "ChessServer.hpp"

#include <unistd.h>
#include <poll.h>

#include <iostream>

ChessServer::ChessServer(int port) : _transporter(Transporter::Role::SERVER, port)
{
    std::cout << "chess server created, waiting for two players" << std::endl;

    while (_players.size() < 2) {

        pollfd fdList[_players.size() + 1];
        bzero(&fdList, sizeof(fdList));

        fdList[0].fd = _transporter.getEndpoint();
        fdList[0].events = POLLIN;

        for (size_t i = 0; i < _players.size(); ++i) {
            fdList[i + 1].fd = std::next(_players.begin(), i)->first;
            fdList[i + 1].events = POLLIN;
        }

        poll(fdList, _players.size() + 1, -1);

        auto playerIterator = _players.begin();

        for (size_t i = 0; i < _players.size(); ++i) {
            if (fdList[i + 1].revents) {
                std::cout << "player " << playerIterator->second << " disconnected" << std::endl;
                playerIterator = _players.erase(playerIterator);
            } else {
                ++playerIterator;
            }
        }

        if (fdList[0].revents) {
            acceptClient();
        }
    }

    std::cout << "clients connected successfully" << std::endl;
}

ChessServer::~ChessServer()
{
    for (const auto & playerEndpoint : _players) {
        close(playerEndpoint.first);
    }
}

void ChessServer::acceptClient()
{
    Transporter::endpoint_t playerEndpoint = _transporter.acceptClient();

    std::vector<char> serializedMessage = Transporter::receiveMessage(playerEndpoint);

    ChessMessage message(serializedMessage);

    std::cout << "accepted client " << message.getContent() << std::endl;

    _players.emplace_back(playerEndpoint, message.getContent());
}
