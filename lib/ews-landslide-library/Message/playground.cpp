#include <iostream>
#include "Message.h"
#include "../IMUSensor/IMUSensor.h";
using namespace std;

int main()
{
    VariableType varType = ACCELERATION;
    Message accelMsg(1, varType);
    cout << "HOla" << endl;

    return 0;
}