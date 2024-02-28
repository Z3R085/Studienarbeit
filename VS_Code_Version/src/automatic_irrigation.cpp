#include "automatic_irrigation.h"
#include <soil_sensor.h>
#include <pump.h>
#include <Arduino.h>
#include <string.h>
#include <temp_sensor.h>
void automaticIrrigation() {
    // Auslesen der Sensorwerte
    float tankLevel = getTankLevel(); // Implementierung benötigt, aktuell nur Dummy-Wert
    float temperature = readTemperature(); // Temperatur auslesen aus temp_sensor.cpp
    float soilMoisture = readsoil_sensor().toFloat();
    float humidity = readHumidity(); // Luftfeuchtigkeit auslesen aus temp_sensor.cpp


    // Bedingungen für das Einschalten der Pumpe
    if (tankLevel > 5 && temperature < 35 && soilMoisture < 50) {
        // Einschalten der Pumpe, wenn sie noch nicht eingeschaltet ist
        if (!pumps[0].isOn){ 
            togglepump(1); // Zustand der Pumpe umschalten
        }
    } 
    else {
        // Ausschalten der Pumpe, wenn sie eingeschaltet ist
        if (pumps[0].isOn) { // Überprüfen, ob die Pumpe eingeschaltet ist
            togglepump(1); // Zustand der Pumpe umschalten

        }   
             

    }
}