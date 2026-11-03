#pragma once
#include <Arduino.h>
#include <freertos/FreeRTOS.h>

struct livedataDto {
    int16_t battery;
    int16_t fullness;
    int16_t temperature;
};

class Livedata {
private:
    int16_t battery{0};
    int16_t fullness{0};
    int16_t temperature{0};
    
    portMUX_TYPE lock = portMUX_INITIALIZER_UNLOCKED;

public:
    Livedata() = default;

    
    void setBattery(int16_t val);
    void setFullness(int16_t val);
    void setTemperature(int16_t val);

    livedataDto toDto();
    
    void update(int16_t bat, int16_t full, int16_t temp);
};