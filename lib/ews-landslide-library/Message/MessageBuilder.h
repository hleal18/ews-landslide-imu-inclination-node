#ifndef MESSAGEBUILDER_H
#define MESSAGEBUILDER_H

#include <Arduino.h>
#include "Message.h"
#include "VariableType/VariableType.h"

class MessageBuilder
{
public:
    inline MessageBuilder() = default;
    inline ~MessageBuilder() { delete _message; };

    void buildHeader(int varId, VariableType varType);
    void buildContentByte(uint8_t byte);
    void resetMessage();

    Message createMessage();

private:
    Message *_message = new Message();
};

#endif