#include "locomotion/locomotion.h"

TaskHandle_t taskForwardHandler = NULL;
TaskHandle_t taskBackwardHandler = NULL;
EventGroupHandle_t eventGroupLocomotionEvent;

void taskForward(void *params)
{
    const TickType_t xFrequency = pdMS_TO_TICKS(1000);

    while (true)
    {
        EventBits_t uxBits = xEventGroupWaitBits(eventGroupLocomotionEvent, BITSTART, pdFALSE, pdTRUE, portMAX_DELAY);
        if ((uxBits & BITHIT) == 0)
        {
            TickType_t xLastWakeTime = xTaskGetTickCount();

            Serial.println("Going forwards! \n");

            vTaskDelayUntil( &xLastWakeTime, xFrequency );
        }
    }  
}

void taskBackward(void *params)
{
    while (true)
    {
        xEventGroupWaitBits(eventGroupLocomotionEvent, BITHIT, pdFALSE, pdTRUE, portMAX_DELAY);
        const TickType_t xDuration = pdMS_TO_TICKS(2000);
        TickType_t xStartTick = xTaskGetTickCount();
        while ((xTaskGetTickCount() - xStartTick) < xDuration)
        {
            Serial.print("Woah that was the limit switch! \n");
            vTaskDelay(pdMS_TO_TICKS(500));
        }

        xEventGroupClearBits(eventGroupLocomotionEvent, BITHIT);
    }
}