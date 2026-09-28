#include <Arduino.h>

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println();
    Serial.println("==============================");
    Serial.println(" RailDrone Control System");
    Serial.println(" Firmware boot successful");
    Serial.println("==============================");
}

void loop() {
    Serial.println("RailDrone alive");
    delay(1000);
}
