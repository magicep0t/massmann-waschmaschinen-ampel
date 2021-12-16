#include "ReadoutAccel.h"
#include <iostream>

int errorCounter = 1;
#define DEBUG std::cout << "KEIN Error,goto.." << errorCounter++ << "..Zeile.."<< __LINE__ << std::endl;

//###################################################################################################################
// "In Arduino, byte, uint8_t and unsigned short can be used interchangeably because they are literally the same type.  It’s just an alias."
//###################################################################################################################

ReadoutAccel::ReadoutAccel()
{
}

ReadoutAccel::~ReadoutAccel()
{
}

bool ReadoutAccel::checkI2c(byte addr)
{
    // We are using the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Serial.println(" ");
    Wire.beginTransmission(addr);

    if (Wire.endTransmission() == 0)
    {
        Serial.print(" Device Found at 0x");
        Serial.println(addr, HEX);
        return true;
    }
    else
    {
        Serial.print(" No Device Found at 0x");
        Serial.println(addr, HEX);
        return false;
    }
}

void ReadoutAccel::mpu6050Begin()
{
    // This function initializes the MPU-6050 IMU Sensor
    // It verifys the address is correct and wakes up the
    // MPU.
    if (checkI2c(MPU_addr))
    {
        Wire.beginTransmission(MPU_addr);
        Wire.write(0x6B); // PWR_MGMT_1 register
        Wire.write(0);    // set to zero (wakes up the MPU-6050)
        Wire.endTransmission(true);

        delay(30); // Ensure gyro has enough time to power up
    }
}

rawdata ReadoutAccel::mpu6050Read(bool Debug)
{
    // This function reads the raw 16-bit data values from
    // the MPU-6050

    rawdata values;

    Wire.beginTransmission(MPU_addr);
    Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_addr, static_cast<uint8_t>(6), static_cast<uint8_t>(1));             // request a total of 14 registers
    values.AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
    values.AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
    values.AcZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)

    if (Debug)
    {
        Serial.println("RAW Data:");

        Serial.print("AcX = ");
        Serial.print(values.AcX);
        Serial.print(" | AcY = ");
        Serial.print(values.AcY);
        Serial.print(" | AcZ = ");
        Serial.println(values.AcZ);
    }
    if(values.AcX == 0 && values.AcY == 0 && values.AcZ == 0 )
    {
        mpu6050Begin();
    }
    return values;
}

void ReadoutAccel::setMPU6050scales(uint8_t Accl)
{
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x1B); // write to register starting at 0x1B
    Wire.write(Accl); // Self Tests Off and set Accl FS to 8g
    Wire.endTransmission(true);
}

void ReadoutAccel::getMPU6050scales(uint8_t &Accl)
{
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x1B); // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_addr, static_cast<uint8_t>(1), static_cast<uint8_t>(1)); // request a total of 14 registers
    Accl = (Wire.read() & (bit(3) | bit(4))) >> 3;
}

scaleddata ReadoutAccel::convertRawToScaled(rawdata data_in, bool Debug)
{

    scaleddata values;
    float scale_value = 0.0;
    uint8_t Accl;

    getMPU6050scales(Accl);

    scale_value = 0.0;
    if (Debug)
    {
        //Serial.print("Accl Full-Scale = ");
    }
    switch (Accl)
    {
    case 0:
        scale_value = MPU_ACCL_2_SCALE;
        break;
    case 1:
        scale_value = MPU_ACCL_4_SCALE;
        break;
    case 2:
        scale_value = MPU_ACCL_8_SCALE;
        break;
    case 3:
        scale_value = MPU_ACCL_16_SCALE;
        break;
    default:
        break;
    }
    values.AcX = (float)data_in.AcX / scale_value;
    values.AcY = (float)data_in.AcY / scale_value;
    values.AcZ = (float)data_in.AcZ / scale_value;

    if (Debug)
    {
        Serial.println("Scaled:");
        Serial.print("AcX = ");
        Serial.print(values.AcX);
        Serial.print(" g| AcY = ");
        Serial.print(values.AcY);
        Serial.print(" g| AcZ = ");
        Serial.print(values.AcZ);
        Serial.println(" g");
        Serial.println("");
    }
DEBUG
    return values;
}
