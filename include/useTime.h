/*  
 * Class 'UseTime': provides time synchronisation
 * functionality for the ESP, i.e. to set correct
 * time stamps for the transmitted data to the server
 */

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
        NTPClient *timeClient;    
        const char** daysOfTheWeek;
};


#endif