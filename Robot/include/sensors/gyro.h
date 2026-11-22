#include "Wire.h"
#include "Arduino.h"
#include "SparkFun_BMI270_Arduino_Library.h"

extern TaskHandle_t taskReadGyroDataHandler;
extern BMI270 imu;

uint8_t i2cAddress = BMI2_I2C_PRIM_ADDR; // 0x68

void setupGyro();
void taskReadGyroData(void *params);