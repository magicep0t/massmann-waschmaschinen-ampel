#ifndef USETIME_H
#define USETIME_H

// #include <time.h>
#include <Arduino.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

class UseTime{
    public:
        UseTime();
        
        const char* ntpServer;
        const long utcOffsetInSeconds;
        WiFiUDP ntpUDP;
        NTPClient timeClient;
};
#endif