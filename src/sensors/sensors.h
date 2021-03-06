
#include <Arduino.h>

void setupSensors();
void loopSensors();
int getBoyaBaja();
void setBoyaBaja(int boyaBajaP);
int getBoyaMedia();
void setBoyaMedia(int boyaMediaP);
int getBoyaAlta();
void setBoyaAlta(int boyaAltaP);
int getElectrovalvula();
void setElectrovalvula(int electrovalvulaP, int manualP);
long getLectura();
void setLectura(long lecturaP);
float getCaudal();
int getEvent();
void setEvent(int eventP);
void setCaptador(int captadorP);
int getCaptador();