#include "network.h"
#include <WiFi.h>

// Netzwerkinformationen 
const char* ssid = "FlosWlan";
const char* password = "16794671194111394536";

void connectToWifi(){
    // Verbinden mit dem WLAN-Netzwerk
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Verbindung zum WLAN-Netzwerk wird aufgebaut...");
  }
  
  Serial.println("Verbunden mit WiFi-Netzwerk");
  Serial.println(WiFi.localIP());
    
}