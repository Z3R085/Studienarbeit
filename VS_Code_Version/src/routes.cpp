#include "routes.h" 
#include "pump.h" // Fuer togglepump
#include "soil_sensor.h" // Fuer readSensor
#include "temp_sensor.h" // Fuer readTemperatureSensor
#include "schedule.h" // Fuer setPumpDuration
#include <SPIFFS.h> // Fuer das Dateisystem
#include <ArduinoJson.h> // Fuer das Arbeiten mit JSON

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
    server.on("/sensor/feuchtigkeit", HTTP_GET, [](AsyncWebServerRequest *request) {
        String feuchtigkeit = readsoil_sensor();
        request->send(200, "text/plain", feuchtigkeit);
    });

    // Route zum Abrufen des aktuellen Temperaturwerts
    server.on("/sensor/temperatur", HTTP_GET, [](AsyncWebServerRequest *request) {
    float temperatur = readTemperature(); 
    // Konvertierung der Temperatur float zu String für die Übertragung
    String tempStr = String(temperatur, 2); // 2 Dezimalstellen
    request->send(200, "text/plain", tempStr);
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
    server.on("/pump/mode/set", HTTP_PUT, [](AsyncWebServerRequest *request) {
        if (request->hasParam("mode", true)) {
            String modeParam = request->getParam("mode", true)->value();
            WateringMode mode = static_cast<WateringMode>(modeParam.toInt());
            setWateringMode(mode);
            request->send(200, "text/plain", "Gießmodus aktualisiert.");
        } else {
            request->send(400, "text/plain", "Modus fehlt.");
        }
    });

    // Route zum Abrufen des Gießmodus
    server.on("/pump/mode/get", HTTP_GET, [](AsyncWebServerRequest *request) {
        WateringMode mode = getWateringMode(); // Funktion, die den aktuellen Modus abruft
        String modeString = String(static_cast<int>(mode)); // Konvertiert Enum zu String
        request->send(200, "text/plain", modeString);
    });

    // Route zum Setzen des Zeitplans
    server.on("/pump/mode/set-schedule", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
    // Erstelle ein temporäres JsonDocument
    // Größe des JsonDocument anpassen, um sicherzustellen, dass es groß genug ist
    DynamicJsonDocument doc(1024);
    
    // Parsen des JSON-Daten, die im Body der POST-Anfrage enthalten sind
    DeserializationError error = deserializeJson(doc, (const char*)data);
    if (error) {
      request->send(400, "application/json", "{\"message\":\"JSON Parsing fehlgeschlagen\"}");
      return;
    }

    // Extrahiere Daten aus dem JsonDocument
    const char* repeatOption = doc["repeat"];
    const char* timeOfDay = doc["timeOfDay"];
    JsonArray daysOfWeekArray = doc["daysOfWeek"];

    Serial.println(repeatOption);
    Serial.println(timeOfDay);
    



    // Erstelle eine neue WateringEvent-Instanz
    WateringEvent event;
    
    if (strcmp(repeatOption, "weekly") == 0) { 
      event.repeat = RepeatInterval::weekly;
      // Fülle den daysOfWeek Vektor
      for (JsonVariant v : daysOfWeekArray) {
        event.daysOfWeek.push_back(v.as<bool>()); 
    }

    } else if (strcmp(repeatOption, "daily") == 0) {
      event.repeat = RepeatInterval::daily;
    } else {
      // Standardwert oder Fehlerbehandlung
      event.repeat = RepeatInterval::daily;
      Serial.println("Ungültige Wiederholungsoption");
    }

    if (strcmp(timeOfDay, "morning") == 0) {
      event.time.tm_hour = 8; event.time.tm_min = 0;
      Serial.println("Morgen");
    } else if (strcmp(timeOfDay, "noon") == 0) {
      event.time.tm_hour = 13; event.time.tm_min = 0;
    } else if (strcmp(timeOfDay, "evening") == 0) {
      event.time.tm_hour = 18; event.time.tm_min = 0;
    } else {
      // Standardwert oder Fehlerbehandlung
      event.time.tm_hour = 0; event.time.tm_min = 0;
      Serial.println("Ungültige Zeitangabe");
    }
  
    // Setze die Dauer auf 10 Sekunden
    event.duration = 10;
    // Füge das Ereignis zum ScheduleManager hinzu
    addEvent(event);

    // Sende eine Bestätigung zurück
    request->send(200, "application/json", "{\"message\":\"Zeitplan gesetzt\"}");
  });
}








