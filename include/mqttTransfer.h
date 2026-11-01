#ifndef MQTTTRANSFER_H
#define MQTTTRANSFER_H

#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include "readoutAccel.h"

class MqttTransfer
{
private:
    String topic;
    String user;
    String passw;
    String clientID;

public:
    PubSubClient pubsubclient;

    MqttTransfer() = default;
    MqttTransfer(WiFiClient* wifiClient, const String& mqttTopic, const String& mqttUser, const String& mqttPasswd, const String& mqttClientID);
    ~MqttTransfer();

    void callback(char* mqttTopic, byte* payload, unsigned int length);
    void reconnect();
    void publish(const String& mqttTopic, const String& message);

    String CreateJson(rawdata data);
    String CreateJson(scaleddata data);
    String CreateJson(scaleddata data, NTPClient* timeClient);
};

#endif // MQTTTRANSFER_H
