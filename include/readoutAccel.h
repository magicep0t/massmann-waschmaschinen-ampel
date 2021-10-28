#ifndef READOUTACCEL_H
#define READOUTACCEL_H

#include "Wire.h"
#include <Arduino.h>

struct rawdata
    {
        int16_t AcX;
        int16_t AcY;
        int16_t AcZ;
    };
    struct scaleddata
    {
        float AcX;
        float AcY;
        float AcZ;
    };

class ReadoutAccel
{
private:

    // scalings for the RAW measurements
    const float MPU_GYRO_250_SCALE = 131.0;
    const float MPU_GYRO_500_SCALE = 65.5;
    const float MPU_GYRO_1000_SCALE = 32.8;
    const float MPU_GYRO_2000_SCALE = 16.4;
    const float MPU_ACCL_2_SCALE = 16384.0;
    const float MPU_ACCL_4_SCALE = 8192.0;
    const float MPU_ACCL_8_SCALE = 4096.0;
    const float MPU_ACCL_16_SCALE = 2048.0;
    
public:
    const uint8_t MPU_addr = 0x68; // I2C address of the MPU-6050
    
    ReadoutAccel();
    ~ReadoutAccel();

    bool checkI2c(unsigned char addr);
    void mpu6050Begin();
    rawdata mpu6050Read(bool Debug);
    void setMPU6050scales(uint8_t Accl);
    void getMPU6050scales(uint8_t &Accl);
    scaleddata convertRawToScaled(rawdata data_in, bool Debug);
};


#endif  // ReadoutAccel_H