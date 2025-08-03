# ESP32S3 Relay via Loxone

Dit project maakt gebruik van een Seeed Studio XIAO ESP32S3 en een 1-channel relay expansion.  
Het relais is te schakelen via HTTP (geschikt voor Loxone en Postman) en de onboard LED knippert elke 2 seconden.

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

## Opmerking

Iedereen die dit project gebruikt, moet zelf een eigen `wifi_config.h` aanmaken met zijn eigen WiFi-