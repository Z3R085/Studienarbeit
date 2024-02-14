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




// Asynchroner Web-Servers auf Port 80
AsyncWebServer server(80);

// Zeitplan-Manager

unsigned long lastCheck = 0; // Speichert den Zeitpunkt der letzten Überprüfung
const unsigned long interval = 60000;  
const unsigned long automatic_interval = 1000; 
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
  
  /// Initialisiere die Webserver-Routen
    setupRoutes(server);;
  
  // Starten des Webservers
  server.begin();
}

void loop() {
  // Aktualisieren der Zeit
  currentTime = millis();
  
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
    //saveSensorValue();
    lastCheck = currentTime;
  }
  break;
  
}
case WateringMode::SENSOR_BASED:
 
   
  if (currentTime - lastCheck >= automatic_interval) {
    automaticIrrigation();
    //saveSensorValue();
    lastCheck = currentTime;
  }
  break;
    

  default:
    Serial.println("Unbekannter Bewässerungsmodus");
    break;
  }
  
}