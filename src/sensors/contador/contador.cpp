#include <Arduino.h>
#include "../../debugger/debugger.h"
#include "math.h"

// ACTUALIZADO!!! 10/02/2021

const int PIN_CONTADOR = 33;

int leida = 0;
int lecturaContador = 0;
int eventoLectura = 0;
double ultimoCaudal = 0.00;
long t1 = millis();
long t2 = t1;
double intervalo = 0;
double intervaloRestar = intervalo;

const int EVENTO_CAUDAL = 4;
const int EVENTO_LECTURA = 10;

void setupContador()
{
    pinMode(PIN_CONTADOR, INPUT);
}

void loopContador(long &lectura, float &caudal, int &captador, int &event)
{
    t1 = millis();
    int lecturaPIN35 = digitalRead(PIN_CONTADOR);

    if (lecturaPIN35 == HIGH)
    {
        if (!leida)
        {

            intervalo = (t1 - t2) / 1000.0; // tiempo entre pulso y pulso en milisegundos
            if (captador == 0)
            {
                lecturaContador++;
                if (lecturaContador == 10)
                {
                    lecturaContador = 0;
                    lectura++;
                    eventoLectura++;
                }
                if (intervalo != 0)
                {
                    caudal = roundf((100 / intervalo) * 100) / 100;
                }
            }
            else
            {
                lectura++;
                eventoLectura++;
                if (intervalo != 0)
                {
                    caudal = roundf((1000 / intervalo) * 100) / 100;
                }
            }

            leida = 1;
            if (caudal < 0)
            {
                caudal = 0;
            }
            t2 = t1;
        }
    }
    else
    {
        leida = 0;
    }

    intervaloRestar = (t1 - t2) / 1000; // Milisegundos

    if (intervaloRestar != 0 && intervaloRestar > intervalo) // Si es mayor que el último intervalo del pulso
    {
        if (captador == 0)
        {
            caudal = roundf((100 / intervaloRestar) * 100) / 100;
        }
        else
        {
            caudal = roundf((1000 / intervalo) * 100) / 100;
        }
    }

    if (caudal < 5)
    {
        caudal = 0;
    }

    if (abs(caudal - ultimoCaudal) > EVENTO_CAUDAL)
    {
        event = 1;
    }

    if (ultimoCaudal > 0 && caudal == 0)
    {
        event = 1;
    }
    if (eventoLectura >= EVENTO_LECTURA)
    {
        event = 1;
        eventoLectura = 0;
    }

    ultimoCaudal = caudal;
}