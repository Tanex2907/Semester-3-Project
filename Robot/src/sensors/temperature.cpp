#include "sensors/temperature.h"

TaskHandle_t taskReadTemperatureHandler = NULL;

void taskReadTemperature(void *params)
{
    waitForStart();
    StatusData *data = (StatusData*) params;
    while (1)
    {
        data->temp = (int)((analogRead(PIN_LM35) * (ADC_VREF_mV / ADC_RESOLUTION)) / 10);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}