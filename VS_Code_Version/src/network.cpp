#include "network.h"
#include <WiFi.h>

void connectToWifi()
{
  // Verbinden mit dem WLAN-Netzwerk
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Verbindung zum WLAN-Netzwerk wird aufgebaut...");
  }

  Serial.println("Verbunden mit WiFi-Netzwerk");
  Serial.println(WiFi.localIP());
}