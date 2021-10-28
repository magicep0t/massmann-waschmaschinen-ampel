#include "MAIN_WaschAmpel.h"

#define DEBUG true

using namespace Ampel_data_config;

ReadoutAccel *Accelerometer = new ReadoutAccel();

UseTime theTime;

WiFiClient wifiClient;

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  Accelerometer->mpu6050Begin();

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
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Connected to Wifi! ");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  mqttConnection = new MqttTransfer(&wifiClient, conf_mqttTopic, conf_mqttUser, conf_mqttPassw, conf_mqttClientID);
  mqttConnection->pubsubclient.setServer(conf_ip_spielwiese, 1883); // Adresse des MQTT-Brokers
  // mqttConnection.pubsubclient.setCallback(mqttConnection.callback, 1883); // callback registration - doesnt work atm
}

void loop()
{
  rawdata next_sample;
  Accelerometer->setMPU6050scales(0b00000000);
  // Accelerometer->setMPU6050scales(0b00010000);
  next_sample = Accelerometer->mpu6050Read(false);
  scaleddata scaled_sample = Accelerometer->convertRawToScaled(next_sample, DEBUG);
  
  while(!mqttConnection->pubsubclient.connected()){
    if(WiFi.status() != WL_CONNECTED){
      while (WiFi.status() != WL_CONNECTED){
        WiFi.begin(conf_ssid, conf_password);
        uint8_t timeout = 8;
        while (timeout && (WiFi.status() != WL_CONNECTED)) {
          timeout--;
          delay(1000);
        }
      }
    }
    mqttConnection->reconnect();
  }

  String data = mqttConnection->CreateJson(next_sample);
  String data_scaled = mqttConnection->CreateJson(scaled_sample, theTime.timeClient);
  Serial.println(data);
  Serial.println(data_scaled);

  String topic = "waesche1/test";
  mqttConnection->publish(topic, data);
  mqttConnection->publish(topic, data_scaled);

  mqttConnection->pubsubclient.loop();

  delay(5000); // Wait 2 seconds and scan again
}
