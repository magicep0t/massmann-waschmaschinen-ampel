#include "readoutAccel.h"

ReadoutAccel::ReadoutAccel()
{
}

ReadoutAccel::~ReadoutAccel()
{
}

bool ReadoutAccel::checkI2c(uint8_t addr)
{
    Wire.beginTransmission(addr);
    byte error = Wire.endTransmission();

    if (error == 0)
    {
        Serial.print("Device found at 0x");
        Serial.println(addr, HEX);
        return true;
    }
    else
    {
        Serial.print("No device found at 0x");
        Serial.println(addr, HEX);
        return false;
    }
}

void ReadoutAccel::mpu6050Begin()
{
    if (checkI2c(MPU_addr))
    {
        // Wake up MPU-6050 by writing 0 to PWR_MGMT_1 (0x6B)
        Wire.beginTransmission(MPU_addr);
        Wire.write(0x6B);
        Wire.write(0);
        Wire.endTransmission(true);

        delay(30); // Allow sensor internal clock to stabilize
    }
}

rawdata ReadoutAccel::mpu6050Read(bool debug)
{
    rawdata values = {0, 0, 0};

    // Read registers 0x3B (ACCEL_XOUT_H) through 0x40 (ACCEL_ZOUT_L) - 6 bytes total
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x3B);
    Wire.endTransmission(false);

    uint8_t bytesReceived = Wire.requestFrom(static_cast<uint8_t>(MPU_addr), static_cast<size_t>(6), true);
    if (bytesReceived == 6)
    {
        values.AcX = (int16_t)(Wire.read() << 8 | Wire.read());
        values.AcY = (int16_t)(Wire.read() << 8 | Wire.read());
        values.AcZ = (int16_t)(Wire.read() << 8 | Wire.read());
    }

    if (debug)
    {
        Serial.print("RAW Data: AcX = ");
        Serial.print(values.AcX);
        Serial.print(" | AcY = ");
        Serial.print(values.AcY);
        Serial.print(" | AcZ = ");
        Serial.println(values.AcZ);
    }

    // If reading returned all zeroes (possibly disconnected/reset), reinitialize
    if (values.AcX == 0 && values.AcY == 0 && values.AcZ == 0)
    {
        mpu6050Begin();
    }

    return values;
}

void ReadoutAccel::setMPU6050scales(uint8_t acclScale)
{
    // ACCEL_CONFIG is register 0x1C (bits 4:3 are AFS_SEL: 0=+/-2g, 1=+/-4g, 2=+/-8g, 3=+/-16g)
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x1C);
    Wire.write((acclScale & 0x03) << 3);
    Wire.endTransmission(true);
}

void ReadoutAccel::getMPU6050scales(uint8_t &acclScale)
{
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x1C);
    Wire.endTransmission(false);
    Wire.requestFrom(static_cast<uint8_t>(MPU_addr), static_cast<size_t>(1), true);
    if (Wire.available())
    {
        acclScale = (Wire.read() >> 3) & 0x03;
    }
    else
    {
        acclScale = 0;
    }
}

scaleddata ReadoutAccel::convertRawToScaled(rawdata dataIn, bool debug)
{
    scaleddata values;
    float scaleValue = MPU_ACCL_2_SCALE;
    uint8_t acclScale = 0;

    getMPU6050scales(acclScale);

    switch (acclScale)
    {
    case 0:
        scaleValue = MPU_ACCL_2_SCALE;
        break;
    case 1:
        scaleValue = MPU_ACCL_4_SCALE;
        break;
    case 2:
        scaleValue = MPU_ACCL_8_SCALE;
        break;
    case 3:
        scaleValue = MPU_ACCL_16_SCALE;
        break;
    default:
        scaleValue = MPU_ACCL_2_SCALE;
        break;
    }

    values.AcX = static_cast<float>(dataIn.AcX) / scaleValue;
    values.AcY = static_cast<float>(dataIn.AcY) / scaleValue;
    values.AcZ = static_cast<float>(dataIn.AcZ) / scaleValue;

    if (debug)
    {
        Serial.print("Scaled: AcX = ");
        Serial.print(values.AcX, 4);
        Serial.print(" g | AcY = ");
        Serial.print(values.AcY, 4);
        Serial.print(" g | AcZ = ");
        Serial.print(values.AcZ, 4);
        Serial.println(" g");
    }

    return values;
}
