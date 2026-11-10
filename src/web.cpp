#include "web.h"

#include "servo.h"

#include <LittleFS.h>
#include <WiFi.h>
#include <WebServer.h>

void handleWave() {
    int n = server.hasArg("n") ? server.arg("n").toInt() : 3;
    if(n < 1 || n > 20) n = 3;
    startWave(n);
    server.send(200, "text/plain", "waving");
}

void handleRoot() {
    File f = LittleFS.open("/index.html", "r");

    if (!f) {
        server.send(404, "text/plain", "index.html missing, run uploadfs");
        return;
    }

    server.streamFile(f, "text/html");
    f.close();
}
