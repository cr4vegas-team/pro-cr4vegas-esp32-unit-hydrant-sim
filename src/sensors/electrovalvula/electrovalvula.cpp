// PINS
// D25 --> Electrovalvula

#include <Arduino.h>

const int PIN_ELECTROVALVULA = 13;

int electrovalvulaLast = 0;

void setupElectrovalvula()
{
    pinMode(PIN_ELECTROVALVULA, OUTPUT);
}

void loopElectrovalvula(int &electrovalvula, int &event)
{
    if (electrovalvulaLast != electrovalvula)
    {
        electrovalvulaLast = electrovalvula;
        event = 1;
        digitalWrite(PIN_ELECTROVALVULA, electrovalvulaLast);
    }
}
