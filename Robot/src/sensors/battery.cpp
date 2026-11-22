#include "sensors/battery.h"

TaskHandle_t taskReadBatteryHandler = NULL;

void taskReadBattery(void *params)
{
    waitForStart();
    StatusData *data = (StatusData*) params;
    while (1)
    {
        data->battery = random(0, 100);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }    
}