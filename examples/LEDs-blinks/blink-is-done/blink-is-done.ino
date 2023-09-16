#include "Delay.h"

// Pins where the LEDs are connected.
#define LED_1_PIN 12
#define LED_2_PIN 11

// Delay for changing the state of the LEDs.
Delay led1Delay(500);
Delay led2Delay(750);

// Fix the real time between function calls.
unsigned long led1StartTime, led2StartTime;

// Initialization.
void setup() {
  Serial.begin(9600);

  pinMode(LED_1_PIN, OUTPUT);
  pinMode(LED_2_PIN, OUTPUT);

  led1Delay.setCallback(toggleLed1);
  led2Delay.setCallback(toggleLed2);
}

// Event loop.
void loop() {
  // Each LED changes its state according to
  // the interval specified in its delay.
  if (led1Delay.isOver()) {
    toggleLed1();
  }

  if (led2Delay.isOver()) {
    toggleLed2();
  }
}

// Changing the status of the LED 1.
void toggleLed1() {
  Serial.print("LED 1, difference between previous run (ms): ");
  Serial.print(millis() - led1StartTime); // We can lose a few milliseconds
                                          // in the work of Serial Monitor.
  Serial.print(" / ");
  Serial.println(led1Delay.getInterval());
  led1StartTime = millis();

  bool isLow = digitalRead(LED_1_PIN) == LOW;
  digitalWrite(LED_1_PIN, isLow ? HIGH : LOW);
}

// Changing the status of the LED 2.
void toggleLed2() {
  Serial.print("LED 2, difference between previous run (ms): ");
  Serial.print(millis() - led2StartTime); // We can lose a few milliseconds
                                          // in the work of Serial Monitor.
  Serial.print(" / ");
  Serial.println(led2Delay.getInterval());
  led2StartTime = millis();

  bool isLow = digitalRead(LED_2_PIN) == LOW;
  digitalWrite(LED_2_PIN, isLow ? HIGH : LOW);
}