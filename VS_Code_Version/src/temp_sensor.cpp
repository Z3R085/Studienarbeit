
#include "temp_sensor.h"

#define DHTPIN 23     // Digitaler Pin, der mit dem DHT-Sensor verbunden ist
#define DHTTYPE DHT11 // DHT 11 

DHT_Unified dht(DHTPIN, DHTTYPE);

void sensorTempSetup() {
  dht.begin(); // Initialisierung des DHT-Sensors

}

float readTemperature() {
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Fehler beim Ablesen der Temperatur!"));
    return NAN;
  } else {
    return event.temperature;
  }
}

float readHumidity() {
  sensors_event_t event;
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Fehler beim Ablesen der Luftfeuchtigkeit!"));
    return NAN;
  } else {
    return event.relative_humidity;
  }
}
