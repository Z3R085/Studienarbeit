#include "automatic_irrigation.h"
#include <soil_sensor.h>
#include <pump.h>
#include <Arduino.h>
#include <string.h>
void automaticIrrigation() {
    // Auslesen der Sensorwerte
    float tankLevel = getTankLevel(); // Implementierung benötigt
    float temperature = getTemperature(); // Implementierung benötigt
    float soilMoisture = readsoil_sensor().toFloat();

    Serial.println("Automatic Irrigation");

    // Bedingungen für das Einschalten der Pumpe
    if (tankLevel > 25 && temperature < 40 && soilMoisture < 90) {
        Serial.println("Es ist Zeit zu gießen!");
        Serial.println("Status der Pumpe: " + String(pumps[0].isOn));
        // Einschalten der Pumpe, wenn sie noch nicht eingeschaltet ist
        if (!pumps[0].isOn){ 
            togglepump(1); // Zustand der Pumpe umschalten
            Serial.println("Pumpe eingeschaltet");

        }
    } 
    else {
        Serial.println("Es ist nicht Zeit zu gießen!");
        // Ausschalten der Pumpe, wenn sie eingeschaltet ist
        if (pumps[0].isOn) { // Überprüfen, ob die Pumpe eingeschaltet ist
            togglepump(1); // Zustand der Pumpe umschalten
            Serial.println("Pumpe ausgeschaltet");
        }
    }
}