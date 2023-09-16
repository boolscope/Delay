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
Delay::Delay(unsigned long interval = 0, bool isActive = true)
    : interval(interval),
      isActive(isActive),
      timestamp(millis()) {
}

/**
 * @brief Enables the Delay object and cancels any active suspend state.
 *
 * This method sets the `isActive` flag to `true`, thereby enabling the
 * Delay object. It also calls the resetTime() method to reset the timer
 * to the current system time. Additionally, any active suspend state is
 * cancelled, and the `suspendTime` is set to 0.
 *
 * @note If the object is in a suspended state when this method is called,
 * the suspend state will be cancelled, and the timer will immediately
 * become active.
 */
void Delay::enable() {
    this->isActive = true;
    this->suspendTime = 0;
    this->suspendDelta = 0;
    this->resetTime();
}

/**
 * @brief Disables the Delay object and cancels any active suspend state.
 *
 * This method sets the `isActive` flag to `false`, thereby disabling the Delay
 * object. Additionally, any active suspend state is cancelled, and the
 * `suspendTime` is set to 0.
 *
 * @note If the object is in a suspended state when this method is called, the
 * suspend state will be cancelled, but the timer will remain disabled.
 */
void Delay::disable() {
    this->isActive = false;
    this->suspendTime = 0;
    this->suspendDelta = 0;
}

/**
 * @brief Suspends the Delay object for a specified amount of time.
 *
 * This method suspends the Delay object for a specified amount of time
 * (in milliseconds). The object will not become active again until the
 * specified amount of time has elapsed. Additionally, the timer's state
 * is reset, and the interval will start anew after the suspend time has
 * elapsed.
 *
 * @param[in] suspendTime The amount of time to suspend the Delay object,
 * in milliseconds.
 * @param[in] shouldContinue (Optional) If set to `true`, the timer will
 * continue counting from where it left off before being suspended. If set to
 * `false` (default), the timer will reset and count from the beginning of the
 * interval after suspension.
 *
 * @note Calling suspend with `continue` set to `false` will reset the timer,
 * effectively losing any time that has already elapsed towards the next
 * interval.
 */
void Delay::suspend(unsigned long suspendTime, bool shouldContinue = false) {
    // Save the time that the timer has already passed before calling the
    // suspend method.
    this->suspendDelta = shouldContinue ? this->getDelta() : 0;

    // Set the suspend time and disable the timer.
    this->suspendTime = suspendTime;
    this->isActive = false;
    this->resetTime();
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
 * `isOver()` returns true.
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
 * This method executes the callback function if it is set and the timer
 * has reached or exceeded the specified delay interval.
 *
 * @return `true` if the callback function was executed, `false` otherwise.
 */
bool Delay::execCallback() {
    bool result = false;
    if (this->isActive && this->hasCallback() && this->isOver()) {
        this->callbackFunction();
        result = true;
    }

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
 * This function does not automatically reset the timer when time over.
 * This is useful when you have a situation to leave the timer "open" after the
 * first event. The timer can be manually reset using the `resetTime` method.
 *
 * @code
 * Delay eventDelay(300);
 * if (eventDelay.isOver()) {
 *   // This code will be available continuously in all
 *   // iterations of the loop until `resetTime` is done.
 * }
 * @endcode
 *
 * The timer can be reset at the time you want.
 *
 * @code
 * Delay eventDelay(300);
 * if (eventDelay.isOver()) {
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
bool Delay::isOver() {
    if (!this->isActive && this->suspendTime == 0) {
        // If disabled and suspend time is 0, then the object is never done.
        return false;
    } else if (!this->isActive && this->suspendTime != 0) {
        // If disabled and suspend time is not 0, then the object is done
        // when the suspend time has elapsed.
        if (this->getDelta() >= this->suspendTime) {
            this->enable();
        }

        // Returns false because only the suspend time ended.
        return false;
    }

    // If the object is active, then the count is incremented.
    if (this->getDelta() >= (this->interval - this->suspendDelta)) {
        this->count++;
        this->suspendDelta = 0;
        return true;
    }

    return false;
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
    bool result = this->isOver();
    if (result) {
        this->resetTime();
    }

    return result;
}

/**
 * @brief Retrieves the count of times the object has been active.
 *
 * This method returns the number of times the object's timer has reached or
 * exceeded the specified delay interval. Essentially, it tells you how many
 * times either `isOver()` or `isCompleted()` has returned `true`.
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
