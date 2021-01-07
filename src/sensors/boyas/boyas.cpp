// PINS
// D25 --> Electrovalvula

#include <Arduino.h>
#include "../../debugger/debugger.h"

const int PIN_BOYA_BAJA = 35;
const int PIN_BOYA_MEDIA = 34;
const int PIN_BOYA_ALTA = 32;

int lecturaBoyaBaja = 0;
int lecturaBoyaMedia = 0;
int lecturaBoyaAlta = 0;

void setupBoyas()
{
    pinMode(PIN_BOYA_BAJA, INPUT);
    pinMode(PIN_BOYA_MEDIA, INPUT);
    pinMode(PIN_BOYA_ALTA, INPUT);
}

void loopBoyas(int &boyaBaja, int &boyaMedia, int &boyaAlta, int &event)
{
    lecturaBoyaBaja = digitalRead(PIN_BOYA_BAJA);
    lecturaBoyaMedia = digitalRead(PIN_BOYA_MEDIA);
    lecturaBoyaAlta = digitalRead(PIN_BOYA_ALTA);

    if (boyaBaja != lecturaBoyaBaja || boyaMedia != lecturaBoyaMedia || boyaAlta != lecturaBoyaAlta)
    {
        event = 1;

        boyaBaja = lecturaBoyaBaja;
        boyaMedia = lecturaBoyaMedia;
        boyaAlta = lecturaBoyaAlta;

        printLNDebug("---");
        printLNDebug((String)lecturaBoyaBaja + ", " + (String)lecturaBoyaMedia + ", " + (String)lecturaBoyaAlta);
        printLNDebug((String)boyaBaja + ", " + (String)boyaMedia + ", " + (String)boyaAlta);
    }
}
