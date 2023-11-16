#include "timesync.h"
#include <time.h>

void setupTime() {
    // Zeitzone auf MEZ setzen
    configTime(3600, 3600, "de.pool.ntp.org", "ptbtime1.ptb.de"); // UTC+1 

}
