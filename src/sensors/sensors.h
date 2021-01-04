
#include <Arduino.h>

void setupSensors();
void loopSensors();

uint32_t getLectura();
void setLectura(uint32_t lecturaP);
double getCaudal();

uint32_t getTemperature();
uint32_t getHumidity();

int getEvent();
void setEvent(int eventP);
int getSaving();
