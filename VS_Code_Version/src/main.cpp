#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <AsyncTCP.h> 

#include <sensor.h>	
#include <network.h>
#include <relay.h>
#include <routes.h>




//Pinbelegung
#define FEUCHTIGKEIT_PIN 34
#define RELAIS1_PIN 4
#define RELAIS2_PIN 5

// Globale Variablen, um den Zustand und die Zeit zu speichern
bool isRelay1On = false;
unsigned long relay1Timer = 0;


// Asynchroner Web-Servers auf Port 80
AsyncWebServer server(80);



void setup() {
  // Initialisieren der seriellen Kommunikation mit einer Baudrate von 9600
  Serial.begin(9600);

  // Initialisieren des Dateisystems
  SPIFFS.begin();

  //WLAN-Verbindung herstellen
  connectToWifi();

  // Setup für Relais-Pins
    for (int i = 0; i < numRelays; ++i) {
    pinMode(relays[i].pin, OUTPUT);
    digitalWrite(relays[i].pin, LOW);
  }

  // Setup für Feuchtigkeitssensor-Pin
  pinMode(FEUCHTIGKEIT_PIN, INPUT);
  
  /// Initialisiere die Webserver-Routen
    setupRoutes(server);
  
  // Starten des Webservers
  server.begin();
}

void loop() {
  // Überprüfen, ob die Verzögerungszeit für Relais abgelaufen ist. Wenn ja, schalte das Relais aus.
  checkRelays();
}


