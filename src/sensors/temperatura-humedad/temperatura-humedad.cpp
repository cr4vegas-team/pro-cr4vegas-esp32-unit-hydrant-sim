#include <Arduino.h>
#include <SimpleDHT.h>

#define SENSOR 5

SimpleDHT11 sensor;

uint32_t SENSOR_READ = 60000;
uint32_t lastSensorRead = 0;

void setupTEMP()
{
    pinMode(SENSOR, INPUT);
}

void loopTEMP(byte &temperature, byte &humidity)
{
    uint32_t t = millis();

    if (t - lastSensorRead > SENSOR_READ)
    {
        byte data[40] = {0};
        sensor.read(SENSOR, &temperature, &humidity, data);
        lastSensorRead = t;
    }
}