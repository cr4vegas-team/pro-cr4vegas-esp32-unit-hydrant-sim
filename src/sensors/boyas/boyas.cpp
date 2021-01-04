// PINS
// D25 --> Electrovalvula

#include <Arduino.h>
#include "../../global/global.h"
#include "../sensors.h"

const int PIN_BOYA_BAJA = 27;
const int PIN_BOYA_MEDIA = 26;
const int PIN_BOYA_ALTA = 25;

void setupBoyas()
{
    pinMode(PIN_BOYA_BAJA, INPUT);
    pinMode(PIN_BOYA_MEDIA, INPUT);
    pinMode(PIN_BOYA_ALTA, INPUT);
}

void loopBoyas(int &boyaBaja, int &boyaMedia, int &boyaAlta, int &cambioBoyas)
{
    int lecturaBoyaBaja = digitalRead(PIN_BOYA_BAJA);
    int lecturaBoyaMedia = digitalRead(PIN_BOYA_MEDIA);
    int lecturaBoyaAlta = digitalRead(PIN_BOYA_ALTA);
    if (boyaBaja != lecturaBoyaBaja)
    {
        boyaBaja = lecturaBoyaBaja;
        cambioBoyas = 1;
    }
    if (boyaMedia != lecturaBoyaMedia)
    {
        boyaMedia = lecturaBoyaMedia;
        cambioBoyas = 1;
    }
    if (boyaAlta != lecturaBoyaAlta)
    {
        boyaAlta = lecturaBoyaAlta;
        cambioBoyas = 1;
    }
}
