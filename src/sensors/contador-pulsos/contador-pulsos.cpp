#include <Arduino.h>

int leida = 0;
double ultimoCaudal = 0.00;
long ultimaLectura = 0;
long t1 = millis();
long t2 = t1;
long intervalo = 0;
long intervaloRestar = intervalo;

const int EVENTO_LECTURA = 1;

void setupPulseCounter()
{
    pinMode(34, INPUT);
}

void loopPulseCounter(uint32_t &lectura, double &caudal, int &event)
{
    t1 = millis();
    int lecturaPIN35 = digitalRead(34);

    if (lecturaPIN35 == HIGH)
    {
        if (!leida)
        {
            lectura++;
            if ((lectura - ultimaLectura) > EVENTO_LECTURA)
            {
                event = 1;
                ultimaLectura = lectura;
            }
            leida = 1;
            // Caudal
            intervalo = (t1 - t2) / 1000;
            if (intervalo != 0)
            {
                caudal = round(1000 / intervalo);
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
        caudal = round(1000 / intervaloRestar);
    }
    if (caudal < 5)
    {
        caudal = 0;
    }
}