#include "Message.h"

Message::Message(const Message &msg)
{
    uint8_t *new_array = new uint8_t[msg._content_size];

    _varId = msg._varId;
    _varType = msg._varType;

    for (int i = 0; i < msg._content_size; i++)
    {
        new_array[i] = msg._content[i];
    }

    _content = new_array;
    _content_size = msg._content_size;
}

Message &Message::operator=(const Message &msg)
{
    _varId = msg._varId;
    _varType = msg._varType;
    _content_size = msg._content_size;

    uint8_t *new_array = new uint8_t[msg._content_size];

    for (int i = 0; i < msg._content_size; i++)
    {
        new_array[i] = msg._content[i];
    }

    delete[] _content;
    _content = new_array;

    // Just convention
    return *this;
}

uint8_t *Message::getMessageArray()
{
    uint8_t *msgArray = new uint8_t[_content_size + 2];

    msgArray[0] = _varId;
    msgArray[1] = _varType;

    for (int i = 0; i < _content_size; i++)
    {
        msgArray[i + 2] = _content[i];
    }

    return msgArray;
}

int Message::getMessageArraySize()
{
    return _content_size + 2;
}

void Message::addContentByte(uint8_t byte)
{
    _content_size++;

    if (_content_size > INITIAL_SIZE)
    {
        // New array with one extra element.
        uint8_t *new_array = new uint8_t[_content_size];

        for (int i = 0; i < _content_size - 1; i++)
        {
            new_array[i] = _content[i];
        }

        delete[] _content;

        _content = new_array;
    }

    _content[_content_size - 1] = byte;
}