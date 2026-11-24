#include "sensors/gyro.h"

TaskHandle_t taskReadGyroDataHandler = NULL;
BMI270 imu;

void setupGyro()
{
    Wire.begin();

    while(imu.beginI2C(i2cAddress) != BMI2_OK)
    {
        delay(1000);
    }
}

void taskReadGyroData(void *params)
{
    imu.getSensorData();
    while (true)
    {
    Serial.print(imu.data.accelX, 3);
    Serial.print(imu.data.accelY, 3);
    Serial.print(imu.data.accelZ, 3);
    Serial.print(imu.data.gyroX, 3);
    Serial.print(imu.data.gyroY, 3);
    Serial.println(imu.data.gyroZ, 3);

    vTaskDelay(pdMS_TO_TICKS(1000));
    }
}