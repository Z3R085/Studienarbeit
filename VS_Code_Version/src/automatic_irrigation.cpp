#include "automatic_irrigation.h"
#include <sensor.h>
#include <pump.h>
void automaticIrrigation() {
    // Auslesen der Sensorwerte
    float tankLevel = getTankLevel(); //Muss noch eingebaut und implementiert werden
    float temperature = getTemperature(); //Muss noch eingebaut und implementiert werden
    float soilMoisture = getSoilMoisture();

    // Entscheidungsbaum 
    if (tankLevel > 25) {
        if (temperature < 40) {
            if (soilMoisture < 90) {
                togglepump(1);
            } else {
                togglepump(1);
            }
        } else {
            togglepump(1);
        }
    } else {
        togglepump(1);
    }
}