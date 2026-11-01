# WaschAmpel ESP8266 & MPU-6050

Dieses Projekt dient der Überwachung des Betriebszustands einer Waschmaschine mithilfe eines **ESP8266 (NodeMCU v2)** und eines **MPU-6050** 3-Achsen-Beschleunigungssensors.
Die erfassten Beschleunigungswerte werden in Echtzeit als JSON formatiert, mit einem NTP-Zeitstempel versehen und über **MQTT** an einen Broker gesendet.

---

## Inhaltsverzeichnis
1. [Hardware-Übersicht & Verkabelung](#hardware-übersicht--verkabelung)
2. [Projektstruktur](#projektstruktur)
3. [Konfiguration](#konfiguration)
4. [Kompilierung & Flashen mit PlatformIO](#kompilierung--flashen-mit-platformio)
5. [Flashen mit einfachsten Mitteln (`esptool`)](#flashen-mit-einfachsten-mitteln-esptool)
6. [MQTT-Datenformat](#mqtt-datenformat)
7. [Fehlerbehebung (Troubleshooting)](#fehlerbehebung-troubleshooting)

---

## Hardware-Übersicht & Verkabelung

### Benötigte Komponenten
- **ESP8266 Board** (z. B. NodeMCU v2 / ESP-12E / D1 Mini)
- **MPU-6050 Sensor-Modul** (GY-521 Breakout)
- **4x Jumperkabel** (Female-to-Female oder Dupont-Kabel)
- **Micro-USB-Kabel** zur Stromversorgung und Programmierung

### Pinbelegung / Schaltplan (I2C)

Das MPU-6050 Modul kommuniziert über den **I2C-Bus**. Die Standard-I2C-Pins des ESP8266 (NodeMCU v2) sind:
- **SDA:** Pin `D2` (GPIO 4)
- **SCL:** Pin `D1` (GPIO 5)

| MPU-6050 (GY-521) Pin | ESP8266 (NodeMCU v2) Pin | Beschreibung |
|-----------------------|--------------------------|--------------|
| **VCC**               | **3V3** (oder 3.3V)      | Stromversorgung (GY-521 hat i. d. R. einen 3.3V-Spannungsregler; 3.3V empfohlen) |
| **GND**               | **GND**                  | Masseverbindung |
| **SCL**               | **D1** (GPIO 5)          | I2C Clock-Leitung |
| **SDA**               | **D2** (GPIO 4)          | I2C Daten-Leitung |
| **AD0**               | **GND** (optional)       | Setzt I2C-Adresse auf `0x68`. Wenn unverbunden, zieht ein interner Pulldown AD0 meist auf GND. |
| **INT**               | *nicht verbunden*        | Für dieses Projekt nicht benötigt |
| **XDA / XCL**         | *nicht verbunden*        | Nicht benötigt (Hilfs-I2C für Magnetometer) |

> **Hinweis zur I2C-Adresse:**
> Liegt `AD0` auf GND, lautet die I2C-Adresse `0x68` (Standard im Code).
> Liegt `AD0` auf 3.3V, lautet die I2C-Adresse `0x69`.

---

## Projektstruktur

```text
.
├── include/
│   ├── configStatic.h.example  # Vorlage für WiFi- & MQTT-Konfiguration
│   ├── configStatic.h          # Lokale Konfiguration (in .gitignore)
│   ├── MAIN_WaschAmpel.h       # Haupt-Header
│   ├── mqttTransfer.h          # MQTT-Verwaltung & JSON-Serialisierung
│   ├── readoutAccel.h          # MPU-6050 Auslesung & Skalierung
│   └── useTime.h               # NTP-Zeitsynchronisation
├── src/
│   ├── MAIN_WaschAmpel.cpp     # setup() und loop(), Ablaufsteuerung
│   ├── mqttTransfer.cpp        # MQTT-Verbindung & Publishing
│   ├── readoutAccel.cpp        # I2C-Kommunikation mit MPU-6050
│   └── useTime.cpp             # NTPClient Initialisierung
├── platformio.ini              # PlatformIO-Projektkonfiguration
└── README.md                   # Dieses Dokument
```

---

## Konfiguration

1. Erstelle die Konfigurationsdatei aus der Vorlage:
   ```bash
   cp include/configStatic.h.example include/configStatic.h
   ```

2. Passe die Einstellungen in `include/configStatic.h` an deine Umgebung an:
   ```cpp
   namespace Ampel_data_config {
       // WiFi
       const char conf_ssid[] = "DEIN_WLAN_NAME";
       const char conf_password[] = "DEIN_WLAN_PASSWORT";

       // IP-Konfiguration: false fuer DHCP (empfohlen), true fuer statische IP
       const bool conf_use_static_ip = false;

       // Falls statische IP gewuenscht:
       const IPAddress conf_ip_this_esp(192, 168, 178, 50);
       const IPAddress conf_gateway(192, 168, 178, 1);
       const IPAddress conf_subnet(255, 255, 255, 0);
       const IPAddress conf_primaryDNS(192, 168, 178, 1);
       const IPAddress conf_secondaryDNS(8, 8, 8, 8);

       // MQTT-Broker
       const IPAddress conf_ip_spielwiese(192, 168, 178, 100);
       const uint16_t conf_mqtt_port = 1883;
       const char conf_mqttTopic[] = "waesche1/test";
       const char conf_mqttUser[] = "";        // Leer lassen, falls keine Authentifizierung
       const char conf_mqttPassw[] = "";       // Leer lassen, falls keine Authentifizierung
       const char conf_mqttClientID[] = "ESP8266_WaschAmpel";
   }
   ```

---

## Kompilierung & Flashen mit PlatformIO

PlatformIO lädt alle Bibliotheken und Toolchains automatisch herunter.

### 1. Firmware kompilieren
```bash
pio run
```
Die erzeugte Binary liegt anschließend unter `.pio/build/nodemcuv2/firmware.bin`.

### 2. Flashen über USB
Schließe das NodeMCU-Board per USB an und führe folgenden Befehl aus:
```bash
pio run -t upload
```

### 3. Seriellen Monitor öffnen
```bash
pio device monitor -b 115200
```

---

## Flashen mit einfachsten Mitteln (`esptool`)

Falls du auf einem Zielrechner kein PlatformIO installieren möchtest, kannst du das offizielle Python-Tool **`esptool`** verwenden, um die bereits kompilierte Binärdatei direkt auf den ESP zu flashen.

### 1. `esptool` installieren
Alles was benötigt wird, ist Python 3 und pip:
```bash
pip install esptool
```

### 2. Port ermitteln
- **Linux:** `/dev/ttyUSB0` oder `/dev/ttyACM0`
  *(Prüfen mit `ls /dev/ttyUSB*` oder `dmesg | grep tty`)*
- **macOS:** `/dev/cu.usbserial-*` oder `/dev/cu.wchusbserial*`
- **Windows:** `COM3`, `COM4`, etc. *(Im Geräte-Manager unter "Anschlüsse (COM & LPT)" nachsehen)*

### 3. ESP8266 Flash löschen (empfohlen vor Erstflash)
```bash
esptool --port /dev/ttyUSB0 erase_flash
```

### 4. Firmware direkt flashen
Flashe die erzeugte `firmware.bin` an die Startadresse `0x00000`:
```bash
esptool --chip esp8266 --port /dev/ttyUSB0 --baud 460800 write_flash 0x00000 .pio/build/nodemcuv2/firmware.bin
```

*Hinweis: Wenn das Flashen fehlschlägt, reduziere die Baudrate auf `--baud 115200`.*

### 5. Serieller Monitor über Terminal (ohne PlatformIO)
Du kannst jeden beliebigen seriellen Monitor mit **115200 Baud** nutzen, z. B.:
```bash
python3 -m serial.tools.miniterm /dev/ttyUSB0 115200
```
(Beenden mit `Ctrl + ]`)

---

## MQTT-Datenformat

Die Messwerte werden im JSON-Format auf das konfigurierte Topic (Standard: `waesche1/test`) übertragen:

```json
{
  "scaled": [
    0.0122,
    -0.0345,
    0.9856
  ],
  "time": "14:23:05",
  "epochTime": 1715091785
}
```

- `scaled[0]`: Beschleunigung X-Achse in **g**
- `scaled[1]`: Beschleunigung Y-Achse in **g**
- `scaled[2]`: Beschleunigung Z-Achse in **g** (~ 1.0g im Ruhezustand bei horizontaler Montage)
- `time`: Formatierte Uhrzeit (HH:MM:SS) über NTP
- `epochTime`: Unix-Timestamp in Sekunden

---

## Fehlerbehebung (Troubleshooting)

1. **"No device found at 0x68" im Seriellen Monitor:**
   - Verkabelung zwischen NodeMCU und MPU-6050 überprüfen (D1 -> SCL, D2 -> SDA, 3V3 -> VCC, GND -> GND).
   - Prüfen, ob der Sensor Strom hat (LED auf dem GY-521 Modul leuchtet).
   - Falls AD0 an 3.3V anliegt, hat der Sensor Adresse `0x69`.

2. **ESP8266 verbindet sich nicht mit dem WLAN:**
   - SSID und Passwort in `include/configStatic.h` überprüfen.
   - Der ESP8266 unterstützt **nur 2,4 GHz WLAN**, kein 5 GHz WLAN!

3. **MQTT Verbindung schlägt fehl (`rc = -2`):**
   - IP-Adresse des Brokers und Port (1883) überprüfen.
   - Prüfen, ob der Broker erreichbar ist (z. B. `ping <IP-des-Brokers>`).
   - Prüfen, ob Benutzername/Passwort benötigt werden.

4. **Kompilierfehler bzgl. `configStatic.h`:**
   - Kopiere die Beispieldatei: `cp include/configStatic.h.example include/configStatic.h`.
