
#include <Arduino.h>
#include <ESP8266WiFi.h>  // for static ip configuration
// #include <WiFiUdp.h>
#include <SPI.h>          // Serial Peripheral Interface
#include "Wire.h"

#include <NTPClient.h>
#include <ReadoutAccel.h>

ReadoutAccel *Accelerometer = new ReadoutAccel();

WiFiClient wifiClient;


void setup() {
  Wire.begin();
  Serial.begin(9600);
  Accelerometer->mpu6050Begin(Accelerometer->MPU_addr);


}

void loop() {
  rawdata next_sample;
  // Accelerometer->setMPU6050scales(Accelerometer->MPU_addr, 0b00000000, 0b00010000);
  Accelerometer->setMPU6050scales(Accelerometer->MPU_addr, 0b00010000);
  next_sample = Accelerometer->mpu6050Read(Accelerometer->MPU_addr, true);
  Accelerometer->convertRawToScaled(Accelerometer->MPU_addr, next_sample, true);

  delay(2000); // Wait 2 seconds and scan again
}
