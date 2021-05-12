#include "../include/useTime.h"

UseTime::UseTime()
{
    const char *ntpServer = "europe.pool.ntp.org";
    // UTC offset in sekunden // Berlin = UTC+2 -> 3600*2 sekunden
    const long utcOffsetInSeconds = 7200;
    ntpUDP = WiFiUDP();
    timeClient = NTPClient(ntpUDP, ntpServer, utcOffsetInSeconds);
}