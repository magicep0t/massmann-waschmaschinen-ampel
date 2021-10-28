#include "MqttTransfer.h"
#include <ArduinoJson.h.>   
// #include "UseTime.h"

String MqttTransfer::CreateJson(scaleddata data)    
{
  StaticJsonDocument<200> doc;
  String Output("");
  // String output = String();
  // output = "{\"scaled\":[" + (String)data.AcX + ", " + (String)data.AcY + ",  " + (String)data.AcZ + "]}";
  // DeserializationError error = deserializeJson(doc, &output[0]);
  // if(error){
  //   Serial.print(F("deserializeJson() failed: "));
  //   Serial.println(error.c_str());
  //   return String("error");
  // }

  // 0 - AcX  //  1 - AcY  //  2 - AcZ
  doc["scaled"][0] = data.AcX;
  doc["scaled"][1] = data.AcY;
  doc["scaled"][2] = data.AcZ;
  // doc["Time"][0] = timeStamp.timeClient.getFormattedTime();
  serializeJson(doc, Output);
  // serializeJsonPretty(doc, Serial);
  return Output;
}
String MqttTransfer::CreateJson(scaleddata data, NTPClient* timeClient)
{
  timeClient->update();
  StaticJsonDocument<200> doc;
  String Output("");
  // 0 - AcX  //  1 - AcY  //  2 - AcZ
  doc["scaled"][0] = data.AcX;
  doc["scaled"][1] = data.AcY;
  doc["scaled"][2] = data.AcZ;
  doc["time"][0] = timeClient->getFormattedTime();
  doc["epochTime"][0] = timeClient->getEpochTime();
  // doc["Time"][0] = timeStamp.timeClient.getFormattedTime();
  serializeJson(doc, Output);
  // serializeJsonPretty(doc, Serial);
  return Output;
}

String MqttTransfer::CreateJson(rawdata data) 
{
  StaticJsonDocument<200> doc;
  String Output("");

  doc["raw"][0] = data.AcX;
  doc["raw"][1] = data.AcY;
  doc["raw"][2] = data.AcZ;
  // doc["Time"][0] = timeStamp.timeClient.getFormattedTime();
  serializeJson(doc, Output);
  // serializeJsonPretty(doc, Serial);
  return Output;
}

MqttTransfer::MqttTransfer(WiFiClient* wificlient, String mqtttopic, String mqttuser, String mqttpasswd, String mqttClientID)
{
  topic = mqtttopic;
  user = mqttuser;
  passw = mqttpasswd;
  ctopic = &mqtttopic[0];
  cuser = &mqttuser[0];
  cpassw = &mqttpasswd[0];
  clientID = mqttClientID;
  
  pubsubclient = PubSubClient(*wificlient);
}

MqttTransfer::MqttTransfer(WiFiClient* wificlient, char* mqtttopic, char* mqttuser, char* mqttpasswd, String mqttClientID)
{
  ctopic = mqtttopic;
  cuser = mqttuser;
  cpassw = mqttpasswd;

  topic = String(mqtttopic);
  user = String(mqttuser);
  passw = String(mqttpasswd);
  clientID = mqttClientID;

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
    if (!pubsubclient.connected()) {
      pubsubclient.connect(clientID.c_str(), cuser, cpassw);
      uint8_t timeout = 8;
      while(timeout && !pubsubclient.connected()){
        timeout--;
        delay(1000);
      }
      if (pubsubclient.connected()){
        Serial.println("Connection successful!");
      } 
      else{
        Serial.print("Fehler, rc=");
        Serial.print(pubsubclient.state());
        Serial.println("");
        Serial.println("Retrying!");
      }
      // pubsubclient.publish("waesche1/test","(Re-)Connected...");
      pubsubclient.subscribe("waesche1/test");    // Topic to subscribe to 
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
