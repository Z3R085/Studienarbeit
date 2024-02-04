#pragma once
#include "pump.h"
#include <vector>
#include <ctime>

enum class RepeatInterval {
  DAILY, // Täglich
  SPECIFIC_DAYS, // An bestimmten Tagen der Woche
  MONTHLY // Monatlich
};

// Eine Struktur zur Darstellung eines einzelnen Bewässerungsereignisses
struct WateringEvent {
  tm time; // Die Zeit, zu der gegossen werden soll
  int duration; // Dauer in Sekunden, wie lange die Pumpe laufen soll
  RepeatInterval repeat; // Wiederholungsintervall
  std::vector<bool> daysOfWeek; // Ein Vektor von 7 boolschen Werten, die die Tage der Woche darstellen, an denen das Ereignis stattfinden soll
};

// Klasse zur Verwaltung der Bewässerungszeitpläne
class ScheduleManager {
public:
  void addEvent(const WateringEvent& event); // Fügt ein neues Bewässerungsereignis hinzu
  void checkAndRunEvents(); // Überprüft, ob ein Ereignis ausgelöst werden soll und führt es aus

private:
  std::vector<WateringEvent> events; // Speichert die Bewässerungsereignisse
};


