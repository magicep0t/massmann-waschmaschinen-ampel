#include "useTime.h"

UseTime::UseTime()
    : ntpServer("pool.ntp.org"),
      utcOffsetInSeconds(7200), // Berlin UTC+2 (bzw. 3600 fuer UTC+1)
      ntpUDP(),
      timeClient(nullptr)
{
    timeClient = new NTPClient(ntpUDP, ntpServer, utcOffsetInSeconds);
}

UseTime::~UseTime()
{
    if (timeClient)
    {
        delete timeClient;
        timeClient = nullptr;
    }
}

void UseTime::begin()
{
    if (timeClient)
    {
        timeClient->begin();
        Serial.println("NTP-Client initialisiert.");
    }
}

void UseTime::update()
{
    if (timeClient)
    {
        timeClient->update();
    }
}
