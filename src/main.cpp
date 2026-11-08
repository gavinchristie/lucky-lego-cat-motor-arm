#include <ESP32Servo.h>

constexpr size_t PIN = 13;
constexpr size_t MIN_PULSE = 500;
constexpr size_t MAX_PULSE = 2400;

Servo servo;

void setup() {
    Serial.begin(115200);
    servo.setPeriodHertz(50);
    servo.attach(PIN, MIN_PULSE, MAX_PULSE);
}

void waveArm(size_t count) {
    Serial.printf("waving %d times\n", count);
    for(int i = 0; i < count; i++) {
        Serial.printf("flip #%d\n", i);
        servo.write(0);
        delay(1000);
        servo.write(90);
        delay(1000);

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
        Serial.print("How many times should I wave? ");

        while (!Serial.available()) delay(20);

        String line = Serial.readStringUntil('\n');
        line.trim();

        char *end;
        long v =strtol(line.c_str(), &end, 10);

        if (!line.isEmpty() && *end == '\0' && v > 0) return v;
        
        Serial.println("Needs to be a number >0");

    }
}

void loop() {
    size_t n;
    if (readWaveCount(n)) {
        waveArm(n);
    }
}
