#include "sensor.h"
#include <Arduino.h>

// Definieren Sie die Pin-Nummer, an die Ihr Feuchtigkeitssensor angeschlossen ist
#define FEUCHTIGKEIT_PIN 34

String readSensor() {
    // Auslesen des Sensorwertes
    int sensorValue = analogRead(FEUCHTIGKEIT_PIN);  
    // Umwandlung in Spannung
    float voltage = sensorValue * (3.3 / 4095.0); 
    // Umwandlung in String mit 2 Dezimalstellen und Anhängen des "V" für Volt
    //return String(voltage, 2) + "V";
    return String(sensorValue);
}
