#include "relay.h"

// Initialisieren der Relais-Strukturen
const int numRelays = 2; // Anzahl der Relais
Relay relays[numRelays] = {
    {RELAIS1_PIN, false, 0},
    {RELAIS2_PIN, false, 0}
};


// Verzögerungszeit für die Relais
const unsigned long relayDelay = 2500; // 2.5 Sekunden

/*
toggleRelay(int relayNumber) schaltet das Relais mit der übergebenen Nummer um.
*/
void toggleRelay(int relayNumber) {
    // Zugriff auf das richtige Relais basierend auf der übergebenen Nummer
    Relay &relay = relays[relayNumber - 1]; // Array-Index ist relayNumber - 1

    // Umschalten des Relais und Speichern des aktuellen Zeitstempels
    digitalWrite(relay.pin, !digitalRead(relay.pin));
    relay.isOn = !relay.isOn;
    if (relay.isOn) {
        relay.timer = millis(); 
        relay.lastActivated = millis(); // Speichern des aktuellen Zeitstempels
    }
}

/*  checkRelays überprüft den Status jedes Relais in der relays-Array.
    Diese Funktion wird in der loop() regelmäßig aufgerufen, um zu prüfen, ob es Zeit ist,
    ein eingeschaltetes Relais auszuschalten.
 */
void checkRelays() {
    // Durchlaufe jedes Relais in der relays-Array
    for (int i = 0; i < sizeof(relays)/sizeof(relays[0]); ++i) {
        // Prüfe, ob das aktuelle Relais eingeschaltet ist UND die festgelegte Verzögerungszeit abgelaufen ist.
        if (relays[i].isOn && (millis() - relays[i].timer >= relayDelay)) {
            // Wenn ja, schalte das Relais aus.
            digitalWrite(relays[i].pin, LOW);
            // Aktualisiere den Status des Relais in der Struktur, um anzuzeigen, dass es jetzt aus ist.
            relays[i].isOn = false;
        }
    }
}



