#include "./boyas/boyas.h"
#include "./electrovalvula/electrovalvula.h"
#include "./contador/contador.h"

// ==================================================
//  Sensores
// ==================================================
int boyaBaja = 0;
int boyaMedia = 0;
int boyaAlta = 0;
int electrovalvula = 0;
long lectura = 0;
float caudal = 0;
int event = 0;

void setupSensors()
{
    setupBoyas();
    setupElectrovalvula();
    setupContador();
}

void loopSensors()
{
    loopBoyas(boyaBaja, boyaMedia, boyaAlta, event);
    loopElectrovalvula(electrovalvula, event);
    loopContador(lectura, caudal, event);
}

int getBoyaBaja()
{
    return boyaBaja;
}

int getBoyaMedia()
{
    return boyaMedia;
}

int getBoyaAlta()
{
    return boyaAlta;
}

int getElectrovalvula()
{
    return electrovalvula;
}

void setElectrovalvula(int electrovalvulaP)
{
    electrovalvula = electrovalvulaP;
}

long getLectura()
{
    return lectura;
}

void setLectura(long lecturaP)
{
    lectura = lecturaP;
    event = 1;
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