// schedule.h
#include <vector>
#include <ctime>


// Klasse für die Zeit des Tages
enum class TimeOfDay {
    MORNING,
    NOON,
    EVENING
};
// Klasse für einen Eintrag im Bewässerungsplan
struct ScheduleEntry {
    int dayOfWeek;  // 0 = Sonntag, 1 = Montag, ..., 6 = Samstag
    TimeOfDay timeOfDay;
    bool repeatWeekly;
    bool repeatMonthly;

  ScheduleEntry(int dow, TimeOfDay tod, bool weekly, bool monthly)
    : dayOfWeek(dow), timeOfDay(tod), repeatWeekly(weekly), repeatMonthly(monthly) {}
};

//Klasse für den Bewässerungsplan
class WateringSchedule {
 public:
    void addScheduleEntry(const ScheduleEntry& entry);
    void checkAndActivatePump(int pumpNumber);
    bool isTimeOfDay(tm *currentTm, TimeOfDay timeOfDay);
    bool isFirstWeekOfMonth(tm *currentTm);

 private:
    std::vector<ScheduleEntry> scheduleEntries;
    time_t lastChecked;
};
