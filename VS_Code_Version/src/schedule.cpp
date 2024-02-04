#include "schedule.h"
#include "pump.h" 
#include <algorithm>
#include <chrono>
#include <thread>
#include <Arduino.h>
  



void ScheduleManager::addEvent(const WateringEvent& event) {
  events.push_back(event);
}

void ScheduleManager::checkAndRunEvents() {
  // Aktuelle Zeit abrufen
  std::time_t now = std::time(0);
  tm* now_tm = std::localtime(&now);

  for (auto& event : events) {
    bool shouldRun = false;

    // Überprüfen, ob das Ereignis für heute geplant ist
    switch (event.repeat) {
      case RepeatInterval::DAILY:
        shouldRun = true;
        break;
      case RepeatInterval::SPECIFIC_DAYS:
        shouldRun = event.daysOfWeek[now_tm->tm_wday];
        break;
      case RepeatInterval::MONTHLY:
        shouldRun = (now_tm->tm_mday == event.time.tm_mday);
        break;
    }

    // Überprüfen, ob die aktuelle Zeit mit der Zeit des Ereignisses übereinstimmt
    if (shouldRun && now_tm->tm_hour == event.time.tm_hour && now_tm->tm_min == event.time.tm_min) {
      // Die Pumpe für die Dauer des Ereignisses einschalten
      togglePump(1);
      Serial.println("Pumpe eingeschaltet");
      std::this_thread::sleep_for(std::chrono::seconds(event.duration));
      togglePump(1);
    }
  }
}
