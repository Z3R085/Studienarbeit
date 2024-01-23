#include "schedule.h"
#include "pump.h" //Wird benötigt, um die Pumpe zu aktivieren.

// Konstruktor, initialisiert das Objekt ohne einen Zeitplaneintrag.
WateringSchedule::WateringSchedule() : currentScheduleEntry(nullptr) {}

// Destruktor, sorgt für die Freigabe des Speichers des Zeitplaneintrags.
WateringSchedule::~WateringSchedule() {
    delete currentScheduleEntry; // Löscht den zugewiesenen Zeitplaneintrag, um Speicherlecks zu vermeiden.
}

// Funktion zum Setzen eines neuen Zeitplaneintrags.
void WateringSchedule::setScheduleEntry(const ScheduleEntry& entry) {
    delete currentScheduleEntry; // Löscht den vorhandenen Zeitplaneintrag, falls einer vorhanden ist.
    currentScheduleEntry = new ScheduleEntry(entry); // Erstellt und speichert einen neuen Zeitplaneintrag.
}

// Funktion, die regelmäßig aufgerufen werden sollte, um zu prüfen, ob die Pumpe aktiviert werden muss.
void WateringSchedule::checkAndActivatePump() {
    serial.println("checkAndActivatePump");
    if (!currentScheduleEntry) return; // Wenn kein Zeitplaneintrag vorhanden ist, tue nichts.

    time_t currentTime = std::time(nullptr); // Aktuelle Zeit in Sekunden seit dem Unix-Epoch.
    tm* currentTm = std::localtime(&currentTime); // Konvertiert die Zeit in eine tm-Struktur für die lokale Zeitzone.

    // Prüft, ob es Zeit ist, den Zeitplaneintrag auszuführen.
    if (isTimeForWatering(*currentScheduleEntry, *currentTm)) {
        // Stellt sicher, dass die Pumpe nicht mehr als einmal pro Minute aktiviert wird.
        if (currentTime - lastChecked >= 60) {
            togglepump(1); // Aktiviert die Pumpe (angenommen, es gibt eine entsprechende Funktion in pump.h).
            lastChecked = currentTime; // Aktualisiert die Zeit der letzten Prüfung/Aktivierung.
        }
    }
}

// Hilfsfunktion, die prüft, ob die aktuellen Bedingungen mit dem Zeitplaneintrag übereinstimmen.
bool WateringSchedule::isTimeForWatering(const ScheduleEntry& entry, const tm& currentTm) {
    // Wenn die Bewässerung wöchentlich wiederholt wird und der aktuelle Wochentag nicht in der Liste ist, gib 'false' zurück.
    if (entry.repeatWeekly && std::find(entry.daysOfWeek.begin(), entry.daysOfWeek.end(), currentTm.tm_wday) == entry.daysOfWeek.end()) {
        return false; // Heutiger Tag nicht in der Liste der Wochentage für die Bewässerung.
    }

    // Wenn die Bewässerung monatlich ist und es nicht die erste Woche des Monats ist, gib 'false' zurück.
    if (entry.repeatMonthly && !isFirstWeekOfMonth(currentTm)) {
        return false; // Es ist nicht die erste Woche des Monats.
    }

    // Prüft die Tageszeit gegen die geplanten Zeiten.
    if (entry.timeOfDay == "morning" && (currentTm.tm_hour >= 6 && currentTm.tm_hour < 12)) {
        serial.println("morning");
        return true; // Es ist Morgen.
    }
    if (entry.timeOfDay == "noon" && (currentTm.tm_hour >= 12 && currentTm.tm_hour < 17)) {
        serial.println("noon");
        return true; // Es ist Mittag.
    }
    if (entry.timeOfDay == "evening" && (currentTm.tm_hour >= 17 && currentTm.tm_hour < 22)) {
        serial.println("evening");
        return true; // Es ist Abend.
    }
    serial.println("Keine Zeit zum gießen");
    return false; // Keine der Bedingungen wurde erfüllt, daher ist es nicht Zeit für die Bewässerung.
}

// Funktion, die prüft, ob das aktuelle Datum in der ersten Woche des Monats liegt.
bool WateringSchedule::isFirstWeekOfMonth(const tm* currentTm) {
    return currentTm->tm_mday <= 7; // Wenn der Tag des Monats 7 oder weniger ist, sind wir in der ersten Woche.
}
