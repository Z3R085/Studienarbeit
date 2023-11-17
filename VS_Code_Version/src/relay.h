#include <Arduino.h>

// Pinbelegung
#define RELAIS1_PIN 4
#define RELAIS2_PIN 5

struct Relay {
    int pin; // Pin, an dem das Relais angeschlossen ist
    bool isOn; // Zustand des Relais (ein- oder ausgeschaltet)
    unsigned long timer; // Zeit, zu der das Relais ausgeschaltet werden soll
    String lastActivated; // Zeit, zu der das Relais zuletzt eingeschaltet wurde
};

// Globale Variablen für die Relais
extern Relay relays[]; // Array mit den Relais
extern const int numRelays; // Anzahl der Relais

extern const unsigned long relayDelay; // Verzögerung, nach der das Relais ausgeschaltet werden soll

void toggleRelay(int relayNumber); // Funktion zum Umschalten eines Relais
void checkRelays(); // Funktion zum Überprüfen, ob die Verzögerungszeit für Relais abgelaufen ist. Wenn ja, schalte das Relais aus.


