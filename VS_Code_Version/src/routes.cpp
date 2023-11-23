#include "routes.h"
#include "relay.h" // Für toggleRelay
#include "sensor.h" // Für readSensor
#include <SPIFFS.h>
#include <ArduinoJson.h>
void setupRoutes(AsyncWebServer &server) {
    // Route für die Wurzel-URL, die das HTML aus einer Datei zurückgibt
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/index.html", "text/html");
    });

    // Route für die style.css-Datei
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/style.css", "text/css");
    });

    // Route für die script.js-Datei
    server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/script.js", "text/javascript");
    });


    // Route zum Umschalten der Relais
    server.on("/relais/toggle", HTTP_PUT, [](AsyncWebServerRequest *request) {
    if (request->hasParam("relay", true)) {
        String relay = request->getParam("relay", true)->value();
        int relayNumber = relay.toInt();
        toggleRelay(relayNumber);
        request->send(200, "text/plain", "OK");
    } else {
        request->send(400, "text/plain", "Bad Request");
    }
    });


    // Route zum Abrufen des aktuellen Feuchtigkeitswerts
    server.on("/feuchtigkeit", HTTP_GET, [](AsyncWebServerRequest *request) {
        String feuchtigkeit = readSensor();
        request->send(200, "text/plain", feuchtigkeit);
    });

     // Route zum Abrufen des lastActivated-Zeitstempels für jedes Relais
    server.on("/relay-status", HTTP_GET, [](AsyncWebServerRequest *request) {
        DynamicJsonDocument doc(1024); // Erstelle ein JSON-Dokument mit einer Größe von 1024 Bytes
        // Erstelle ein JSON-Array mit der Anzahl der Relais
        for (int i = 0; i < numRelays; ++i) {
            doc["relays"][i]["pin"] = relays[i].pin; // Pin-Nummer
            doc["relays"][i]["lastActivated"] = (relays[i].lastActivated);;    // Zeitstempel
        }

        String response; 
        serializeJson(doc, response); 
        request->send(200, "application/json", response); // Senden der JSON-Datei
    });
}
