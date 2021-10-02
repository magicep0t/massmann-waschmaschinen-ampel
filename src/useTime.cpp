#include "../include/useTime.h"
// #include "../include/configStatic.h"
#include <Arduino.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <stdio.h>

// call Constructor in setup() method for ESP initialization
UseTime::UseTime() : ntpServer("europe.pool.ntp.org"),  // standard europe ntp server
                     utcOffsetInSeconds(7200)   // UTC offset in sekunden // Berlin = UTC+2 -> 3600*2 sekunden
                    //  ntpUDP(WiFiUDP())
{
    const char daysOfTheWeek[7][12] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
    // daysOfTheWeek = new const char*[7];
    // daysOfTheWeek[0] = {"Monday"};
    // daysOfTheWeek[1] = {"Tuesday"};
    // daysOfTheWeek[2] = {"Wednesday"};
    // daysOfTheWeek[3] = {"Thursday"};
    // daysOfTheWeek[4] = {"Friday"};
    // daysOfTheWeek[5] = {"Saturday"};
    // daysOfTheWeek[6] = {"Sunday"};
    ntpUDP = WiFiUDP();
    timeClient = new NTPClient(ntpUDP, ntpServer, utcOffsetInSeconds);

    timeClient->begin();
    printf("Time Client started successfully!");
}

// use Usetime.timeClient.update() in loop() method
// get times with:
//          Usetime.timeClient.getDay()
//          Usetime.timeClient.getHours()
//          Usetime.timeClient.getMinutes()
//          Usetime.timeClient.getSeconds()