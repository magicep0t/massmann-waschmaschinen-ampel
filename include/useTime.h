// This file is used to set correct time stamps for the transmitted data to the server

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
    private:
        const char* ntpServer;
        const long utcOffsetInSeconds;
        WiFiUDP ntpUDP;
        // NTPClient timeClient;    no idea why this is not allowed to be declared here. ..
        const char** daysOfTheWeek;
};
#endif