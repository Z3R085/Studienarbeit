// schedule.cpp
#include "schedule.h"
#include "pump.h"


void WateringSchedule::addScheduleEntry(const ScheduleEntry& entry) {
  scheduleEntries.push_back(entry); // Fügt einen Eintrag zum Bewässerungsplan hinzu
}
// Aktiviert die Pumpe, wenn die aktuelle Zeit mit einem Eintrag im Bewässerungsplan übereinstimmt
void WateringSchedule::checkAndActivatePump(int pumpNumber) {
  time_t currentTime = std::time(nullptr);
  tm *currentTm = std::localtime(&currentTime);
    
  for (const auto& entry : scheduleEntries) {
    if ((currentTm->tm_wday == entry.dayOfWeek) && // aktuelle Tag, tm_wday = 0 = Sonntag, 1 = Montag, ..., 6 = Samstag
        isTimeOfDay(currentTm, entry.timeOfDay) && // aktuelle Zeit mit der Zeit des Eintrags übereinstimmt
        (entry.repeatWeekly || (entry.repeatMonthly && isFirstWeekOfMonth(currentTm)))) { // Prüfen, ob der Eintrag wiederholt werden soll
      
      if (currentTime - lastChecked >= 60) {
        togglepump(pumpNumber);
        lastChecked = currentTime;
      }
    }
  }
}
//Testen, ob die aktuelle Zeit mit einem Eintrag im Bewässerungsplan übereinstimmt
bool WateringSchedule::isTimeOfDay(tm *currentTm, TimeOfDay timeOfDay) {
  switch(timeOfDay) {
    case TimeOfDay::MORNING:
      return currentTm->tm_hour >= 6 && currentTm->tm_hour < 10; //  Rückgabe true, wenn die aktuelle Zeit zwischen 6 und 10 Uhr liegt
    case TimeOfDay::NOON:
      return currentTm->tm_hour >= 12 && currentTm->tm_hour < 15; //  Rückgabe true, wenn die aktuelle Zeit zwischen 12 und 15 Uhr liegt
    case TimeOfDay::EVENING:
      return currentTm->tm_hour >= 18 && currentTm->tm_hour < 22; // Rückgabe true, wenn die aktuelle Zeit zwischen 18 und 22 Uhr liegt
    default:
      return false;
  }
}
// Rückgabe true, wenn der aktuelle Tag im ersten Monat liegt
bool WateringSchedule::isFirstWeekOfMonth(tm *currentTm) {
  return currentTm->tm_mday <= 7;
}
