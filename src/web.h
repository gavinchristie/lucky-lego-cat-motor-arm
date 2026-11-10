#pragma once

extern WebServer server(80);

/**
 * Handler for the root url on the webserver
 */
void handleRoot();

/**
 * Handler for the wave endpoint on the webserver
 */
void handleWave();
