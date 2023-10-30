#define FEUCHTIGKEIT_PIN 34  // Definiert den Pin, an dem der Feuchtigkeitssensor angeschlossen ist

void setup() {
  Serial.begin(9600); 
}

void loop() {
  int sensorValue = analogRead(FEUCHTIGKEIT_PIN);  // Liest den Wert vom Sensor
  float voltage = sensorValue * (3.3 / 4095.0);  // Wandelt den ADC-Wert in eine Spannung um (für ESP32 mit 3,3V und 12-bit ADC)

  Serial.print("ADC Wert: ");  
  Serial.print(sensorValue);
  Serial.print("\tSpannung: ");
  Serial.println(voltage, 2);  // Gibt die Spannung mit 2 Dezimalstellen aus

  delay(1000);  // Wartet eine Sekunde vor dem nächsten Auslesen
}
