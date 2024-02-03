#include <Arduino.h>
#define FEUCHTIGKEIT_PIN 34

// Deklaration der Funktion zum Setup des Sensors
void setupSensor();

// Deklaration der Funktion zum Lesen des Sensors
String readSensor();

float getTankLevel();
float getTemperature();
