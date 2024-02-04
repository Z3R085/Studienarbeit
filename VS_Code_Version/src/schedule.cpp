#include "schedule.h"
#include "pump.h" 
#include <algorithm>
#include <chrono>
#include <thread>
#include <Arduino.h>
  

// Interne Liste von Bewässerungsereignissen
static std::vector<WateringEvent> events;

void addEvent(const WateringEvent& event) {
    //Altes Event löschen und neues hinzufügen
    events.erase(std::remove_if(events.begin(), events.end(), [&event](const WateringEvent& e) {
        return e.time.tm_hour == event.time.tm_hour && e.time.tm_min == event.time.tm_min;
    }), events.end());
    events.push_back(event);
    //In der Konsole ausgeben
    Serial.println("Event hinzugefügt");
    Serial.print("Zeit: ");
    Serial.print(event.time.tm_hour);
    Serial.print(":");
    Serial.println(event.time.tm_min);
    Serial.print("Dauer: ");
    Serial.println(event.duration);
    Serial.print("Wiederholung: ");
    switch (event.repeat) {
        case RepeatInterval::daily:
            Serial.println("Täglich");
            break;
        case RepeatInterval::weekly:
            Serial.println("Wöchentlich");
            break;
    }
    Serial.println("Tage:");
    for (bool day : event.daysOfWeek) {
        Serial.println(day);
    }
    Serial.println("Event hinzugefügt");

}

void checkAndRunEvents() {
    Serial.println("Überprüfe Ereignisse");
  // Aktuelle Zeit abrufen
  std::time_t now = std::time(0);
  tm* now_tm = std::localtime(&now);

  for (auto& event : events) {
    bool shouldRun = false;

    // Überprüfen, ob das Ereignis für heute geplant ist
    switch (event.repeat) {
      case RepeatInterval::daily:
        shouldRun = true;
        break;
      case RepeatInterval::weekly:
        shouldRun = event.daysOfWeek[now_tm->tm_wday];  //Schauen ob an Wochentag true oder false steht
        break;
    }

    // Überprüfen, ob die aktuelle Zeit mit der Zeit des Ereignisses übereinstimmt
    if (shouldRun && now_tm->tm_hour == event.time.tm_hour && now_tm->tm_min == event.time.tm_min) {
      // Die Pumpe für die Dauer des Ereignisses einschalten
      togglepump(1);
      Serial.println("Pumpe eingeschaltet");
      std::this_thread::sleep_for(std::chrono::seconds(event.duration));
      togglepump(1);
    }
  }


}
