#ifndef MESSAGEBUILDERMANAGER_H
#define MESSAGEBUILDERMANAGER_H

#include <Arduino.h>
#include "Message.h"
#include "MessageBuilder.h"
#include "VariableType/VariableType.h"

class MessageBuilderManager
{
public:
    MessageBuilderManager() : _builder(){};
    ~MessageBuilderManager() = default;

    Message createRotationRateMessage(int var_id, VariableType var_type, uint8_t x, uint8_t y, uint8_t z);
    Message createAccelerationMessage(int var_id, VariableType var_type, uint8_t x, uint8_t y, uint8_t z);
    Message createInclinationMessage(int var_id, VariableType var_type, uint8_t x, uint8_t y);
    Message createSoilMoistureMessage(int var_id, VariableType var_type, uint8_t soil_moisture);

private:
    MessageBuilder _builder;
};

#endif