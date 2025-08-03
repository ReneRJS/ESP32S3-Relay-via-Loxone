#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "wifi_config.h" // <-- Voeg dit bestand toe in /include en zet je credentials daarin

// Definieer de pin voor de onboard LED en het relais
const int LED_PIN = 21;    // Onboard LED XIAO ESP32S3
const int RELAY_PIN = 2;   // Relay Expansion GPIO2

// WiFi instellingen uit extern bestand
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

// HTTP server instellingen
WebServer httpServer(80); // HTTP server op poort 80

unsigned long previousLedMillis = 0;
const long ledInterval = 2000;
bool ledState = false;

// HTTP handler voor relais
void handleRelay() {
  String state = httpServer.arg("state");
  if (state == "on") {
    digitalWrite(RELAY_PIN, HIGH);
    httpServer.send(200, "text/plain", "Relay ON");
  } else if (state == "off") {
    digitalWrite(RELAY_PIN, LOW);
    httpServer.send(200, "text/plain", "Relay OFF");
  } else {
    httpServer.send(400, "text/plain", "Unknown command");
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  httpServer.on("/relay", handleRelay);
  httpServer.begin();
}

void loop() {
  // LED knipperen elke 2 seconden
  unsigned long currentMillis = millis();
  if (currentMillis - previousLedMillis >= ledInterval) {
    previousLedMillis = currentMillis;
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState ? HIGH : LOW);
  }

  // HTTP server afhandelen
  httpServer.handleClient();
}