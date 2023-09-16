[![Stay with Ukraine](https://img.shields.io/static/v1?label=Stay%20with&message=Ukraine%20♥&color=ffD700&labelColor=0057B8&style=flat)](https://u24.gov.ua/)

# Delay

The Delay library provides the simplest tools for implementing asynchronous delays and timeouts in the runtime environment of microcontrollers.


## Why Delay?

- **Non-Blocking:** Implement delays without freezing the rest of your code. Continue to read sensors, control motors, and more.
- **Easy-to-Use:** With a few lines of code, transform your project into a multitasking powerhouse.
- **Precision Timing:** Whether you're flashing an LED or activating a servo, do it exactly when you want to.
- **Event-Driven:** Easily build logic around asynchronous events.
- **Resource-Efficient:** Keep your microcontroller running smoothly with optimized code that won't hog your precious memory or CPU cycles.

## Features

- Interval-based triggering with millisecond precision.
- Automatic and manual timer resets.
- Counter to track the number of completed delays.
- Advanced methods for more complex timing logic, such as even/odd checks and more.

## Theory

Microcontrollers are often used in environments that require multitasking, where multiple operations or processes have to be managed simultaneously. This could be as simple as blinking two LEDs at different intervals or as complex as managing sensors, actuators, and communications all at once.

The delay() function is a simple and straightforward way to introduce pauses or delays in the code. While it's beginner-friendly, it's not efficient for multitasking applications. When delay() is called, it blocks the entire program, making the CPU idle and unavailable to perform other tasks. This is often referred to as "busy-waiting."

For example, if you use delay() to blink an LED for 500 milliseconds, you can't easily do anything else during that time period. The CPU will be stuck in that delay, unable to read sensors, communicate data, or perform any other tasks.

This blocking behavior severely limits the microcontroller's ability to manage multiple tasks and respond to real-time events. Therefore, delay() is often considered a bad practice for anything beyond the simplest projects.

For these reasons, alternative methods such as using millis() for non-blocking delays or more advanced techniques like state machines, timers, or libraries like Delay are recommended. These methods allow your program to multitask, making efficient use of the microcontroller's processing power.

For example, the simplest task is to flash two LEDs so that the first one is on and off every 500 milliseconds, the other every 750 milliseconds.

### Sketch 1.
Using delay().

It is not possible to make two LEDs blink in parallel just by using the `delay()` function without additional algorithms for checking the total delay and repeating the code.

```cpp
#define LED_1_PIN 12
#define LED_2_PIN 11

void setup() {
  pinMode(LED_1_PIN, OUTPUT);
  pinMode(LED_2_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_1_PIN, HIGH);
  delay(500);
  digitalWrite(LED_1_PIN, LOW);
  delay(500);

  // Problem: Cannot blink LED_2_PIN in parallel with LED_1_PIN because
  // the delay() function is blocking the program execution. This makes
  // LED_1_PIN and LED_2_PIN blink sequentially instead of simultaneously.

  digitalWrite(LED_2_PIN, HIGH);
  delay(750);
  digitalWrite(LED_2_PIN, LOW);
  delay(750);

  // A total of 2500 milliseconds, during which time the LED_1_PIN should
  // have lit up 3 times, but only once due to blocking.
  // ...
}
```

### Sketch 2.

Using the `millis()` function can solve this problem. But it will required additional control over intervals, their manual verification and declaration of additional variables. This approach is fine for small programs, and overly complicated for large programs.

```cpp
#define LED_1_PIN 12
#define LED_2_PIN 11

unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;

#define INTERVAL_1 500
#define INTERVAL_2 750

void setup() {
  pinMode(LED_1_PIN, OUTPUT);
  pinMode(LED_2_PIN, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis();

  // Non-blocking way to blink LED_1_PIN, solving the problem in Sketch 1.
  if (currentMillis - previousMillis1 >= INTERVAL_1) {
    previousMillis1 = currentMillis;
    bool isLow = digitalRead(LED_1_PIN) == LOW;
    digitalWrite(LED_1_PIN, isLow ? HIGH : LOW);
  }

  // Non-blocking way to blink LED_2_PIN, solving the problem in Sketch 1.
  if (currentMillis - previousMillis2 >= INTERVAL_2) {
    previousMillis2 = currentMillis;
    bool isLow = digitalRead(LED_2_PIN) == LOW;
    digitalWrite(LED_2_PIN, isLow ? HIGH : LOW);
  }
}

```

### Sketch 3.

Using Delay allows you to focus on the implementation of the algorithm specifically for the application being developed, and not on delay control.

```cpp
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
  if (led1Delay.isOver()) {
    bool isLow = digitalRead(LED_1_PIN) == LOW;
    digitalWrite(LED_1_PIN, isLow ? HIGH : LOW);
  }

  // Non-blocking way to blink LED_2_PIN,
  // similar to Sketch 2 but more readable.
  if (led2Delay.isOver()) {
    bool isLow = digitalRead(LED_2_PIN) == LOW;
    digitalWrite(LED_2_PIN, isLow ? HIGH : LOW);
  }
}

```

In addition, the Delay library allows you to determine, for example, whether the ready state was reached at least once `isNever()` method; whether it is an even/odd state of readiness of `isEven()` and `isOdd()` methods; the number of times the state of readiness was reached `getCount()` method, etc.
