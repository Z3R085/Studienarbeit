#include <Arduino.h>
void setuppumps(); // Funktion zum Setup der Pumpen-Pins

// Pinbelegung
#define pump1_PIN 4
#define pump2_PIN 5

struct pump {
    int pin; // Pin, an dem die Pumpe angeschlossen ist
    bool isOn; // Zustand des pump (ein- oder ausgeschaltet)
    unsigned long timer; // Zeit, zu der die Pumpe ausgeschaltet werden soll
    String lastActivated; // Zeit, zu der die Pumpe zuletzt eingeschaltet wurde
};

// Globale Variablen fuer die Pumpe
extern pump pumps[]; // Array mit den Pumpen
extern const int numpumps; // Anzahl der Pumpen

extern const unsigned long pumpDelay; // Verzögerung, nach der die Pumpe ausgeschaltet werden soll

void togglepump(int pumpNumber); // Funktion zum Umschalten eines pump
void checkpumps(); // Funktion zum ueberpruefen, ob die Verzögerungszeit fuer die Pumpe abgelaufen ist. Wenn ja, schalte die Pumpe aus.


