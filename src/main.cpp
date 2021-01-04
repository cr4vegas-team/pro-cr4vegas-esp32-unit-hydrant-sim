#include <Arduino.h>
#include "sim/sim.h"
#include "save/save.h"
#include "sensors/sensors.h"

#define SerialMon Serial

TaskHandle_t taskSensors;
TaskHandle_t taskSIM;

long timeToSave = 0;
long TIME_TO_SAVE = 600000;

void runTaskSensors(void *pvParameters);
void runTaskSIM(void *pvParameters);

void setup()
{
    SerialMon.begin(115200);
    xTaskCreatePinnedToCore(runTaskSensors, "Task Sensors", 5120, NULL, 5, &taskSensors, 0);
    delay(1000);
    xTaskCreatePinnedToCore(runTaskSIM, "Task Sim", 10240, NULL, 6, &taskSIM, 1);
    delay(1000);
}

void runTaskSIM(void *pvParameters)
{
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 10;

    // Initialise the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount();

    SerialMon.println("runTaskSIM running on core: " + (String)xPortGetCoreID());

    setLectura(readLectura()); // Cargo la lectura desde la flash

    setupSIM(xLastWakeTime);

    vTaskDelayUntil(&xLastWakeTime, xFrequency);

    for (;;)
    {
        // Wait for the next cycle.
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
        // Perform action here.
        loopSIM();

        if (getEvent() == 1)
        {
            saveLectura(getLectura());
            setEvent(0);
        }
    }
}

void runTaskSensors(void *pvParameters)
{
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 10;

    // Initialise the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount();

    SerialMon.println("runTaskSensors running on core: " + (String)xPortGetCoreID());

    setupSensors();

    vTaskDelayUntil(&xLastWakeTime, xFrequency);

    for (;;)
    {
        // Wait for the next cycle.
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
        // Perform action here.
        loopSensors();
    }
}

void loop()
{
}