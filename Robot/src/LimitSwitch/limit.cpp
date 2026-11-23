#include "limitSwitch/limit.h"

void setupLimitSwitches()
{
    pinMode(PIN_LIMIT, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(PIN_LIMIT), isrRecognizeHit, FALLING);
}

void isrRecognizeHit()
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    BaseType_t xResult;

    xResult = xEventGroupSetBitsFromISR(eventGroupLocomotionEvent, BITHIT, &xHigherPriorityTaskWoken);

    if(xResult != pdFAIL)
    {
        portYIELD_FROM_ISR(pdTRUE);
    }
}