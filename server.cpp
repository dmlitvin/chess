#include <iostream>

#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include "ChessServer.hpp"

int main(int ac, char **av)
{
    av[ac] = nullptr;

    ChessServer chessServer(4242);

    read(0, NULL, 1);

    std::cout << "chess_server" << std::endl;

    return 0;
}
