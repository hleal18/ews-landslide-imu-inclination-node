#include "MessageBuilder.h"

void MessageBuilder::buildHeader(int varId, VariableType varType)
{
    _message->setVariableId(varId);
    _message->setVariableType(varType);
}

void MessageBuilder::buildContentByte(uint8_t byte)
{
    _message->addContentByte(byte);
}

void MessageBuilder::resetMessage()
{
    delete _message;
    _message = new Message();
}

Message MessageBuilder::createMessage()
{
    Message msg = *_message;
    resetMessage();

    return msg;
}