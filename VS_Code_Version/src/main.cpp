#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <AsyncTCP.h> 

#include <timesync.h>
#include <sensor.h>	
#include <network.h>
#include <pump.h>
#include <routes.h>

//Feuchtigkeitssensor-Pin
#define FEUCHTIGKEIT_PIN 34

// Asynchroner Web-Servers auf Port 80
AsyncWebServer server(80);



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
  pinMode(FEUCHTIGKEIT_PIN, INPUT); //Modul selbst erstellen
  
  /// Initialisiere die Webserver-Routen
    setupRoutes(server);
  
  // Starten des Webservers
  server.begin();
}

void loop() {
  // ueberpruefen, ob die Verzögerungszeit fuer pump abgelaufen ist. Wenn ja, schalte die pump aus.
  checkpumps();

  
}