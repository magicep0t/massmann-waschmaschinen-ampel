/*
* Class 'mqttTransfer'; basic data transfer functionality
* to send data to the server with the mqtt protocol
*
*/

#ifndef MQTTTRANSFER_H
#define MQTTTRANSFER_H

#include <PubSubClient.h> // MQTT Bibliothek

// ************* used variables *********************

// const char* ssid     
// const char* password 

// byte mac_this_esp[]
// IPAddress ip_this_esp
// IPAddress subnet
// IPAddress gateway
// IPAddress primaryDNS
// IPAddress secondaryDNS

// byte mac_andrea[] 
// IPAddress ip_spielwiese

// MQTT configuration
// const char* mqttUser 
// const char* mqttPassw

class mqttTransfer{
public:
    mqttTransfer(); // initialize local variables with values from configStatic.h

private:
    const char* ssid;
    const char* password;

    const byte mac_this_esp;
    const IPAddress ip_this_esp;
    const IPAddress subnet;
    const IPAddress gateway;
    const IPAddress primaryDNS;
    const IPAddress secondaryDNS;

    const char* mqttUser; 
    const char* mqttPassw;
}

#endif