#pragma once
#include <iostream>

struct livedataDto
{
    int16_t battery;
    int16_t fullness;
    int16_t temperature;
};

class Livedata
{
private:
    int16_t battery{};
    int16_t fullness{};
    int16_t temperature{};
public:
    Livedata(int16_t battery, int16_t fullness, int16_t temperature);
    static Livedata fromDto(const livedataDto& dto);
    static livedataDto toDto(const Livedata liveData);
};

