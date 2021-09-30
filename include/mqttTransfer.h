/*
* Class 'mqttTransfer'; basic data transfer functionality
* to send data to the server with the mqtt protocol
*
*/

#ifndef MQTTTRANSFER_H
#define MQTTTRANSFER_H

#include <PubSubClient.h> // MQTT Bibliothek
#include <ESP8266WiFi.h>  // for static ip configuration
#include <string.h>

// ************* used variables *********************

// const String ssid     
// const String password 

// byte mac_this_esp[]
// IPAddress ip_this_esp
// IPAddress subnet
// IPAddress gateway
// IPAddress primaryDNS
// IPAddress secondaryDNS

// byte mac_andrea[] 
// IPAddress ip_spielwiese

// MQTT configuration
// const String mqttUser 
// const String mqttPassw

class mqttTransfer{
public:
    // mqttTransfer();
    mqttTransfer(WiFiClient* wifiClient, String mqtttopic, String mqttuser, String mqttpasswd);
    mqttTransfer(WiFiClient* wifiClient, char* mqtttopic, char* mqttuser, char* mqttpasswd);
    ~mqttTransfer();

    void callback(String mqtttopic, byte* payload, unsigned int length);
    void callback(char* mqtttopic, byte* payload, unsigned int length);
    void reconnect();

private:
    // const String wlanSsid;
    // const String wlanPassword;

    // const byte mac_this_esp;
    // const IPAddress ip_this_esp;
    // const IPAddress subnet;
    // const IPAddress gateway;
    // const IPAddress primaryDNS;
    // const IPAddress secondaryDNS;

    // conversion to char* with referencing to the first string element
    // and the other way round with the constructor String(char*)
    // all this hustle just coz I dislike char* for most practical applications
    String topic;
    char* ctopic;
    String user; 
    char* cuser; 
    String passw;
    char* cpassw;
    PubSubClient pubsubclient;
};

#endif