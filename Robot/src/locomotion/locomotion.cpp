#include "locomotion/locomotion.h"

TaskHandle_t taskForwardHandler = NULL;
EventGroupHandle_t eventGroupLocomotionEvent;

void taskForward(void *params)
{
    const TickType_t xFrequency = pdMS_TO_TICKS(1000);

    while (true)
    {
        xEventGroupWaitBits(eventGroupLocomotionEvent, BITSTART, pdFALSE, pdFALSE, portMAX_DELAY);
        TickType_t xLastWakeTime = xTaskGetTickCount();

        Serial.println("Going forwards! \n");

        vTaskDelayUntil( &xLastWakeTime, xFrequency );
    }  
}