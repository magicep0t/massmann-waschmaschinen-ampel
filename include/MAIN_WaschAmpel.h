#ifndef MAIN_WASCHAMPEL_H
#define MAIN_WASCHAMPEL_H

#include <Arduino.h>
#include <ESP8266WiFi.h>  // for static ip configuration
// #include <WiFiUdp.h>
#include <SPI.h>          // Serial Peripheral Interface
#include "Wire.h"

#include <NTPClient.h>
#include <ReadoutAccel.h>
#include <configStatic.h>
#include <MqttTransfer.h>

void setup();
void loop();

MqttTransfer mqttConnection;    // creates mqtt object, requires WiFi connection, topic, MQTTusername and MQTTpassword

#endif // MAIN_WASCHAMPEL_H