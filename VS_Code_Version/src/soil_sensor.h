#pragma once
#include <Arduino.h>
#define FEUCHTIGKEIT_PIN 34

// Deklaration der Funktion zum Setup des soil_sensors
void setupsoil_sensor();

// Deklaration der Funktion zum Lesen des soil_sensors
String readsoil_sensor();

float getTankLevel();

