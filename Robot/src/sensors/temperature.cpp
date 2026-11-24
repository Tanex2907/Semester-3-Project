#include "sensors/temperature.h"

TaskHandle_t taskReadTemperatureHandler = NULL;

void taskReadTemperature(void *params)
{
    waitForStart();
    StatusData *data = (StatusData*) params;
    while (1)
    {
        data->temp = random(0, 100);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}