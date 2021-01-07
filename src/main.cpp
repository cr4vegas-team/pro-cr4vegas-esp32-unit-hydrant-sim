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
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 10;

    // Initialise the xLastWakeTime variable with the current time.
    xLastWakeTime = xTaskGetTickCount();

    printLNDebug("runTaskSIM running on core: " + (String)xPortGetCoreID());

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
            saveDataOnFlash();
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

    printLNDebug("runTaskSensors running on core: " + (String)xPortGetCoreID());

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