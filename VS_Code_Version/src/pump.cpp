#include "pump.h"

// Initialisieren der pumpn-Strukturen
const int numpumps = 1; // Anzahl der Pumpen
pump pumps[numpumps] = {
    {pump1_PIN, false, 0,"0",2500},
    //{pump2_PIN, false, 0,"0",2500}
};

//Setup fuer Pumpen-Pins
void setuppumps() {
    for (int i = 0; i < numpumps; ++i) {
        pinMode(pumps[i].pin, OUTPUT);
        digitalWrite(pumps[i].pin, LOW);
    }
}

WateringMode currentMode = WateringMode::MANUAL; // Startet im manuellen Modus




// Setzt die Dauer, wie lange die Pumpe laufen soll.
void setPumpDuration(int pumpNumber, unsigned long duration) {
    pumps[pumpNumber - 1].duration = duration;
}

/*
togglepump(int pumpNumber) schaltet die Pumpe mit der uebergebenen Nummer um.
*/
void togglepump(int pumpNumber) {
    // Zugriff auf die richtige pump basierend auf der uebergebenen Nummer
    pump &pump = pumps[pumpNumber - 1]; // Array-Index ist pumpNumber - 1

    // Umschalten des pump und Speichern des aktuellen Zeitstempels
    digitalWrite(pump.pin, !digitalRead(pump.pin));
    pump.isOn = !pump.isOn;
    if (pump.isOn) {
        pump.timer = millis(); 
         // Speichern der aktuellen Uhrzeit als String
        time_t now = time(nullptr);
        struct tm *timeinfo = localtime(&now);
        char buffer[26];
        strftime(buffer, sizeof(buffer), "%H:%M:%S %d.%m.%Y ", timeinfo);
        pump.lastActivated = String(buffer);
        
    }
}

/*  checkpumps ueberprueft den Status jeder Pumpe in der pumps-Array.
    Diese Funktion wird in der loop() regelmäßig aufgerufen, wenn der Gießmodi auf MANUAL steht, um zu pruefen, ob es Zeit ist,
    eine eingeschaltete Pumpe auszuschalten.
 */
void checkpumps() {
    // Durchlaufe jede Pumpe in der pumps-Array
    for (int i = 0; i < sizeof(pumps)/sizeof(pumps[0]); ++i) {
        // Pruefe, ob die aktuelle Pumpe eingeschaltet ist UND die festgelegte Verzögerungszeit abgelaufen ist.
        if (pumps[i].isOn && (millis() - pumps[i].timer >= pumps[i].duration)) {
            // Wenn ja, schalte die Pumpe aus.
            digitalWrite(pumps[i].pin, LOW);
            // Aktualisiere den Status der Pumpe in der Struktur, um anzuzeigen, dass es jetzt aus ist.
            pumps[i].isOn = false;
            

        }
    }
}

// Setzt den Bewaesserungsmodus
void setWateringMode(WateringMode mode) {
    currentMode = mode;
}

//Gibt den Bewaesserungsmodus zurueck
WateringMode getWateringMode() {
    return currentMode;
}



