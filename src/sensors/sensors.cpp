#include "./boyas/boyas.h"
#include "./electrovalvula/electrovalvula.h"
#include "./contador/contador.h"
#include "../debugger/debugger.h"

int boyaBaja = 0;
int boyaMedia = 0;
int boyaAlta = 0;
int electrovalvula = 0;
long lectura = 0;
float caudal = 0;
int event = 0;
int alarma = 0;
int manual = 0;

void comprobarBoyas();

void setupSensors()
{
    setupBoyas();
    setupElectrovalvula();
    setupContador();
}

void loopSensors()
{
    loopBoyas(boyaBaja, boyaMedia, boyaAlta, event);
    comprobarBoyas();
    loopElectrovalvula(electrovalvula, alarma, event);
    loopContador(lectura, caudal, event);
}

void comprobarBoyas()
{
    if (boyaMedia == 1 && manual == 0)
    {
        alarma = 1;
    }
    if (boyaAlta == 1)
    {
        alarma = 1;
        manual = 0;
    }
}

int getBoyaBaja()
{
    return boyaBaja;
}

void setBoyaBaja(int boyaBajaP)
{
    boyaBaja = boyaBajaP;
}

int getBoyaMedia()
{
    return boyaMedia;
}

void setBoyaMedia(int boyaMediaP)
{
    boyaMedia = boyaMediaP;
}

int getBoyaAlta()
{
    return boyaAlta;
}

void setBoyaAlta(int boyaAltaP)
{
    boyaAlta = boyaAltaP;
}

int getElectrovalvula()
{
    return electrovalvula;
}

void setElectrovalvula(int electrovalvulaP, int manualP)
{
    electrovalvula = electrovalvulaP;
    manual = manualP;
}

long getLectura()
{
    return lectura;
}

void setLectura(long lecturaP)
{
    lectura = lecturaP;
}

float getCaudal()
{
    return caudal;
}

int getEvent()
{
    return event;
}

void setEvent(int eventP)
{
    event = eventP;
}