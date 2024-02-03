#include "sensor.h"
#include <Arduino.h>

/// Pinbelegung
#define FEUCHTIGKEIT_PIN 34

void settupSensor() {
    // Setup fuer Feuchtigkeitssensor-Pin
    pinMode(FEUCHTIGKEIT_PIN, INPUT); //Modul selbst erstellen
}

String readSensor() {
    // Auslesen des Sensorwertes
    int sensorValue = analogRead(FEUCHTIGKEIT_PIN);  

    // Definieren der Min- und Max-Werte des Sensors
    const int minValue = 960;
    const int maxValue = 2844;
    
    //Umwandlung in Prozent
    sensorValue = map(sensorValue, maxValue, minValue, 0, 100); 

    return String(sensorValue);
}
