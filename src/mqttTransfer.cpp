#include "mqttTransfer.h"

mqttTransfer::mqttTransfer(WiFiClient* wificlient, String mqtttopic, String mqttuser, String mqttpasswd)
{
    topic = mqtttopic;
    user = mqttuser;
    passw = mqttpasswd;

    ctopic = &mqtttopic[0];
    cuser = &mqttuser[0];
    cpassw = &mqttpasswd[0];

    pubsubclient = PubSubClient(*wificlient);
}

mqttTransfer::mqttTransfer(WiFiClient* wificlient, char* mqtttopic, char* mqttuser, char* mqttpasswd)
{
    ctopic = mqtttopic;
    cuser = mqttuser;
    cpassw = mqttpasswd;

    topic = String(mqtttopic);
    user = String(mqttuser);
    passw = String(mqttpasswd);

    pubsubclient = PubSubClient(*wificlient);
}

mqttTransfer::~mqttTransfer()
{
}

// generic callback to register at PubSubClient.setCallback(void func*(String, uint8_t*, unsigned int))
void mqttTransfer::callback(char* mqtttopic, byte* payload, unsigned int length) 
{
  Serial.print("Nachricht eingetroffen [");
  Serial.print(mqtttopic);
  Serial.print("]: ");
  for (unsigned int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void mqttTransfer::reconnect() {
  // Solange wiederholen bis Verbindung wiederhergestellt ist
  while (!pubsubclient.connected()) {
    Serial.print("Versuch des MQTT Verbindungsaufbaus...");
    // Serial.println("");
    //Verbindungsversuch:
    // if (client.connect("arduinoClient", mqttUser, mqttPassw)) {
    if (pubsubclient.connect("Erste_Waschmaschine", cuser, cpassw)) {
      Serial.println("Erfolgreich verbunden!");
      // Nun versendet der Arduino eine Nachricht in outTopic ...
      pubsubclient.publish("waesche1/test","ESP8266 nach Hause telefonieren");
      // und meldet sich bei inTopic für eingehende Nachrichten an:
      pubsubclient.subscribe("waesche1/test");
    } else { // Im Fehlerfall => Fehlermeldung und neuer Versuch
      Serial.print("Fehler, rc=");
      Serial.print(pubsubclient.state());
      Serial.println("Nächster Versuch in 5 Sekunden");
      // 5 Sekunden Pause vor dem nächsten Versuch
      delay(5000);
    }
  }
}