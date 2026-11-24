#include "sensors/capacity.h"

TaskHandle_t taskReadCapacityHandler = NULL;

void taskReadCapacity(void *params)
{
    waitForStart();
    StatusData *data = (StatusData*) params;
    while (1)
    {
        data->fullness = random(0, 100);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}