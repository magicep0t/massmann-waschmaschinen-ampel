#ifndef USETIME_H
#define USETIME_H

#include <Arduino.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

class UseTime
{
public:
    UseTime();
    ~UseTime();

    void begin();
    void update();

    const char* ntpServer;
    const long utcOffsetInSeconds;
    WiFiUDP ntpUDP;
    NTPClient* timeClient;
};

#endif // USETIME_H
