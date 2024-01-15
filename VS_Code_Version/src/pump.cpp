#include "pump.h"

// Initialisieren der pumpn-Strukturen
const int numpumps = 2; // Anzahl der pumpn
pump pumps[numpumps] = {
    {pump1_PIN, false, 0,"0",0},
    {pump2_PIN, false, 0,"0",0}
};

//Setup fuer pump-Pins
void setuppumps() {
    for (int i = 0; i < numpumps; ++i) {
        pinMode(pumps[i].pin, OUTPUT);
        digitalWrite(pumps[i].pin, LOW);
    }
}


// Verzögerungszeit fuer die pump
const unsigned long pumpDelay = 2500; // 2.5 Sekunden

// Setzt die Dauer, wie lange die Pumpe laufen soll.
void setPumpDuration(int pumpNumber, unsigned long duration) {
    pumps[pumpNumber - 1].duration = duration;
}

/*
togglepump(int pumpNumber) schaltet die pump mit der uebergebenen Nummer um.
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

/*  checkpumps ueberprueft den Status jedes pump in der pumpns-Array.
    Diese Funktion wird in der loop() regelmäßig aufgerufen, um zu pruefen, ob es Zeit ist,
    ein eingeschaltetes pump auszuschalten.
 */
void checkpumps() {
    // Durchlaufe jedes pump in der pumpns-Array
    for (int i = 0; i < sizeof(pumps)/sizeof(pumps[0]); ++i) {
        // Pruefe, ob das aktuelle pump eingeschaltet ist UND die festgelegte Verzögerungszeit abgelaufen ist.
        if (pumps[i].isOn && (millis() - pumps[i].timer >= pumps[i].duration)) {
            // Wenn ja, schalte die pump aus.
            digitalWrite(pumps[i].pin, LOW);
            // Aktualisiere den Status des pump in der Struktur, um anzuzeigen, dass es jetzt aus ist.
            pumps[i].isOn = false;
            

        }
    }
}



