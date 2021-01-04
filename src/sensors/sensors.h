
#include <Arduino.h>

void setupSensors();
void loopSensors();
int getBoyaBaja();
int getBoyaMedia();
int getBoyaAlta();
int getElectrovalvula();
void setElectrovalvula(int electrovalvulaP);
long getLectura();
void setLectura(long lecturaP);
float getCaudal();
int getEvent();
void setEvent(int eventP);