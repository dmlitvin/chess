//
// Created by Dmitro LITVINENKO on 2019-11-10.
//

#ifndef CHESS_CHESSSERVER_HPP
#define CHESS_CHESSSERVER_HPP


#include "Transporter.hpp"

#include <list>

struct ChessServer
{
    explicit ChessServer(int port);

    void acceptClient();

    ~ChessServer();

private:
    Transporter _transporter;
    std::list<std::pair<Transporter::endpoint_t, std::string>> _players;
};


#endif //CHESS_CHESSSERVER_HPP
