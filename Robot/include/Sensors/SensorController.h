#pragma once
#include "Wire.h"
#include "Gyroscope.h"

class SensorController 
{

private:
Gyroscope gyro;

public:
    SensorController();
    void initi2c();
};