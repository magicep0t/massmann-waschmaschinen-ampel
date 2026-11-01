#ifndef MAIN_WASCHAMPEL_H
#define MAIN_WASCHAMPEL_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SPI.h>
#include <Wire.h>

#include <NTPClient.h>
#include "readoutAccel.h"
#include "configStatic.h"
#include "mqttTransfer.h"
#include "useTime.h"

void setup();
void loop();

extern MqttTransfer* mqttConnection;

#endif // MAIN_WASCHAMPEL_H
