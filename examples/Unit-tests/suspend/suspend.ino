#include "Delay.h"

Delay continueDelay(5000);  // Object to test suspend() with continue == true
Delay resetDelay(5000);     // Object to test suspend() with continue == false (or no second parameter)

unsigned long prevTimeContinue = 0;  // The time of the last event for continueDelay
unsigned long prevTimeReset = 0;     // The time of the last event for resetDelay

unsigned long delta;  // To measure the time taken by Serial.print

void setup() {
  // Initialize serial port.
  Serial.begin(9600);
}

void loop() {
  // Testing with continue == false (default).
  if (resetDelay.isOver()) {
    Serial.println("resetDelay is done. Suspend for 2000ms without continue.");
    unsigned long preSerialTime = millis();
    resetDelay.suspend(2000);
    delta = millis() - preSerialTime;

    unsigned long currentTime = millis();
    unsigned long actualDeltaTime = currentTime - prevTimeReset;
    prevTimeReset = currentTime;

    unsigned long expectedDeltaTime = 5000 + 2000 + delta;
    Serial.print("Expected time between events: ");
    Serial.print(expectedDeltaTime);
    Serial.print("ms, Actual time: ");
    Serial.print(actualDeltaTime);
    Serial.println("ms");

    // At the first launch, the test score will be Error -
    // that's why we don't show it.
    if (resetDelay.getCount() > 1) {
      // Tolerance of 50ms.
      if (abs((long)(actualDeltaTime - expectedDeltaTime)) <= 50) {
        Serial.println("Result ...Ok\n\n");
      } else {
        Serial.println("Result ...Error\n\n");
      }
    } else {
      Serial.println("Result ...???\n\n");
    }
  }

  // Testing with continue == true.
  if (continueDelay.isOver()) {
    Serial.println("continueDelay is done. Suspend for 2000ms with continue.");
    unsigned long preSerialTime = millis();
    continueDelay.suspend(2000, true);
    delta = millis() - preSerialTime;

    unsigned long currentTime = millis();
    unsigned long actualDeltaTime = currentTime - prevTimeContinue;
    prevTimeContinue = currentTime;

    unsigned long expectedDeltaTime = 7000 + delta;
    Serial.print("Expected time between events: ");
    Serial.print(expectedDeltaTime);
    Serial.print("ms, Actual time: ");
    Serial.print(actualDeltaTime);
    Serial.println("ms");

    // At the first launch, the test score will be Error -
    // that's why we don't show it.
    if (continueDelay.getCount() > 1) {
      // Tolerance of 50ms.
      if (abs((long)(actualDeltaTime - expectedDeltaTime)) <= 50) {
        Serial.println("Result ...Ok\n\n");
      } else {
        Serial.println("Result ...Error\n\n");
      }
    } else {
      Serial.println("Result ...???\n\n");
    }
  }
}
