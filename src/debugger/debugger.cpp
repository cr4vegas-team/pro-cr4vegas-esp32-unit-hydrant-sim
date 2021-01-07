#include <Arduino.h>

#define SerialMon Serial

const int DEBUG = 1;

void setupDebug();
void printLNDebug(String msg);
void printDebug(String msg);

void setupDebug()
{
    SerialMon.begin(115200);
}

void printLNDebug(String msg)
{
    if (DEBUG == 1)
    {
        SerialMon.println(msg);
    }
}

void printDebug(String msg)
{
    if (DEBUG == 1)
    {
        SerialMon.print(msg);
    }
}