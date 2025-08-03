# ESP32S3 Relay via Loxone

Dit project maakt gebruik van een Seeed Studio XIAO ESP32S3 en een 1-channel relay expansion.  
Het relais is te schakelen via HTTP (geschikt voor Loxone en Postman) **en via MQTT**. De onboard LED knippert elke 2 seconden.

## WiFi-configuratie

Om je WiFi-gegevens veilig te houden, worden deze **niet** in de broncode opgeslagen.  
Maak zelf een bestand aan met de naam `wifi_config.h` in de map `/include` van het project.

### Stappen

1. Maak het bestand aan:  
   ```
   /include/wifi_config.h
   ```

2. Voeg de volgende inhoud toe en vul je eigen SSID en wachtwoord in:

   ```cpp
   #pragma once

   #define WIFI_SSID "JOUW_SSID"
   #define WIFI_PASSWORD "JOUW_WACHTWOORD"
   ```

3. Voeg het bestand toe aan `.gitignore` zodat je gegevens niet op GitHub komen:

   ```
   include/wifi_config.h
   ```

## MQTT-configuratie

Ook je MQTT-gegevens kun je veilig houden door ze in een apart bestand te plaatsen.  
Maak zelf een bestand aan met de naam `mqtt_config.h` in de map `/include` van het project.

### Stappen

1. Maak het bestand aan:  
   ```
   /include/mqtt_config.h
   ```

2. Voeg de volgende inhoud toe en vul je eigen MQTT-instellingen in:

   ```cpp
   #pragma once

   #define MQTT_SERVER "JOUW_MQTT_BROKER_IP"
   #define MQTT_PORT 1883
   #define MQTT_USER ""
   #define MQTT_PASS ""
   ```

   > Laat `MQTT_USER` en `MQTT_PASS` leeg als je broker anonieme toegang toestaat.

3. Voeg het bestand toe aan `.gitignore` zodat je gegevens niet op GitHub komen:

   ```
   include/mqtt_config.h
   ```

## Gebruik

- Upload de code via PlatformIO.
- Schakel het relais via een HTTP GET-request:  
  - Aan: `http://<IP-ESP32S3>/relay?state=on`
  - Uit: `http://<IP-ESP32S3>/relay?state=off`
- De onboard LED knippert automatisch elke 2 seconden.

### Gebruik met Loxone

1. **Virtuele Uitgang aanmaken:**
   - Ga in Loxone Config naar "Periferie" → "Virtuele Uitgangen".
   - Voeg een nieuwe Virtuele Uitgang toe.
   - Stel het IP-adres in van je ESP32S3 en poort 80.
   - Kies als type: **HTTP**.

2. **Virtuele Uitgangscommando's toevoegen:**
   - Voor relais aan:
     ```
     GET /relay?state=on
     ```
   - Voor relais uit:
     ```
     GET /relay?state=off
     ```

3. **Virtuele Uitgang schakelen:**
   - Gebruik een Virtuele Uitgangsconnector in je Loxone-programma om het commando te sturen.

### Gebruik met MQTT

- **Relais schakelen:**  
  Stuur een bericht naar het topic  
  ```
  /esp32s3/relay/set
  ```
  met als payload `"on"` of `"off"`.

- **Relais status ontvangen:**  
  Het device publiceert de status op het topic  
  ```
  /esp32s3/relay/status
  ```
  met als payload `"on"` of `"off"`.

- Je kunt bijvoorbeeld [MQTT Explorer](https://mqtt-explorer.com/) of [Mosquitto_pub](https://mosquitto.org/) gebruiken om te testen.

## OTA (Over-the-Air) Support

Dit project ondersteunt OTA-updates, zodat je het device via WiFi kunt programmeren zonder USB-kabel.

### OTA uploaden via PlatformIO

1. **Zorg dat je ESP32S3 met OTA-code is geprogrammeerd en verbonden met WiFi.**
2. **Vind het IP-adres van je ESP32S3** (bijvoorbeeld via seriële monitor).
3. **Pas je `platformio.ini` aan:**
   ```ini
   upload_protocol = espota
   upload_port = <IP-ESP32S3>
   ```
   Vervang `<IP-ESP32S3>` door het juiste IP-adres van je device.

4. **Upload via PlatformIO:**
   - Gebruik de upload-knop in VS Code of het commando:
     ```
     pio run --target upload
     ```
   - Je ESP32S3 wordt nu via WiFi geprogrammeerd.

## Opmerking

Iedereen die dit project gebruikt, moet zelf een eigen `wifi_config.h` en `mqtt_config.h` aanmaken met zijn eigen WiFi- en MQTT-gegevens.