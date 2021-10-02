#include "MAIN_WaschAmpel.h"

ReadoutAccel *Accelerometer = new ReadoutAccel();

WiFiClient wifiClient;


void setup() {
  Wire.begin();
  Serial.begin(9600);
  Accelerometer->mpu6050Begin(Accelerometer->MPU_addr);

  Serial.print("Connect to WiFi...");

  // if (!WiFi.config(Ampel_data_config::conf_ip_this_esp, Ampel_data_config::conf_gateway, Ampel_data_config::conf_subnet, Ampel_data_config::conf_primaryDNS, Ampel_data_config::conf_secondaryDNS))
  // {
  //   Serial.println("WiFi failed to configure...!");
  // }
}

void loop() {
  rawdata next_sample;
  // Accelerometer->setMPU6050scales(Accelerometer->MPU_addr, 0b00000000, 0b00010000);
  Accelerometer->setMPU6050scales(Accelerometer->MPU_addr, 0b00010000);
  next_sample = Accelerometer->mpu6050Read(Accelerometer->MPU_addr, true);
  Accelerometer->convertRawToScaled(Accelerometer->MPU_addr, next_sample, true);

  delay(2000); // Wait 2 seconds and scan again
}
