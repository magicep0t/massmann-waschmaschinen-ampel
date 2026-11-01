#ifndef READOUTACCEL_H
#define READOUTACCEL_H

#include <Wire.h>
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
    // Scalings for the RAW measurements (g)
    const float MPU_ACCL_2_SCALE = 16384.0f;
    const float MPU_ACCL_4_SCALE = 8192.0f;
    const float MPU_ACCL_8_SCALE = 4096.0f;
    const float MPU_ACCL_16_SCALE = 2048.0f;

public:
    static const uint8_t MPU_addr = 0x68; // I2C address of MPU-6050 (AD0 to GND)

    ReadoutAccel();
    ~ReadoutAccel();

    bool checkI2c(uint8_t addr);
    void mpu6050Begin();
    rawdata mpu6050Read(bool debug = false);
    void setMPU6050scales(uint8_t acclScale);
    void getMPU6050scales(uint8_t &acclScale);
    scaleddata convertRawToScaled(rawdata dataIn, bool debug = false);
};

#endif // READOUTACCEL_H
