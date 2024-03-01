#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "soil_sensor.h"
#include "string.h"
#include "temp_sensor.h"

const char *serverUrl = "http://192.168.178.42:5000/api/insertData";

void saveSensorValue()
{
    float moisture = readsoil_sensor().toFloat();
    float temperature = readTemperature();
    Serial.println("Feuchtigkeitswert: " + String(moisture));
    Serial.println("Temperatur: " + String(temperature));
    if (WiFi.status() == WL_CONNECTED)
    {
        HTTPClient http;
        http.begin(serverUrl);                              // Starte die Verbindung zum Server
        http.addHeader("Content-Type", "application/json"); // Setze den Inhaltstyp auf JSON

        StaticJsonDocument<200> doc; // Erstelle ein JSON-Dokument
        // Füge die Feuchtigkeits- und Temperaturwerte zum Dokument hinzu
        doc["feuchtigkeitswert"] = moisture;
        doc["temperatur"] = temperature;

        String requestBody;
        serializeJson(doc, requestBody); // Konvertiere das JSON-Dokument in einen String

        int httpResponseCode = http.POST(requestBody); // Sende die POST-Anfrage

        // Überprüfe den HTTP-Antwortcode und drucke die Antwort
        if (httpResponseCode > 0)
        {
            Serial.print("HTTP Response code: ");
            Serial.println(httpResponseCode);
            String response = http.getString(); // Hole die Antwort
            Serial.println(response);           // Drucke die Antwort zur Debugging-Zwecken
        }
        else
        {
            Serial.print("Error on sending POST: ");
            Serial.println(httpResponseCode);
        }

        http.end(); // Beende die Verbindung
    }
    else
    {
        Serial.println("Error in WiFi connection");
    }
}
