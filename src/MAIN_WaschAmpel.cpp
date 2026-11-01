#include "MAIN_WaschAmpel.h"

#define DEBUG true

using namespace Ampel_data_config;

ReadoutAccel *Accelerometer = nullptr;
UseTime theTime;
WiFiClient wifiClient;
MqttTransfer *mqttConnection = nullptr;

void connectWiFi()
{
    Serial.print("Verbinde mit WiFi: ");
    Serial.println(conf_ssid);

    if (conf_use_static_ip)
    {
        if (!WiFi.config(conf_ip_this_esp, conf_gateway, conf_subnet, conf_primaryDNS, conf_secondaryDNS))
        {
            Serial.println("Warnung: Statische IP-Konfiguration fehlgeschlagen! Versuche DHCP...");
        }
    }

    WiFi.mode(WIFI_STA);
    WiFi.begin(conf_ssid, conf_password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.print("WiFi verbunden! Lokale IP: ");
    Serial.println(WiFi.localIP());
}

void setup()
{
    Serial.begin(115200);
    delay(200);
    Serial.println();
    Serial.println("=== WaschAmpel ESP8266 Start ===");

    // I2C Initialisierung (NodeMCU: D2=SDA=GPIO4, D1=SCL=GPIO5)
    Wire.begin(D2, D1);

    Accelerometer = new ReadoutAccel();
    Accelerometer->mpu6050Begin();
    Accelerometer->setMPU6050scales(0); // 0 = +/- 2g

    connectWiFi();

    // NTP-Zeit initialisieren
    theTime.begin();

    // MQTT initialisieren
    mqttConnection = new MqttTransfer(&wifiClient, conf_mqttTopic, conf_mqttUser, conf_mqttPassw, conf_mqttClientID);
    mqttConnection->pubsubclient.setServer(conf_ip_spielwiese, conf_mqtt_port);
}

void loop()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        connectWiFi();
    }

    if (!mqttConnection->pubsubclient.connected())
    {
        mqttConnection->reconnect();
    }

    mqttConnection->pubsubclient.loop();

    // Messwerte vom Beschleunigungssensor auslesen
    rawdata rawSample = Accelerometer->mpu6050Read(false);
    scaleddata scaledSample = Accelerometer->convertRawToScaled(rawSample, DEBUG);

    // JSON Payload generieren
    String payloadRaw = mqttConnection->CreateJson(rawSample);
    String payloadScaled = mqttConnection->CreateJson(scaledSample, theTime.timeClient);

    if (DEBUG)
    {
        Serial.print("Payload (Rohdaten): ");
        Serial.println(payloadRaw);
        Serial.print("Payload (Skaliert): ");
        Serial.println(payloadScaled);
    }

    // MQTT nachricht publishen
    mqttConnection->publish(conf_mqttTopic, payloadScaled);

    delay(500); // 500ms Abtastintervall
}
