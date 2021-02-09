#include <Arduino.h>

const int PIN_CONTADOR = 33;

int leida = 0;
int lecturaContador = 0;
int eventoLectura = 0;
double ultimoCaudal = 0.00;
long t1 = millis();
long t2 = t1;
long intervalo = 0;
long intervaloRestar = intervalo;

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
            intervalo = (t1 - t2) / 1000; // tiempo entre pulso y pulso en segundos

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
                    caudal = round(100 / intervalo);
                }
            }
            else
            {
                lectura++;
                eventoLectura++;
                if (intervalo != 0)
                {
                    caudal = round(1000 / intervalo);
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

    intervaloRestar = (t1 - t2) / 1000;

    if (intervaloRestar != 0 && intervaloRestar > intervalo)
    {
        if (captador == 0)
        {
            caudal = round(100 / intervaloRestar);
        }
        else
        {
            caudal = round(1000 / intervaloRestar);
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