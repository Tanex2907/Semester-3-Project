#include "Livedata/Livedata.h"

void Livedata::setBattery(int16_t val) {
    portENTER_CRITICAL(&lock);
    battery = val;
    portEXIT_CRITICAL(&lock);
}

void Livedata::setFullness(int16_t val) {
    portENTER_CRITICAL(&lock);
    fullness = val;
    portEXIT_CRITICAL(&lock);
}

void Livedata::setTemperature(int16_t val) {
    portENTER_CRITICAL(&lock);
    temperature = val;
    portEXIT_CRITICAL(&lock);
}

livedataDto Livedata::toDto() {
    portENTER_CRITICAL(&lock);
    livedataDto snapshot{battery, fullness, temperature};
    portEXIT_CRITICAL(&lock);
    return snapshot; // Returns a safe copy all at once
}

void Livedata::update(int16_t bat, int16_t full, int16_t temp) {
    portENTER_CRITICAL(&lock);
    battery = bat;
    fullness = full;
    temperature = temp;
    portEXIT_CRITICAL(&lock);
}