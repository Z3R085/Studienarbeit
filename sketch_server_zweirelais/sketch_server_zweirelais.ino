#include <ESPAsyncWebServer.h>
#include <WebResponseImpl.h>
#include <StringArray.h>
#include <AsyncEventSource.h>
#include <WebHandlerImpl.h>
#include <AsyncWebSocket.h>
#include <AsyncJson.h>
#include <SPIFFSEditor.h>
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

// Asynchroner Web-Servers auf Port 80
AsyncWebServer server(80);

// HTML-Code für die Webseite, die auf dem ESP32 gehostet wird
//Speichern des HTML-Codes im Flash 
const char index_html[] PROGMEM = R"rawliteral(  
<!DOCTYPE HTML><html>
<head>
  <title>Relais Steuerung</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      margin: 0;
      display: flex;
      flex-direction: column;
      justify-content: center;
      align-items: center;
      height: 100vh;
      background-color: #f4f4f4;
    }
    button {
      margin: 5px;
      padding: 10px 20px;
      font-size: 16px;
      cursor: pointer;
    }
    #feuchtigkeit {
      margin-top: 15px;
    }
  </style>
  <script>
  // Funktion zum Umschalten der Relais über einen HTTP-Request
  function toggleRelay(relay) { //toggleRelay bekommt Nummer der "Pumpe"
    var xhr = new XMLHttpRequest(); // Ein neues XMLHttpRequest-Objekt wird erstellt, das für die Durchführung von HTTP-Anfragen verwendet wird.
     // Die "open"-Methode konfiguriert die Art der Anfrage (GET), die Ziel-URL und setzt die Anfrage als asynchron (true).
    // Der URL wird der Query-Parameter "relay" angehängt, der den Wert des "relay"-Parameters enthält.
    xhr.open("GET", "/toggle?relay="+relay, true);
    // Die "send"-Methode sendet die konfigurierte Anfrage an den Server.
    xhr.send();
  }

  // Funktion zum periodischen Abrufen des Feuchtigkeitswertes
  function refreshData() {
    var xhr = new XMLHttpRequest(); 
    // Festlegen einer Funktion, die aufgerufen wird, wenn sich der Zustand des XMLHttpRequests ändert.
    xhr.onreadystatechange = function() {
       // Überprüfen, ob die Anfrage abgeschlossen ist (readyState == 4) und ob sie erfolgreich war (status == 200).
      if (this.readyState == 4 && this.status == 200) {
        // Aktualisieren des Textinhalts des HTML-Elements mit der ID "feuchtigkeit" mit der Antwort des Servers.
        document.getElementById("feuchtigkeit").innerText = this.responseText;
      }
    };
    // Konfigurieren der GET-Anfrage
    xhr.open("GET", "/feuchtigkeit", true);
    //Senden der Anfrage an den Server
    xhr.send();
  }

  // Aktualisieren Sie die Feuchtigkeitsdaten alle 3 Sekunden
  setInterval(refreshData, 3000);
  </script>
</head>
<body>
  <h1>Relais Steuerung</h1>
  <button onclick="toggleRelay(1)">Pumpe 1 umschalten</button>
  <button onclick="toggleRelay(2)">Pumpe 2 umschalten</button>
  <p>Feuchtigkeit: <span id="feuchtigkeit">Lade...</span></p>
</body>
</html>
)rawliteral";



void setup() {
  // Initialisieren der seriellen Kommunikation mit einer Baudrate von 9600
  Serial.begin(9600);

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

  // Definieren der Route für die Wurzel-URL, die das HTML zurückgibt
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html);
  });

  // Definieren der Route "/toggle", um auf HTTP GET-Anfragen zu reagieren, die zum Umschalten der Relais dienen.
  // Extrahieren des 'relay'-Parameters aus der Anfrage, der angibt, welches Relais (Pumpe) umgeschaltet werden soll.
  server.on("/toggle", HTTP_GET, [](AsyncWebServerRequest *request) {
    String relay = request->getParam("relay")->value();
    if (relay == "1") {
      // Relais 1 toggeln
      digitalWrite(RELAIS1_PIN, !digitalRead(RELAIS1_PIN));
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
  // Hier bleibt der Loop leer, da alle Operationen über den Webserver asynchron ablaufen
}
