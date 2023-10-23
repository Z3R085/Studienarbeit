#define RELAIS1_PIN 4
#define RELAIS2_PIN 5

void setup() {
  Serial.begin(9600);
  
  pinMode(RELAIS1_PIN, OUTPUT);
  pinMode(RELAIS2_PIN, OUTPUT);

  digitalWrite(RELAIS1_PIN, LOW);
  digitalWrite(RELAIS2_PIN, LOW);

  Serial.println("Relais-Steuerung bereit. Geben Sie '1' ein, um Relais 1 zu schalten.");
  Serial.println("Geben Sie '2' ein, um Relais 2 zu schalten.");
}

void loop() {
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
      case '\n':  // Zeilenumbruch ignorieren
      case '\r':  // Wagenrücklauf ignorieren
        break;
      default:
        Serial.println("Ungültige Eingabe! Bitte '1' oder '2' eingeben.");
        break;
    }
  }
}
