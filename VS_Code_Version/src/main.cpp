#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <AsyncTCP.h> 

#include <timesync.h>
#include <sensor.h>	
#include <network.h>
#include <pump.h>
#include <routes.h>
#include <schedule.h>



// Asynchroner Web-Servers auf Port 80
AsyncWebServer server(80);

// Zeitplan-Manager

unsigned long lastCheck = 0; // Speichert den Zeitpunkt der letzten Überprüfung
const unsigned long interval = 60000; // Überprüfungsintervall in Millisekunden (1 Minute)



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

  // Setup fuer Feuchtigkeitssensor-Pin
  setupSensor();
  
  /// Initialisiere die Webserver-Routen
    setupRoutes(server);;
  
  // Starten des Webservers
  server.begin();
}

void loop() {
  // ueberpruefen, ob die Verzögerungszeit fuer pump abgelaufen ist. Wenn ja, schalte die pump aus.
  checkpumps();

  unsigned long currentTime = millis(); // Aktuelle Zeit seit Programmstart in Millisekunden
  if (currentTime - lastCheck >= interval) { // Prüft, ob das Intervall vergangen ist
    checkAndRunEvents(); // Funktion aufrufen
    lastCheck = currentTime; // Aktualisiert den Zeitpunkt der letzten Überprüfung
  }


 
}