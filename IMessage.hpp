//
// Created by Dmitro LITVINENKO on 2019-11-10.
//

#ifndef CHESS_IMESSAGE_HPP
#define CHESS_IMESSAGE_HPP

#include <string>

struct IMessage
{

    virtual std::string serialize() const = 0;

    virtual ~IMessage() = default;

};

#endif //CHESS_IMESSAGE_HPP
