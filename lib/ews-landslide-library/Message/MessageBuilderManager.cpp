#include "MessageBuilderManager.h"

Message MessageBuilderManager::createAccelerationMessage(int var_id, VariableType var_type, uint8_t x, uint8_t y, uint8_t z)
{
    _builder.buildHeader(var_id, var_type);
    _builder.buildContentByte(x);
    _builder.buildContentByte(y);
    _builder.buildContentByte(z);
    return _builder.createMessage();
}

Message MessageBuilderManager::createRotationRateMessage(int var_id, VariableType var_type, uint8_t x, uint8_t y, uint8_t z)
{
    _builder.buildHeader(var_id, var_type);
    _builder.buildContentByte(x);
    _builder.buildContentByte(y);
    _builder.buildContentByte(z);
    return _builder.createMessage();
}

Message MessageBuilderManager::createInclinationMessage(int var_id, VariableType var_type, uint8_t x, uint8_t y)
{
    _builder.buildHeader(var_id, var_type);
    _builder.buildContentByte(x);
    _builder.buildContentByte(y);
    return _builder.createMessage();
}

Message MessageBuilderManager::createSoilMoistureMessage(int var_id, VariableType var_type, uint8_t soil_moisture)
{
    _builder.buildHeader(var_id, var_type);
    _builder.buildContentByte(soil_moisture);
    return _builder.createMessage();
}