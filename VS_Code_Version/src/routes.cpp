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

     // Route zum Abrufen des lastActivated-Zeitstempels für jedes Relais
    server.on("/relay-status", HTTP_GET, [](AsyncWebServerRequest *request) {
        DynamicJsonDocument doc(1024); // Erstelle ein JSON-Dokument mit einer Größe von 1024 Bytes
        // Erstelle ein JSON-Array mit der Anzahl der Relais
        for (int i = 0; i < numRelays; ++i) {
            doc["relays"][i]["pin"] = relays[i].pin; // Pin-Nummer
            doc["relays"][i]["isOn"] = relays[i].isOn; // Zustand des Relais (ein- oder ausgeschaltet)
            doc["relays"][i]["lastActivated"] = relays[i].lastActivated; // Zeit, zu der das Relais zuletzt eingeschaltet wurde
        }

        String response; 
        serializeJson(doc, response); 
        request->send(200, "application/json", response); // Senden der JSON-Datei
    });
}
