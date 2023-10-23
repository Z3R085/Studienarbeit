const int relayPin = 5; 

void setup() {
  Serial.begin(9600); 
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW); // Initialzustand ist AUS.
}

void loop() {
  if (Serial.available()) { // Überprüfen, ob Daten auf der seriellen Schnittstelle vorhanden sind.
    String command = Serial.readStringUntil('\n'); // Liest die Befehlszeile bis zum Zeilenumbruch.

    // Überprüfen des Befehls und entsprechendes Handeln
    if (command == "ON") {
      digitalWrite(relayPin, HIGH);
      Serial.println("Relais eingeschaltet");
    } 
    else if (command == "OFF") {
      digitalWrite(relayPin, LOW);
      Serial.println("Relais ausgeschaltet");
    }
    else {
      Serial.println("Unbekannter Befehl");
    }
  }
}
