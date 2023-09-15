#include "Delay.h"

#define LED_1_PIN 12
#define LED_2_PIN 11

Delay led1Delay(500);
Delay led2Delay(750);

void setup() {
    pinMode(LED_1_PIN, OUTPUT);
    pinMode(LED_2_PIN, OUTPUT);
}

void loop() {
    // Non-blocking way to blink LED_1_PIN,
    // similar to Sketch 2 but more readable.
    if (led1Delay.isDone()) {
        bool isLow = digitalRead(LED_1_PIN) == LOW;
        digitalWrite(LED_1_PIN, isLow ? HIGH : LOW);
        led1Delay.resetTime();
    }

    // Non-blocking way to blink LED_2_PIN,
    // similar to Sketch 2 but more readable.
    if (led2Delay.isCompleted()) {
        bool isLow = digitalRead(LED_2_PIN) == LOW;
        digitalWrite(LED_2_PIN, isLow ? HIGH : LOW);
        led2Delay.resetTime();
    }
}