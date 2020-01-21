#include <Arduino.h>
#include <lmic.h>
#include <hal/hal.h>
#include <WiFi.h>
#include "esp_sleep.h"
#include "IMUSensor/IMUSensor.h"
#include "Message/Message.h"
#include "Message/MessageBuilder.h"
#include "Message/MessageBuilderManager.h"
#include "config.h"
// For storing data on non-volatile memory
#include <Preferences.h>

struct Sensor
{
    VariableType variableType;
    uint8_t id_sensor;
};

const int epoch_size = 15;
IMUSensor imu_unit;
Axis<int8_t> sensor_axis_readings[epoch_size];
Sensor inclination_sensor = {INCLINATION, 4};

int8_t angle_pitch_acc;
int8_t angle_roll_acc;

double acc_total_vector;

MessageBuilderManager msg_manager;
Preferences storage_manager;

// These callbacks are only used in over - the - air activation, so they are
// left empty here (we cannot leave them out completely unless
// DISABLE_JOIN is set in config.h, otherwise the linker will complain).
void os_getArtEui(u1_t *buf)
{
}
void os_getDevEui(u1_t *buf) {}
void os_getDevKey(u1_t *buf) {}

static osjob_t sendjob;
void do_send(osjob_t *j);
// Schedule TX every this many seconds (might become longer due to duty cycle limitations).
const unsigned TX_INTERVAL = 1;

void print_acceleration(Axis<int8_t> acceleration)
{
    Serial.print(acceleration.x);
    Serial.print("\t");
    Serial.print(acceleration.y);
    Serial.print("\t");
    Serial.print(acceleration.z);
    Serial.println();
}

// Pin mapping
const lmic_pinmap lmic_pins = {
    .nss = 18,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = LMIC_UNUSED_PIN, // was "14,"
    .dio = {26, 33, 32},
};

void onEvent(ev_t ev)
{
    switch (ev)
    {
    case EV_TXCOMPLETE:
        Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
        digitalWrite(BUILTIN_LED, LOW);
        // Schedule next transmission
        esp_sleep_enable_timer_wakeup(TX_INTERVAL * 1000000);
        esp_deep_sleep_start();
        //ESP.restart();
        //delay(TX_INTERVAL * 1000);
        do_send(&sendjob);
        break;
    }
}

void do_send(osjob_t *j)
{
    Serial.print("Variable type: ");
    Serial.println(inclination_sensor.variableType);

    Axis<double> acceleration;
    Axis<double> temp;

    for (int i = 0; i < 25; i++)
    {
        temp = imu_unit.get_acceleration();
        acceleration.x += temp.x;
        acceleration.y += temp.y;
        acceleration.z += temp.z;
        delay(4);
    }

    acceleration.x = acceleration.x / 25;
    acceleration.y = acceleration.y / 25;
    acceleration.z = acceleration.z / 25;

    acc_total_vector = sqrt((acceleration.x * acceleration.x) + (acceleration.y * acceleration.y) + (acceleration.z * acceleration.z));

    angle_pitch_acc = asin(acceleration.y / acc_total_vector) * 57.296;
    angle_roll_acc = asin(acceleration.x / acc_total_vector) * -57.296;

    Serial.print("pitch, x: ");
    Serial.print(angle_pitch_acc);
    Serial.print(" roll, y: ");
    Serial.println(angle_roll_acc);

    delay(4);

    Message msg = msg_manager.createInclinationMessage(inclination_sensor.id_sensor, inclination_sensor.variableType, angle_pitch_acc, angle_roll_acc);
    uint8_t *buffer = msg.getMessageArray();
    int buffer_size = msg.getMessageArraySize();

    LMIC_setTxData2(1, buffer, buffer_size, 0);

    delete[] buffer;
    Serial.println(F("Packet queued"));
    digitalWrite(BUILTIN_LED, HIGH);
}

void setup()
{
    Serial.begin(9600);

    //Turn off WiFi and Bluetooth
    WiFi.mode(WIFI_OFF);
    btStop();
    imu_unit.begin();

    // LMIC init
    os_init();
    // Reset the MAC state. Session and pending data transfers will be discarded.
    LMIC_reset();

    LMIC_setSession(0x1, DEVADDR, NWKSKEY, APPSKEY);

    LMIC_setupChannel(0, 903900000, DR_RANGE_MAP(DR_SF12, DR_SF7), BAND_CENTI);  // g-band
    LMIC_setupChannel(1, 904100000, DR_RANGE_MAP(DR_SF12, DR_SF7B), BAND_CENTI); // g-band
    LMIC_setupChannel(2, 904300000, DR_RANGE_MAP(DR_SF12, DR_SF7), BAND_CENTI);  // g-band
    LMIC_setupChannel(3, 904500000, DR_RANGE_MAP(DR_SF12, DR_SF7), BAND_CENTI);  // g-band
    LMIC_setupChannel(4, 904700000, DR_RANGE_MAP(DR_SF12, DR_SF7), BAND_CENTI);  // g-band
    LMIC_setupChannel(5, 904900000, DR_RANGE_MAP(DR_SF12, DR_SF7), BAND_CENTI);  // g-band
    LMIC_setupChannel(6, 905100000, DR_RANGE_MAP(DR_SF12, DR_SF7), BAND_CENTI);  // g-band
    LMIC_setupChannel(7, 905300000, DR_RANGE_MAP(DR_SF12, DR_SF7), BAND_CENTI);  // g-band
    LMIC_setupChannel(8, 904600000, DR_RANGE_MAP(DR_FSK, DR_FSK), BAND_MILLI);   // g2-band

    // Disable link check validation
    LMIC_setLinkCheckMode(0);

    // TTN uses SF9 for its RX2 window.
    LMIC.dn2Dr = DR_SF9;

    // Set data rate and transmit power for uplink (note: txpow seems to be ignored by the library)
    LMIC_setDrTxpow(DR_SF10, 14);

    do_send(&sendjob);
    pinMode(BUILTIN_LED, OUTPUT);
    digitalWrite(BUILTIN_LED, LOW);
}

void loop()
{
    os_runloop_once();
}