#include "MAIN_WaschAmpel.h"

using namespace Ampel_data_config;

ReadoutAccel *Accelerometer = new ReadoutAccel();

WiFiClient wifiClient;


void setup() {
  Wire.begin();
  Serial.begin(9600);
  Accelerometer->mpu6050Begin(Accelerometer->MPU_addr);

  Serial.print("Connect to WiFi...");
  Serial.println();
  Serial.println(conf_ip_this_esp);
  Serial.println(conf_gateway);
  Serial.println(conf_subnet);
  Serial.println(conf_primaryDNS);

  if(!WiFi.config(conf_ip_this_esp, conf_gateway, conf_subnet, conf_primaryDNS, conf_secondaryDNS)){
    Serial.println("WiFi failed to configure...!");
  }
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  // mqttConnection = MqttTransfer(&wifiClient, conf_mqttTopic, conf_mqttUser, conf_mqttPassw);
  // mqttConnection.pubsubclient.setServer(conf_ip_spielwiese, 1883); // Adresse des MQTT-Brokers
  // mqttConnection.pubsubclient.setCallback(mqttConnection.callback, 1883); // callback registration - doesnt work atm
}

void loop() {
  rawdata next_sample;
  // Accelerometer->setMPU6050scales(Accelerometer->MPU_addr, 0b00000000, 0b00010000);
  Accelerometer->setMPU6050scales(Accelerometer->MPU_addr, 0b00000000);
  // Accelerometer->setMPU6050scales(Accelerometer->MPU_addr, 0b00010000);
  next_sample = Accelerometer->mpu6050Read(Accelerometer->MPU_addr, true);
  Accelerometer->convertRawToScaled(Accelerometer->MPU_addr, next_sample, true);

  delay(2000); // Wait 2 seconds and scan again
}
