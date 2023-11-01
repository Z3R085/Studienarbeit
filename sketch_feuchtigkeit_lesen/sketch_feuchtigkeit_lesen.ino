#define FEUCHTIGKEIT_PIN 34
#define RELAIS1_PIN 4
#define RELAIS2_PIN 5

unsigned long lastFeuchtigkeitMillis = 0;  // Zeitpunkt der letzten Feuchtigkeitsdatenausgabe kleiner Test

void setup() {
  Serial.begin(9600);

  pinMode(RELAIS1_PIN, OUTPUT); // PIN 4 Output
  pinMode(RELAIS2_PIN, OUTPUT); // PIN 5 Output

  digitalWrite(RELAIS1_PIN, LOW);
  digitalWrite(RELAIS2_PIN, LOW);

  Serial.println("Relais-Steuerung bereit. Geben Sie '1' ein, um Relais 1 zu schalten.");
  Serial.println("Geben Sie '2' ein, um Relais 2 zu schalten.");
}

void loop() {
  // Feuchtigkeitsdaten alle drei Sekunden ausgeben
  if (millis() - lastFeuchtigkeitMillis >= 3000) {
    int sensorValue = analogRead(FEUCHTIGKEIT_PIN);  
    float voltage = sensorValue * (3.3 / 4095.0);

    Serial.print("ADC Wert: ");  
    Serial.print(sensorValue);
    Serial.print("\tSpannung: ");
    Serial.println(voltage, 2);

    lastFeuchtigkeitMillis = millis();
  }

  // Relais steuern, wenn Daten über die serielle Schnittstelle verfügbar sind
  if (Serial.available()) {
    char input = Serial.read();

    switch (input) {
      case '1':
        if (digitalRead(RELAIS1_PIN) == LOW) {
          digitalWrite(RELAIS1_PIN, HIGH);
          Serial.println("Relais 1 EIN");
        } else {
          digitalWrite(RELAIS1_PIN, LOW);
          Serial.println("Relais 1 AUS");
        }
        break;
      case '2':
        if (digitalRead(RELAIS2_PIN) == LOW) {
          digitalWrite(RELAIS2_PIN, HIGH);
          Serial.println("Relais 2 EIN");
        } else {
          digitalWrite(RELAIS2_PIN, LOW);
          Serial.println("Relais 2 AUS");
        }
        break;
      case '\n':
      case '\r':
        break;
      default:
        Serial.println("Ungültige Eingabe! Bitte '1' oder '2' eingeben.");
        break;
    }
  }
}
