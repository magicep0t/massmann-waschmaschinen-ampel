#include "MqttTransfer.h"

MqttTransfer::MqttTransfer(WiFiClient* wificlient, String mqtttopic, String mqttuser, String mqttpasswd)
{
    topic = mqtttopic;
    user = mqttuser;
    passw = mqttpasswd;

    ctopic = &mqtttopic[0];
    cuser = &mqttuser[0];
    cpassw = &mqttpasswd[0];

    pubsubclient = PubSubClient(*wificlient);
}

MqttTransfer::MqttTransfer(WiFiClient* wificlient, char* mqtttopic, char* mqttuser, char* mqttpasswd)
{
    ctopic = mqtttopic;
    cuser = mqttuser;
    cpassw = mqttpasswd;

    topic = String(mqtttopic);
    user = String(mqttuser);
    passw = String(mqttpasswd);

    pubsubclient = PubSubClient(*wificlient);
}

MqttTransfer::~MqttTransfer()
{
}

// generic callback to register at PubSubClient.setCallback(void func*(String, uint8_t*, unsigned int))
void MqttTransfer::callback(char* mqtttopic, byte* payload, unsigned int length) 
{
  Serial.print("Nachricht eingetroffen [");
  Serial.print(mqtttopic);
  Serial.print("]: ");
  for (unsigned int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void MqttTransfer::reconnect() 
{
  while (!pubsubclient.connected()) {
    Serial.print("Trying to connect to MQTT broker...");
    Serial.println("");
    if (pubsubclient.connect("esp8266_machine_1", cuser, cpassw)) {
      Serial.println("Connection successful!");
      pubsubclient.publish("waesche1/test","(Re-)Connected...");
      pubsubclient.subscribe("waesche1/test");
    } else { // error -> retry in 5 sec
      Serial.print("Fehler, rc=");
      Serial.print(pubsubclient.state());
      Serial.println("");
      Serial.println("Retry connecting in 5 sec");
      delay(5000);
    }
  }
}
void MqttTransfer::publish(char* mqtttopic, char* message)
{
  pubsubclient.publish(mqtttopic, message);
}
void MqttTransfer::publish(String mqtttopic, String message)
{
  pubsubclient.publish(&mqtttopic[0], &message[0]);
}
