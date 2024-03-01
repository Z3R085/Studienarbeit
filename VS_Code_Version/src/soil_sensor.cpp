#include "soil_sensor.h"
#include <Arduino.h>

/// Pinbelegung
#define FEUCHTIGKEIT_PIN 34

void setupsoil_sensor()
{
    // Setup fuer Feuchtigkeitssoil_sensor-Pin
    pinMode(FEUCHTIGKEIT_PIN, INPUT);
}

String readsoil_sensor()
{
    // Auslesen des soil_sensorwertes
    int soil_sensorValue = analogRead(FEUCHTIGKEIT_PIN);

    // Definieren der Min- und Max-Werte des soil_sensors
    const int minValue = 950;  // Nasse Erde
    const int maxValue = 1900; // Trockene Erde

    // Umwandlung in Prozent
    soil_sensorValue = map(soil_sensorValue, maxValue, minValue, 0, 100);

    return String(soil_sensorValue);
}

float getTankLevel()
{
    return 100;
}
