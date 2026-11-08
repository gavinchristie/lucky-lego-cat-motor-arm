#include <ESP32Servo.h>
#include <WiFi.h>
#include <WebServer.h>

constexpr size_t PIN = 13;
constexpr size_t MIN_PULSE = 500;
constexpr size_t MAX_PULSE = 2400;
constexpr size_t UP_STATE = 180;
constexpr unsigned long MOVE_DELAY_MS = 500;

enum WaveState { IDLE, UP, DOWN };
WaveState state = IDLE;
int wavesLeft = 0;
unsigned long lastMovement = 0;
Servo servo;

WebServer server(80);

void startWave(int count) {
    if (state != IDLE) return;

    wavesLeft = count;
    state = UP;
    servo.write(UP_STATE);
    lastMovement = millis();
}

void updateWave() {
    if (state == IDLE) return;

    if(millis() - lastMovement < MOVE_DELAY_MS) return;

    lastMovement = millis();

    if(state == UP) {
        servo.write(0);
        state = DOWN;
    } else {
        if (--wavesLeft > 0) {
            servo.write(180);
            state = UP;
        } else {
            state = IDLE;
        }
    }
}

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

void handleWave() {
    int n = server.hasArg("n") ? server.arg("n").toInt() : 3;
    if(n < 1 || n > 20) n = 3;
    startWave(n);
    server.send(200, "text/plain", "waving");
}

void setup() {
    Serial.begin(115200);
    servo.setPeriodHertz(50);
    servo.attach(PIN, MIN_PULSE, MAX_PULSE);

    Serial.printf("SSID=%s, PASS=%s",WIFI_SSID, WIFI_PASS);
    
    Serial.printf("Connecting to %s", WIFI_SSID);
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    while (WiFi.status() != WL_CONNECTED) {
        Serial.print('.');
        delay(250);
    }
    Serial.println(WiFi.localIP());

    server.on("/wave", handleWave);
    server.begin();
}

void loop() {
    size_t waveCount = 0;

    server.handleClient();

    updateWave();

}
