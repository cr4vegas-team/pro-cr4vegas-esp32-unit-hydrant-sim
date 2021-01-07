#include <Arduino.h>
#include "debugger/debugger.h"
#include "sim/sim.h"
#include "save/save.h"
#include "sensors/sensors.h"

TaskHandle_t taskSensors;
TaskHandle_t taskSIM;

long timeToSave = 0;
long TIME_TO_SAVE = 600000;

void runTaskSensors(void *pvParameters);
void runTaskSIM(void *pvParameters);

void setup()
{
    setupDebug();
    setupSave();

    delay(1000);
    xTaskCreatePinnedToCore(runTaskSensors, "Task Sensors", 5120, NULL, 5, &taskSensors, 0);
    delay(1000);
    xTaskCreatePinnedToCore(runTaskSIM, "Task Sim", 10240, NULL, 6, &taskSIM, 1);
    delay(1000);
}

void runTaskSIM(void *pvParameters)
{
    printLNDebug("runTaskSIM running on core: " + (String)xPortGetCoreID());

    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = 10;
    vTaskDelayUntil(&xLastWakeTime, xFrequency);

    setupSIM(xLastWakeTime);

    // Iteración en el núcleo asignado
    for (;;)
    {
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
        loopSIM();
        if (getEvent() == 1)
        {
            saveDataOnFlash();
            setEvent(0);
        }
    }
}

void runTaskSensors(void *pvParameters)
{
    printLNDebug("runTaskSensors running on core: " + (String)xPortGetCoreID());

    const TickType_t xFrequency = 10;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    vTaskDelayUntil(&xLastWakeTime, xFrequency);

    setupSensors();

    // Iteración en el núcleo asignado
    for (;;)
    {
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
        loopSensors();
    }
}

void loop()
{
}