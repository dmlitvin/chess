#include <iostream>

#include <sys/socket.h>
#include <netdb.h>

int main(int ac, char **av)
{
    av[ac] = 0;
    int serverSock = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSock == -1) {
        perror("error creating endpoint");
        return 1;
    }

    sockaddr_in sockaddrIn;
    bzero(&sockaddrIn, sizeof(sockaddr_in));

    sockaddrIn.sin_family = AF_INET;
    sockaddrIn.sin_port = htons(4242);
    sockaddrIn.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSock, (sockaddr*)&sockaddrIn, sizeof(sockaddr_in)) == -1) {
        perror("error binding sockaddr");
        return 1;
    }

    if (listen(serverSock, 1) == -1) {
        perror("error on listen");
        return 1;
    }

    sockaddr_in clientSockaddr;
    socklen_t clientSocklen;

    int clientSock = accept(serverSock, (sockaddr*)&clientSockaddr, &clientSocklen);

    if (clientSock == -1) {
        perror("error accepting client");
        return 1;
    }

    std::cout << "chess_server" << std::endl;

    return 0;
}
