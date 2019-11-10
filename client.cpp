#include <iostream>

#include <sys/socket.h>
#include <netdb.h>

int main(int ac, char **av)
{
    av[ac] = 0;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1) {
        perror("error creating endpoint");
        return 1;
    }

    sockaddr_in sockaddrConnect;
    bzero(&sockaddrConnect, sizeof(sockaddr_in));

    hostent *localhost = gethostbyname("localhost");

    if (!localhost) {
        perror("error resolving hostname");
        return 1;
    }

    sockaddrConnect.sin_family = AF_INET;
    sockaddrConnect.sin_port = htons(4242);
    sockaddrConnect.sin_addr.s_addr = ((in_addr*)localhost->h_addr)->s_addr;

    if (connect(sockfd, (sockaddr*)&sockaddrConnect, sizeof(sockaddr_in)) == -1) {
        perror("error connecting to server");
        return 1;
    }

    std::cout << "chess_client" << std::endl;
    return 0;
}
