<<<<<<< HEAD
#include "../include/useTime.h"
#include "../include/configStatic.h"
#include <Arduino.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

UseTime::UseTime() : ntpServer("europe.pool.ntp.org"),
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
    NTPClient timeClient(ntpUDP, ntpServer, utcOffsetInSeconds);
=======
/*  
 * Class 'UseTime': provides time synchronisation
 * functionality for the ESP
 */

#include "../include/useTime.h"

UseTime::UseTime()
{
    const char *ntpServer = "europe.pool.ntp.org";
    // UTC offset in sekunden // Berlin = UTC+2 -> 3600*2 sekunden
    const long utcOffsetInSeconds = 7200;
    ntpUDP = WiFiUDP();
    timeClient = NTPClient(ntpUDP, ntpServer, utcOffsetInSeconds);
>>>>>>> 0b15dfd (first commit)
}