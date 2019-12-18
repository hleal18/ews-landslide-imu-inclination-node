#include <Arduino.h>
#include "IMUSensor/IMUSensor.h"

IMUSensor imu;

double angle_pitch_acc;
double angle_roll_acc;

double acc_total_vector;

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(9600);
    imu.begin();
}

void loop()
{

    Axis<double> acceleration = imu.get_acceleration();

    acc_total_vector = sqrt((acceleration.x * acceleration.x) + (acceleration.y * acceleration.y) + (acceleration.z * acceleration.z));

    angle_pitch_acc = asin(acceleration.y / acc_total_vector) * 57.296;
    angle_roll_acc = asin(acceleration.x / acc_total_vector) * -57.296;

    Serial.print("pitch, x: ");
    Serial.print(angle_pitch_acc);
    Serial.print(" roll, y: ");
    Serial.println(angle_roll_acc);

    delay(4);
}