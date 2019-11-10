#include <iostream>

#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include "Transporter.hpp"
#include "ChessFigure.hpp"

int main(int ac, char **av)
{
    av[ac] = nullptr;

//    std::cout << sizeof(ChessFigure::figure_type) << std::endl;

    Transporter transporter(Transporter::Role::CLIENT, 4242);

    transporter.connect("localhost");

    transporter.sendMessage(ChessMessage(av[1]));

    read(0, NULL, 1);

    std::cout << "chess_client" << std::endl;
    return 0;
}
