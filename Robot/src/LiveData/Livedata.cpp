#include "LiveData/livedata.h"

Livedata::Livedata(int16_t battery, int16_t fullness, int16_t temperature)
    : battery(battery), fullness(fullness), temperature(temperature) {}

Livedata Livedata::fromDto(const livedataDto& dto) {
    return Livedata(dto.battery, dto.fullness, dto.temperature);
}

livedataDto Livedata::toDto(const Livedata liveData) {
    livedataDto dto;
    dto.battery = liveData.battery;
    dto.fullness = liveData.fullness;
    dto.temperature = liveData.temperature;
    return dto;
}