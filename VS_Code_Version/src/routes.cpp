#include "routes.h"
#include "pump.h" // Fuer togglepump
#include "sensor.h" // Fuer readSensor
#include <SPIFFS.h>
#include <ArduinoJson.h>
void setupRoutes(AsyncWebServer &server) {
    // Route fuer die Wurzel-URL, die das HTML aus einer Datei zurueckgibt
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/index.html", "text/html");
    });

    // Route fuer die style.css-Datei
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/style.css", "text/css");
    });

    // Route fuer die script.js-Datei
    server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/script.js", "text/javascript");
    });


    // Route zum Umschalten der pumpn
    server.on("/pump/toggle", HTTP_PUT, [](AsyncWebServerRequest *request) {
    if (request->hasParam("pump", true)) {
        String pump = request->getParam("pump", true)->value();
        int pumpNumber = pump.toInt();
        togglepump(pumpNumber);
        request->send(200, "text/plain", "OK");
    } else {
        request->send(400, "text/plain", "Bad Request");
    }
    });

    // Route zum Setzen der Pumpendauer
    server.on("/pump/duration", HTTP_PUT, [](AsyncWebServerRequest *request) {
        if (request->hasParam("pump", true) && request->hasParam("duration", true)) { // Pruefe, ob beide Parameter vorhanden sind
            String pumpParam = request->getParam("pump", true)->value(); // Hole den Wert des pump-Parameters(Nr. der Pumpe)
            String durationParam = request->getParam("duration", true)->value(); // Hole den Wert des duration-Parameters
            int pumpNumber = pumpParam.toInt(); // Konvertiere den pump-Parameter in eine Zahl
            unsigned long duration = durationParam.toInt(); // Konvertiere den duration-Parameter in eine Zahl

            setPumpDuration(pumpNumber, duration); // Setze die Pumpendauer

            request->send(200, "text/plain", "Die Pumpendauer wurde aktualisiert.");
        } else {
            request->send(400, "text/plain", "Bad Request - Parameter fehlen.");
        }
    });


    // Route zum Abrufen des aktuellen Feuchtigkeitswerts
    server.on("/feuchtigkeit", HTTP_GET, [](AsyncWebServerRequest *request) {
        String feuchtigkeit = readSensor();
        request->send(200, "text/plain", feuchtigkeit);
    });

     // Route zum Abrufen des lastActivated-Zeitstempels fuer jedes pump
    server.on("/pump/status", HTTP_GET, [](AsyncWebServerRequest *request) {
        DynamicJsonDocument doc(1024); // Erstelle ein JSON-Dokument mit einer Größe von 1024 Bytes
        // Erstelle ein JSON-Array mit der Anzahl der pumpn
        for (int i = 0; i < numpumps; ++i) {
            doc["pumps"][i]["pin"] = pumps[i].pin; // Pin-Nummer
            doc["pumps"][i]["lastActivated"] = (pumps[i].lastActivated);;    // Zeitstempel
        }

        String response; 
        serializeJson(doc, response); 
        request->send(200, "application/json", response); // Senden der JSON-Datei
    });

    // Route zum Setzen des Gießmodus
    server.on("pump//mode", HTTP_PUT, [](AsyncWebServerRequest *request) {
        if (request->hasParam("mode", true)) {
            String modeParam = request->getParam("mode", true)->value();
            WateringMode mode = static_cast<WateringMode>(modeParam.toInt());
            setWateringMode(mode);
            request->send(200, "text/plain", "Gießmodus aktualisiert.");
        } else {
            request->send(400, "text/plain", "Modus fehlt.");
        }
    });
}
