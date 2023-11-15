#include "routes.h"
#include "relay.h" // Für toggleRelay
#include "sensor.h" // Für readSensor
#include <SPIFFS.h>

void setupRoutes(AsyncWebServer &server) {
    // Route für die Wurzel-URL, die das HTML aus einer Datei zurückgibt
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/index.html", "text/html");
    });

    // Route zum Umschalten der Relais
    server.on("/toggle", HTTP_GET, [](AsyncWebServerRequest *request) {
        String relay = request->getParam("relay")->value();
        int relayNumber = relay.toInt(); // Konvertiere die Relay-Nummer von String zu Int

        toggleRelay(relayNumber);

        request->send(200, "text/plain", "OK");
    });

    // Route zum Abrufen des aktuellen Feuchtigkeitswerts
    server.on("/feuchtigkeit", HTTP_GET, [](AsyncWebServerRequest *request) {
        String feuchtigkeit = readSensor();
        request->send(200, "text/plain", feuchtigkeit);
    });
}
