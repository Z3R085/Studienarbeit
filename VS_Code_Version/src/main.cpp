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

#include <WiFi.h>

// Netzwerkinformationen 
const char* ssid = "FlosWlan";
const char* password = "16794671194111394536";

//Pinbelegung
#define FEUCHTIGKEIT_PIN 34
#define RELAIS1_PIN 4
#define RELAIS2_PIN 5

// Globale Variablen, um den Zustand und die Zeit zu speichern
bool isRelay1On = false;
unsigned long relay1Timer = 0;
const int relayDelay = 2500; // Versuch immer 100ml zu pumpen

// Asynchroner Web-Servers auf Port 80
AsyncWebServer server(80);

// HTML-Code für die Webseite, die auf dem ESP32 gehostet wird
//Speichern des HTML-Codes im Flash 




void setup() {
  // Initialisieren der seriellen Kommunikation mit einer Baudrate von 9600
  Serial.begin(9600);

  SPIFFS.begin();

  // Initialisieren der Relais-Pins als Ausgänge
  pinMode(RELAIS1_PIN, OUTPUT);
  pinMode(RELAIS2_PIN, OUTPUT);

  // Setzen der Relais in den Aus-Zustand
  digitalWrite(RELAIS1_PIN, LOW);
  digitalWrite(RELAIS2_PIN, LOW);

  // Verbinden mit dem WLAN-Netzwerk
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Verbindung zum WLAN-Netzwerk wird aufgebaut...");
  }
  
  Serial.println("Verbunden mit WiFi-Netzwerk");
  Serial.println(WiFi.localIP());


  //Beginn der eigentlichen Logik
// Definieren der Route für die Wurzel-URL, die das HTML aus einer Datei zurückgibt
server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
  request->send(SPIFFS, "/index.html", "text/html");
});


  // Definieren der Route "/toggle", um auf HTTP GET-Anfragen zu reagieren, die zum Umschalten der Relais dienen.
  // Extrahieren des 'relay'-Parameters aus der Anfrage, der angibt, welches Relais (Pumpe) umgeschaltet werden soll.
  server.on("/toggle", HTTP_GET, [](AsyncWebServerRequest *request) {
    String relay = request->getParam("relay")->value();
    if (relay == "1") {
      // Einschalten von Relais 1 ohne Verwendung von delay()
    digitalWrite(RELAIS1_PIN, !digitalRead(RELAIS1_PIN));
    // Speichern des aktuellen Zeitstempels und des Zustands
    relay1Timer = millis();
    isRelay1On = !isRelay1On;
    } else if (relay == "2") {
      // Relais 2 toggeln
      digitalWrite(RELAIS2_PIN, !digitalRead(RELAIS2_PIN));
    }
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
  // Überprüfen, ob die Verzögerungszeit für Relais 1 abgelaufen ist
  if (isRelay1On && millis() - relay1Timer >= relayDelay) {
    // Relais zurückumschalten
    digitalWrite(RELAIS1_PIN, !digitalRead(RELAIS1_PIN));
    // Zurücksetzen des Zustands
    isRelay1On = false;
  }
}
