#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoOTA.h>
#include "wifi_config.h"
#include "mqtt_config.h"
#include <PubSubClient.h>

const int LED_PIN = 21;
const int RELAY_PIN = 2;

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

// MQTT instellingen
WiFiClient espClient;
PubSubClient mqttClient(espClient);

const char* mqtt_server = MQTT_SERVER;
const int mqtt_port = MQTT_PORT;
const char* mqtt_user = MQTT_USER;
const char* mqtt_pass = MQTT_PASS;

const char* relaySetTopic = "/esp32s3/relay/set";
const char* relayStatusTopic = "/esp32s3/relay/status";

WebServer httpServer(80);

unsigned long previousLedMillis = 0;
const long ledInterval = 2000;
bool ledState = false;
bool relayState = false;

// MQTT callback
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String msg;
  for (unsigned int i = 0; i < length; i++) {
    msg += (char)payload[i];
  }
  msg.trim();
  if (String(topic) == relaySetTopic) {
    if (msg == "on") {
      digitalWrite(RELAY_PIN, HIGH);
      relayState = true;
      mqttClient.publish(relayStatusTopic, "on", true);
    } else if (msg == "off") {
      digitalWrite(RELAY_PIN, LOW);
      relayState = false;
      mqttClient.publish(relayStatusTopic, "off", true);
    }
  }
}

// HTTP handler voor relais
void handleRelay() {
  String state = httpServer.arg("state");
  if (state == "on") {
    digitalWrite(RELAY_PIN, HIGH);
    relayState = true;
    mqttClient.publish(relayStatusTopic, "on", true);
    httpServer.send(200, "text/plain", "Relay ON");
  } else if (state == "off") {
    digitalWrite(RELAY_PIN, LOW);
    relayState = false;
    mqttClient.publish(relayStatusTopic, "off", true);
    httpServer.send(200, "text/plain", "Relay OFF");
  } else {
    httpServer.send(400, "text/plain", "Unknown command");
  }
}

void reconnectMQTT() {
  while (!mqttClient.connected()) {
    if (mqttClient.connect("ESP32S3Relay", mqtt_user, mqtt_pass)) {
      mqttClient.subscribe(relaySetTopic);
      // Publish initial status
      mqttClient.publish(relayStatusTopic, relayState ? "on" : "off", true);
    } else {
      delay(2000);
    }
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  mqttClient.setServer(mqtt_server, mqtt_port);
  mqttClient.setCallback(mqttCallback);

  // OTA initialisatie
  ArduinoOTA.setHostname("esp32s3-relay");
  ArduinoOTA.begin();

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

  // OTA afhandelen
  ArduinoOTA.handle();

  // MQTT afhandelen
  if (!mqttClient.connected()) {
    reconnectMQTT();
  }
  mqttClient.loop();
}