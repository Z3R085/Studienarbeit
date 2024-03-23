#include "automatic_irrigation.h"
#include <soil_sensor.h>
#include <pump.h>
#include <Arduino.h>
#include <string.h>
#include <temp_sensor.h>
void automaticIrrigation()
{
    // Auslesen der Sensorwerte
    float tankLevel = getTankLevel();       // Implementierung benötigt, aktuell nur Dummy-Wert
    float temperature = readTemperature();  // Temperatur auslesen aus temp_sensor.cpp
    float soilMoisture = readsoil_sensor(); // Bodenfeuchtigkeit auslesen aus soil_sensor.cpp
    float humidity = readHumidity();        // Luftfeuchtigkeit auslesen aus temp_sensor.cpp

    // Bedingungen für das Einschalten der Pumpe
    if (tankLevel > 5 && temperature < 35 && soilMoisture < 60)
    {
        togglepump(1); // Pumpe einschalten
        delay(2000);   // Pumpe für 2 Sekunden laufen lassen
        togglepump(1); // Pumpe ausschalten
    }
}