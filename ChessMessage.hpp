//
// Created by Dmitro LITVINENKO on 2019-11-10.
//

#ifndef CHESS_MESSAGE_HPP
#define CHESS_MESSAGE_HPP

#include <vector>
#include <string>

enum class message_t {
    message,
    move
};

struct ChessMessage
{

    explicit ChessMessage(const std::vector<char> & serializedMessage);
    ChessMessage(const std::string & content, message_t type = message_t::message);

    std::vector<char> serialize() const;

    message_t getType() const;
    const std::string & getContent() const;

    ~ChessMessage() = default;

private:
    message_t _type;
    std::string _content;
};

#endif //CHESS_IMESSAGE_HPP
