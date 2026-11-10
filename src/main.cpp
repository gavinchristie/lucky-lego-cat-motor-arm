#include <ESP32Servo.h>
#include <LittleFS.h>
#include <WiFi.h>
#include <WebServer.h>

#include "servo.h"
#include "web.h"

constexpr size_t PIN = 13;
constexpr size_t MIN_PULSE = 500;
constexpr size_t MAX_PULSE = 2400;

bool readWaveCount(size_t &out) {
    if (!Serial.available()) return false;

    String line = Serial.readStringUntil('\n');
    line.trim();

    char *end;
    long v = strtol(line.c_str(), &end, 10);

    if (line.isEmpty() || *end != '\0' || v <= 0 || v > INT_MAX) {
      Serial.println("Enter a number > 0");
      return false;
    }

    out = (int)v;
    return true;
}

size_t askCount() {
    while(true) {

        while (!Serial.available()) delay(20);

        String line = Serial.readStringUntil('\n');
        line.trim();

        char *end;
        long v =strtol(line.c_str(), &end, 10);

        if (!line.isEmpty() && *end == '\0' && v > 0) return v;
        
        Serial.println("Needs to be a number >0");

    }
}

void setup() {
    Serial.begin(115200);
    servo.setPeriodHertz(50);
    servo.attach(PIN, MIN_PULSE, MAX_PULSE);

    if(!LittleFS.begin(true)) {
        Serial.println("LittleFS mount failed");
    }

    Serial.printf("SSID=%s, PASS=%s",WIFI_SSID, WIFI_PASS);
    
    Serial.printf("Connecting to %s", WIFI_SSID);
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    while (WiFi.status() != WL_CONNECTED) {
        Serial.print('.');
        delay(250);
    }
    Serial.println(WiFi.localIP());

    server.on("/", handleRoot);
    server.on("/wave", handleWave);
    server.begin();
}

void loop() {
    server.handleClient();

    updateWave();

}
