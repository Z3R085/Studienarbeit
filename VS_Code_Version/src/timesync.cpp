#include "timesync.h"
#include <time.h>
#include <WiFi.h>

void setupTime()
{
    // Zeitzone auf MEZ setzen
    configTime(3600, 3600, "pool.ntp.org", "ptbtime1.ptb.de"); // UTC+1

    // Warte auf Zeit-Synchronisierung
    while (!time(nullptr))
    {
        delay(1000);
    }
}
