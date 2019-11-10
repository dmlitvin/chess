//
// Created by Dmitro LITVINENKO on 2019-11-10.
//

#include "ChessMessage.hpp"
#include <iostream>

message_t ChessMessage::getType() const {
    return _type;
}

const std::string & ChessMessage::getContent() const {
    return _content;
}

std::vector<char> ChessMessage::serialize() const
{
    std::vector<char> messageBuffer(_content.size() + 1);

    messageBuffer[0] = static_cast<char>(_type);

    std::copy(_content.begin(), _content.end(), messageBuffer.begin() + 1);

//    memcpy(messageBuffer.data() + 1, _content.data(), _content.size());

    return messageBuffer;
}

ChessMessage::ChessMessage(const std::string & content, message_t type)
: _type(type), _content(content) { }

ChessMessage::ChessMessage(const std::vector<char> &serializedMessage)
{
    _type = static_cast<message_t>(serializedMessage[0]);

    _content = { serializedMessage.data() + 1, serializedMessage.size() - 1 };
}
