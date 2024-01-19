#include <Arduino.h>
void setuppumps(); // Funktion zum Setup der pumpn-Pins

// Pinbelegung
#define pump1_PIN 4
#define pump2_PIN 5

struct pump {
    int pin; // Pin, an dem die pump angeschlossen ist
    bool isOn; // Zustand des pump (ein- oder ausgeschaltet)
    unsigned long timer; // Zeit, zu der die pump ausgeschaltet werden soll
    String lastActivated; // Zeit, zu der die pump zuletzt eingeschaltet wurde
    unsigned long duration; // Dauer, wie lange die Pumpe laufen soll
};

// Enum fuer die Bewaesserungsmodi
enum class WateringMode {
  MANUAL, // Manuell durch Benutzer gesteuert
  SCHEDULED, // Gießen nach einem festgelegten Zeitplan
  SENSOR_BASED // Gießen basierend auf Sensordaten
};



// Globale Variablen fuer die pump
extern pump pumps[]; // Array mit den pumpn
extern const int numpumps; // Anzahl der pumpn
extern WateringMode currentMode; // Aktueller Bewaesserungsmodus


extern const unsigned long pumpDelay; // Verzögerung, nach der die pump ausgeschaltet werden soll

void togglepump(int pumpNumber); // Funktion zum Umschalten eines pump
void checkpumps(); // Funktion zum ueberpruefen, ob die Verzögerungszeit fuer die pump abgelaufen ist. Wenn ja, schalte die pump aus.
void setPumpDuration(int pumpNumber, unsigned long duration); // Funktion zum Setzen der Dauer, wie lange die Pumpe laufen soll
void setWateringMode(WateringMode mode); // Funktion zum Setzen des Bewaesserungsmodus
WateringMode getWateringMode(); // Funktion zum Abrufen des Bewaesserungsmodus


