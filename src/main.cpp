#include <Arduino.h>
#include "debugger/debugger.h"
#include "sim/sim.h"
#include "save/save.h"
#include "sensors/sensors.h"

TaskHandle_t taskSensors;
TaskHandle_t taskSIM;

long lastSave = 0;
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
    const TickType_t xFrequency = 13;

    setupSIM(xLastWakeTime);
    vTaskDelayUntil(&xLastWakeTime, xFrequency);

    // Iteración en el núcleo asignado
    for (;;)
    {
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
        loopSIM();

        long t1 = millis();

        if (t1 - lastSave > TIME_TO_SAVE)
        {
            lastSave = t1;
            saveDataOnFlash();
        }
    }
}

void runTaskSensors(void *pvParameters)
{
    printLNDebug("runTaskSensors running on core: " + (String)xPortGetCoreID());

    const TickType_t xFrequency = 10;
    TickType_t xLastWakeTime = xTaskGetTickCount();

    setupSensors();
    vTaskDelayUntil(&xLastWakeTime, xFrequency);

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