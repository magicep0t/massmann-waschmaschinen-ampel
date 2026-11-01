#include "mqttTransfer.h"

MqttTransfer::MqttTransfer(WiFiClient* wifiClient, const String& mqttTopic, const String& mqttUser, const String& mqttPasswd, const String& mqttClientID)
    : topic(mqttTopic),
      user(mqttUser),
      passw(mqttPasswd),
      clientID(mqttClientID),
      pubsubclient(*wifiClient)
{
}

MqttTransfer::~MqttTransfer()
{
}

String MqttTransfer::CreateJson(rawdata data)
{
    StaticJsonDocument<200> doc;
    String output;

    doc["raw"][0] = data.AcX;
    doc["raw"][1] = data.AcY;
    doc["raw"][2] = data.AcZ;

    serializeJson(doc, output);
    return output;
}

String MqttTransfer::CreateJson(scaleddata data)
{
    StaticJsonDocument<200> doc;
    String output;

    doc["scaled"][0] = data.AcX;
    doc["scaled"][1] = data.AcY;
    doc["scaled"][2] = data.AcZ;

    serializeJson(doc, output);
    return output;
}

String MqttTransfer::CreateJson(scaleddata data, NTPClient* timeClient)
{
    if (timeClient)
    {
        timeClient->update();
    }
    StaticJsonDocument<256> doc;
    String output;

    doc["scaled"][0] = data.AcX;
    doc["scaled"][1] = data.AcY;
    doc["scaled"][2] = data.AcZ;
    if (timeClient)
    {
        doc["time"] = timeClient->getFormattedTime();
        doc["epochTime"] = timeClient->getEpochTime();
    }

    serializeJson(doc, output);
    return output;
}

void MqttTransfer::callback(char* mqttTopic, byte* payload, unsigned int length)
{
    Serial.print("Nachricht eingetroffen [");
    Serial.print(mqttTopic);
    Serial.print("]: ");
    for (unsigned int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
    }
    Serial.println();
}

void MqttTransfer::reconnect()
{
    while (!pubsubclient.connected())
    {
        Serial.print("Verbinde mit MQTT Broker...");

        bool connected = false;
        if (user.length() > 0)
        {
            connected = pubsubclient.connect(clientID.c_str(), user.c_str(), passw.c_str());
        }
        else
        {
            connected = pubsubclient.connect(clientID.c_str());
        }

        if (connected)
        {
            Serial.println(" Erfolgreich verbunden!");
            pubsubclient.subscribe(topic.c_str());
        }
        else
        {
            Serial.print(" Fehlgeschlagen, rc=");
            Serial.print(pubsubclient.state());
            Serial.println(". Neuer Versuch in 2 Sekunden...");
            delay(2000);
        }
    }
}

void MqttTransfer::publish(const String& mqttTopic, const String& message)
{
    pubsubclient.publish(mqttTopic.c_str(), message.c_str());
}
