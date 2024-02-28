#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <AsyncTCP.h>
#include <timesync.h>
#include <soil_sensor.h>	
#include <network.h>
#include <pump.h>
#include <routes.h>
#include <schedule.h>
#include <database.h>
#include <automatic_irrigation.h>
#include <temp_sensor.h>
#include "esp_task_wdt.h" // Watchdog-Timer



// Asynchroner Web-Servers auf Port 80
AsyncWebServer server(80);

// Zeitplan-Manager

unsigned long lastCheck = 0; // Speichert den Zeitpunkt der letzten Überprüfung
unsigned long lastSave = 0; // Speichert den Zeitpunkt der letzten Speicherung
unsigned long lastCheck_sensor = 0; // S
const unsigned long interval = 60000;  // Überprüft den Zeitplan alle 60 Sekunden
const unsigned long automatic_interval = 1000; // Überprüft die Sensorwerte alle 1 Sekunden
const unsigned long savedata_intervall = 300000; // Speichert die Sensorwerte alle 5 Minuten
unsigned long currentTime = millis();



void setup() {
  // Initialisieren der seriellen Kommunikation mit einer Baudrate von 9600
  Serial.begin(9600);

  // Initialisieren des Dateisystems
  SPIFFS.begin();

  //WLAN-Verbindung herstellen
  connectToWifi();

  // Setup fuer die Zeit
  setupTime();

  // Setup fuer pump-Pins 
  setuppumps();

  // Setup fuer Feuchtigkeitssoil_sensor-Pin
  setupsoil_sensor();

  // Setup für den DHT11-Sensor (Temperatur und Luftfeuchtigkeit)
  sensorTempSetup();
  
  /// Initialisiere die Webserver-Routen
    setupRoutes(server);;
  
  // Starten des Webservers
  server.begin();

  esp_task_wdt_init(60, true); // Setze den Timeout auf 60 Sekunden
  esp_task_wdt_add(NULL); // Watchdog-Timer hinzufügen
}

void loop() {
  esp_task_wdt_reset(); // Watchdog-Timer zurücksetzen
  // Aktualisieren der Zeit
  currentTime = millis();
  //Temperatur printen


  //Alle 30 Minuten die Sensorwerte speichern
  if (currentTime - lastSave >= savedata_intervall) {
    saveSensorValue();
    lastSave = currentTime;
  }
  
  
  switch (currentMode)
  {
  case WateringMode::MANUAL:
    // ueberpruefen, ob die Verzögerungszeit fuer pump abgelaufen ist. Wenn ja, schalte die pump aus.
    checkpumps();
    //saveSensorValue();
    
  break;
  case WateringMode::SCHEDULED: {
    
  if (currentTime - lastCheck >= interval) {
    checkAndRunEvents();
    lastCheck = currentTime;
  }
  break;
  
}
case WateringMode::SENSOR_BASED:
 
   
  if (currentTime - lastCheck_sensor >= automatic_interval) {
    automaticIrrigation();
    lastCheck_sensor = currentTime;
  }
  break;
    

  default:
    Serial.println("Unbekannter Bewässerungsmodus");
    break;
  }
  
}