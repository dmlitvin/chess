//
// Created by Dmitro LITVINENKO on 2019-11-10.
//

#ifndef CHESS_TRANSPORTER_HPP
#define CHESS_TRANSPORTER_HPP


#include <vector>

#include "ChessMessage.hpp"

struct Transporter {

    using endpoint_t = int;

    enum class Role {
        SERVER,
        CLIENT
    };

    struct closed_connection_exception : std::exception {
        const char *what() const noexcept {
            return nullptr;
        }
    };

    Transporter(Role, int port);

    endpoint_t acceptClient();
    void connect(const std::string & hostName);

    endpoint_t getEndpoint() const;

    static void sendMessage(const ChessMessage&, endpoint_t);
    static std::vector<char> receiveMessage(endpoint_t);

    void sendMessage(const ChessMessage&);
    std::vector<char> receiveMessage();

    ~Transporter();

private:
    int _port;
    Role _role;
    endpoint_t _endpoint;
};


#endif //CHESS_TRANSPORTER_HPP
