#pragma once
#include "pump.h"
#include <vector>
#include <ctime>

enum class RepeatInterval
{
  daily,
  weekly
};

struct WateringEvent
{
  tm time;
  int duration;
  RepeatInterval repeat;
  std::vector<bool> daysOfWeek;
};

// Deklaration der Funktionen für die Verwaltung der Bewässerungsereignisse
void addEvent(const WateringEvent &event);
void checkAndRunEvents();
