#include <Arduino.h>

const int PIN_CONTADOR = 33;

int leida = 0;
int lecturaContador = 0;
double ultimoCaudal = 0.00;
long t1 = millis();
long t2 = t1;
long intervalo = 0;
long intervaloRestar = intervalo;

const int EVENTO_CAUDAL = 4;

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
            if (captador == 0)
            {
                lecturaContador++;
                if (lecturaContador == 10)
                {
                    lecturaContador = 0;
                    lectura++;
                }
            }
            else
            {
                lectura++;
            }

            leida = 1;
            // Caudal
            intervalo = (t1 - t2) / 1000; // tiempo entre pulso y pulso en segundos
            if (intervalo != 0)
            {
                caudal = round(100 / intervalo);
            }
            if (caudal < 0)
                caudal = 0;
            t2 = t1;
        }
    }
    else
    {
        leida = 0;
    }

    intervaloRestar = (t1 - t2) / 1000;
    if (intervaloRestar != 0 && intervaloRestar > intervalo)
    {
        caudal = round(100 / intervaloRestar);
    }

    if (caudal < 5)
    {
        caudal = 0;
    }

    if (caudal - ultimoCaudal > EVENTO_CAUDAL)
    {
        event = 1;
    }

    if (ultimoCaudal > 0 && caudal == 0)
    {
        event = 1;
    }

    ultimoCaudal = caudal;
}