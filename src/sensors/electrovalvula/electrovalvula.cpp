#include <Arduino.h>
#include "../../debugger/debugger.h"

const int PIN_ELECTROVALVULA = 13;

int electrovalvulaLast = 0;

void setupElectrovalvula()
{
    pinMode(PIN_ELECTROVALVULA, OUTPUT);
    digitalWrite(PIN_ELECTROVALVULA, HIGH);
}

void loopElectrovalvula(int &electrovalvula, int &alarma, int &event)
{
    // 1 --> CERRADO!!!
    // 0 --> ABIERTO!!!

    if (alarma == 1)
    {
        // Si recibo un 1 y el último estado es 1
        if (electrovalvulaLast == 1)
        {
            digitalWrite(PIN_ELECTROVALVULA, HIGH); // CIERRO
            electrovalvulaLast = 0;
            electrovalvula = 0;
            event = 1;
        }
        alarma = 0;
    }
    else
    {
        if (electrovalvulaLast != electrovalvula)
        {
            if (electrovalvula == 1)
            {
                digitalWrite(PIN_ELECTROVALVULA, LOW); // CIERRO
            }
            else
            {
                digitalWrite(PIN_ELECTROVALVULA, HIGH); // ABRO
            }
            electrovalvulaLast = electrovalvula;
            event = 1;
        }
    }

    electrovalvula = electrovalvulaLast;
}
