#include "Delay.h"

/**
 * @brief Constructs a new Delay object and sets its interval.
 *
 * The constructor calls the setInterval method, initializing the delay
 * interval as well as resetting the internal timestamp.
 *
 * @param[in] interval The delay time in milliseconds. Defaults to 0.
 * @param[in] isActive Indicates whether the timer is active.
 */
Delay::Delay(unsigned long interval, bool isActive) {
    this->isActive = isActive;
    setInterval(interval);
}

/**
 * @brief Sets the delay interval for the Delay object and resets the
 * timer.
 *
 * This method updates the delay interval for the object and automatically
 * resets its internal timestamp to the current system time.
 *
 * @param[in] interval The new delay time in milliseconds.
 */
void Delay::setInterval(unsigned long interval) {
    // Zero is a valid value - the delay never happens (is always ready).
    // A negative number cannot be set, because the unsigned long type is used.
    // The value cannot be greater than DELAY_MAX_INTERVAL,
    if (interval > ULONG_MAX - 1) {
        interval = ULONG_MAX - 1;
    } else {
        this->interval = interval;
    }

    this->resetTime();
}

/**
 * @brief Returns the delay interval of the Delay object.
 *
 * This method retrieves the delay interval, in milliseconds, that the
 * object is configured to wait.
 *
 * @return The configured delay time in milliseconds.
 */
unsigned long Delay::getInterval() {
    return this->interval;
}

/**
 * @brief Sets the callback function to be executed when the delay
 * interval is reached.
 *
 * The provided function will be called automatically when the method
 * `isDone()` returns true.
 *
 * @param[in] fn The function to be called as a callback.
 */
void Delay::setCallback(CallbackFunction fn) {
    this->callbackFunction = fn;
}

/**
 * @brief Checks if a callback function has been set.
 *
 * This method returns `true` if a callback function has been set using the
 * `setCallback` method, `false` otherwise.
 *
 * @return `true` if a callback function exists, `false` otherwise.
 */
bool Delay::hasCallback() {
    return this->callbackFunction != nullptr;
}

/**
 * @brief Retrieves the current callback function.
 *
 * This method returns the current callback function set for this Delay
 * object. If no callback function has been set, it returns nullptr.
 *
 * @return The current callback function or nullptr if no callback function
 * has been set.
 */
CallbackFunction Delay::getCallback() {
    return this->hasCallback() ? this->callbackFunction : nullptr;
}

/**
 * @brief Executes the callback function.
 *
 * This method executes the callback function if it is set.
 *
 * In any case (regardless of whether the callback is set or not),
 * it performs the resetTime function.
 *
 * If you don't need to reset the timer after the callback is executed,
 * use the `if' construct with the `isDone()` method.
 *
 * @return `true` if the callback function was executed, `false` otherwise.
 */
bool Delay::execCallback() {
    bool result = false;
    if (this->isActive && this->hasCallback()) {
        this->callbackFunction();
        result = true;
    }

    this->resetTime();
    return result;
}

/**
 * @brief Resets the internal timestamp to the current system time.
 *
 * This method updates the internal timestamp to the current system time,
 * effectively resetting the timer.
 */
void Delay::resetTime() {
    this->timestamp = millis();
}

/**
 * @brief Calculates the elapsed time since the last timestamp update.
 *
 * This method returns the elapsed time, in milliseconds, since the last
 * timestamp update. It accounts for the rollover behavior of Arduino's
 * millis() function.
 *
 * @note The millis() function resets to zero approximately every 49.7 days.
 *
 * @return The elapsed time in milliseconds.
 */
unsigned long Delay::getDelta() {
    // The millis method resets to zero when the ULONG_MAX range is reached.
    // Therefore, if the last fixation of time was close to the moment of
    // resetting the delta can get a value with a very large range
    // close to ULONG_MAX.
    //
    // We can determine the overflow of millis (approximately every 50 days) -
    // if millis is less than the timestamp.
    unsigned long m = millis();
    return m < this->timestamp ? ULONG_MAX - this->timestamp + m
                               : m - this->timestamp;
}

/**
 * @brief Checks if the delay interval has been reached or exceeded.
 *
 * * This function automatically reset the timer when has the `true` status.
 * This is useful when you have short and quick code that will be executed
 * when a state is reached.
 *
 * @code
 * Delay eventDelay(300);
 * if (eventDelay.isDone()) {
 *   // Short and quick code here...
 * }
 * @endcode
 *
 * For large and slow code, you need to manually update the timer after
 * executing large code.
 *
 * @code
 * Delay eventDelay(300);
 * if (eventDelay.isDone()) {
 *   // Long-running code here...
 *   eventDelay.resetTime();  // manual reset
 * }
 * @endcode
 *
 * @note Never returns true if the `isActive` is `false`.
 *
 * @return `true` if the delay interval is reached or exceeded,
 * `false` otherwise.
 */
bool Delay::isDone() {
    // If the interval is set to zero, the "ready" state can never occur.
    if (!this->isActive) {
        return false;
    }

    // If the object is active, then the count is incremented.
    if (this->getDelta() >= this->interval) {
        this->count++;
        this->resetTime();
        return true;
    }

    return false;
}

/**
 * @brief Retrieves the count of times the object has been active.
 *
 * This method returns the number of times the object's timer has reached or
 * exceeded the specified delay interval. Essentially, it tells you how many
 * times either `isDone()` or `isCompleted()` has returned `true`.
 *
 * @return The count of times the object has been active.
 */
unsigned long Delay::getCount() {
    return this->count;
}

/**
 * @brief Resets the activation count to zero.
 *
 * This method resets the internal counter that keeps track of how many
 * times the object has been active (i.e., the timer has reached or exceeded
 * the specified delay interval).
 */
void Delay::resetCount() {
    this->count = 0;
}

/**
 * @brief Checks if the count of activations is even.
 *
 * This method returns `true` if the internal counter is an even
 * number and non-zero.
 *
 * @note Always returns `false` if the counter is zero.
 *
 * @return `true` if the count is even and non-zero, `false` otherwise.
 */
bool Delay::isEven() {
    return this->count % 2 == 0 && this->count != 0;
}

/**
 * @brief Checks if the count of activations is odd.
 *
 * This method returns `true` if the internal counter is an odd number and
 * non-zero.
 *
 * @note Always returns `false` if the counter is zero.
 *
 * @return `true` if the count is odd and non-zero, `false` otherwise.
 */
bool Delay::isOdd() {
    return this->count % 2 != 0 && this->count != 0;
}

/**
 * @brief Checks if the object has never been active.
 *
 * This method returns `true` if the internal counter is zero, indicating that
 * the object has never reached or exceeded the specified delay interval.
 *
 * @return `true` if the object has never been active, `false` otherwise.
 */
bool Delay::isNever() {
    return this->count == 0;
}
