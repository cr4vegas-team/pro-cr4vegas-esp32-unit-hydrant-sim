#include "contador-pulsos/contador-pulsos.h"
#include "temperatura-humedad/temperatura-humedad.h"

// ==================================================
//  Sensores
// ==================================================
uint32_t lectura = 0;
double caudal = 0;
byte temperature = 0;
byte humidity = 0;
int event = 0;

void setupSensors()
{
    setupPulseCounter();
    //setupTEMP();
}

void loopSensors()
{
    loopPulseCounter(lectura, caudal, event);
    //loopTEMP(temperature, humidity);
}

uint32_t getLectura()
{
    return lectura;
}

void setLectura(uint32_t lecturaP)
{
    lectura = lecturaP;
    event = 1;
}

double getCaudal()
{
    return caudal;
}

uint32_t getTemperature()
{
    return temperature;
}

uint32_t getHumidity()
{
    return humidity;
}

int getEvent()
{
    return event;
}

void setEvent(int eventP)
{
    event = eventP;
}