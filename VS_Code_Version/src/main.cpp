#include <ESPAsyncWebServer.h>
#include <WebResponseImpl.h>
#include <StringArray.h>
#include <AsyncEventSource.h>
#include <WebHandlerImpl.h>
#include <AsyncWebSocket.h>
#include <AsyncJson.h>
#include <SPIFFSEditor.h>
#include <SPIFFS.h>
#include <WebAuthentication.h>
#include <AsyncWebSynchronization.h>
#include <AsyncTCP.h>
#include <Arduino.h>
#include <network.h>
#include <relay.h>




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



  


  //Beginn der eigentlichen Logik
// Definieren der Route für die Wurzel-URL, die das HTML aus einer Datei zurückgibt
server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
  request->send(SPIFFS, "/index.html", "text/html");
});


  // Definieren der Route "/toggle", um auf HTTP GET-Anfragen zu reagieren, die zum Umschalten der Relais dienen.
  // Extrahieren des 'relay'-Parameters aus der Anfrage, der angibt, welches Relais (Pumpe) umgeschaltet werden soll.
  server.on("/toggle", HTTP_GET, [](AsyncWebServerRequest *request) {
    // Extrahieren des 'relay'-Parameters aus der Anfrage, der angibt, welches Relais (Pumpe) umgeschaltet werden soll.
    String relay = request->getParam("relay")->value();

    // Umschalten des Relais
    toggleRelay(relay.toInt());
    // Senden einer HTTP-Antwort mit dem Statuscode 200 (OK) und einer einfachen Textnachricht "OK" als Bestätigung, dass die Aktion ausgeführt wurde.
    request->send(200, "text/plain", "OK");
  });

  // Definieren der Route zum Abrufen des aktuellen Feuchtigkeitswerts
  server.on("/feuchtigkeit", HTTP_GET, [](AsyncWebServerRequest *request) {
    int sensorValue = analogRead(FEUCHTIGKEIT_PIN);  
    float voltage = sensorValue * (3.3 / 4095.0); // Umwandlung in Spannung
    String feuchtigkeit = String(voltage, 2) + "V"; // Umwandlung in String mit 2 Dezimalstellen
    request->send(200, "text/plain", feuchtigkeit);
  });

  // Starten des Webservers
  server.begin();
}

void loop() {
  // Überprüfen, ob die Verzögerungszeit für Relais abgelaufen ist. Wenn ja, schalte das Relais aus.
  checkRelays();
}
