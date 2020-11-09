#ifndef MESSAGE_H
#define MESSAGE_H

//typedef char uint8_t;
#include <Arduino.h>
//#include "VariableType.h"
#include "VariableType/VariableType.h";

class Message
{
public:
    Message(){};
    Message(int varId, VariableType varType) : _varId(varId), _varType(varType){};
    Message(const Message &msg);
    ~Message() { delete[] _content; };

    Message &operator=(const Message &msg);

    void setVariableId(int varId) { _varId = varId; }
    void setVariableType(VariableType varType) { _varType = varType; }
    void addContentByte(uint8_t byte);

    uint8_t *getMessageArray();
    int getMessageArraySize();

private:
    static const int INITIAL_SIZE = 10;

    int _varId;
    VariableType _varType;
    uint8_t *_content = new uint8_t[INITIAL_SIZE];
    int _content_size = 0;
};

#endif