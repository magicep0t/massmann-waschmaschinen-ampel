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
  WiFi.begin(conf_ssid, conf_password);
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  mqttConnection = new MqttTransfer(&wifiClient, conf_mqttTopic, conf_mqttUser, conf_mqttPassw);
  mqttConnection->pubsubclient.setServer(conf_ip_spielwiese, 1883); // Adresse des MQTT-Brokers
  // mqttConnection.pubsubclient.setCallback(mqttConnection.callback, 1883); // callback registration - doesnt work atm
}

void loop() {
  rawdata next_sample;
  // Accelerometer->setMPU6050scales(Accelerometer->MPU_addr, 0b00000000, 0b00010000);
  Accelerometer->setMPU6050scales(Accelerometer->MPU_addr, 0b00000000);
  // Accelerometer->setMPU6050scales(Accelerometer->MPU_addr, 0b00010000);
  next_sample = Accelerometer->mpu6050Read(Accelerometer->MPU_addr, true);
  scaleddata scaled_sample = Accelerometer->convertRawToScaled(Accelerometer->MPU_addr, next_sample, true);

  if (!mqttConnection->pubsubclient.connected()){
    mqttConnection->reconnect();
  }
  String data = String(next_sample.AcX);
  data += " ";
  data += next_sample.AcY;
  data += " ";
  data += next_sample.AcZ;

  String data_scaled = String(scaled_sample.AcX);
  data_scaled += " ";
  data_scaled += next_sample.AcY;
  data_scaled += " ";
  data_scaled += next_sample.AcZ;

  String topic = "waesche1/test";
  mqttConnection->publish(topic, data);
  mqttConnection->publish(topic, data_scaled);

  mqttConnection->pubsubclient.loop();

  delay(5000); // Wait 2 seconds and scan again
}
