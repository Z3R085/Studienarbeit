#pragma once

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

// Initialisierungsfunktion
void sensorTempSetup();

// Funktionen zum Auslesen der Sensordaten
float readTemperature();
float readHumidity();
