#include <Arduino.h>

// Pinbelegung
#define RELAIS1_PIN 4
#define RELAIS2_PIN 5

struct Relay {
    int pin;
    bool isOn;
    unsigned long timer;
};

// Globale Variablen für die Relais
extern Relay relays[];
extern const int numRelays;


// Verzögerungszeit für die Relais
extern const unsigned long relayDelay;

void toggleRelay(int relayNumber);
void checkRelays();

