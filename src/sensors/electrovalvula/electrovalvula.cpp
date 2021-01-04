// PINS
// D25 --> Electrovalvula

#include <Arduino.h>
#include "../../global/global.h"

const int PIN_ELECTROVALVULA = 13;

int electrovalvulaLast = 0;

void setupElectrovalvula()
{
    pinMode(PIN_ELECTROVALVULA, OUTPUT);
}

void loopElectrovalvula(int &electrovalvula, int &cambioElectrovalvula)
{
    if (electrovalvulaLast != electrovalvula)
    {
        electrovalvulaLast = electrovalvula;
        cambioElectrovalvula = 1;
        digitalWrite(PIN_ELECTROVALVULA, electrovalvulaLast);
    }
}
