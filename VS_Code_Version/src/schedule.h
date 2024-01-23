#pragma once
#include <ctime>
#include "pump.h"

class ScheduleEntry {
public:
    std::vector<int> daysOfWeek; // 0 = Montag, 1 = Dienstag, ..., 6 = Sonntag
    std::string timeOfDay; // "morning", "noon", "evening"
    bool repeatWeekly;
    bool repeatMonthly;

    // Konstruktor mit Initialisierungsliste 
    ScheduleEntry(const std::vector<int>& dow, const std::string& tod, bool weekly, bool monthly)
        : daysOfWeek(dow), timeOfDay(tod), repeatWeekly(weekly), repeatMonthly(monthly) {}
};

class WateringSchedule {
public:
    void setScheduleEntry(const ScheduleEntry& entry);
    void checkAndActivatePump();

private:
    ScheduleEntry* currentScheduleEntry; // Zeiger auf das aktuelle ScheduleEntry-Objekt
    time_t lastChecked = 0;
    bool isTimeForWatering(const ScheduleEntry& entry, const tm& currentTm);
    bool isFirstWeekOfMonth(const tm* currentTm);
};
