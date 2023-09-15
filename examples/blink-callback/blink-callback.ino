#include "Delay.h"

Delay printDelay(500);

void setup() {
    Serial.begin(9600);
}

void loop() {
    if (printDelay.isCompleted()) {
        Serial.print("Loop count: ");
        Serial.println(printer.getCount());
    }
}