/**
 * @brief Provides a library for managing asynchronous delays and
 * timeouts in embedded systems.
 *
 * @file Delay.h
 *
 * @author boolscope
 * @version 1.2.0
 */
#ifndef _DELAY_H
#define _DELAY_H

// ULONG_MAX is not defined in Arduino.h,
// so we use the value from limits.h
#include <Arduino.h>
#include <limits.h>

/**
 * @brief Type definition for a callback function with no arguments and no
 * return value.
 *
 * This `CallbackFunction` type definition is used to represent a callback
 * function that takes no arguments and has no return value. It is commonly
 * employed when setting callback functions within the Delay class to execute
 * specific actions when a timer expires.
 */
typedef void (*CallbackFunction)();

/**
 * @brief This class facilitates creating non-blocking delays and timeouts.
 * @class Delay
 *
 * The Delay class allows you to create delays and timeouts that are
 * non-blocking, meaning your program can perform other tasks while waiting.
 * This improves the efficiency of the program, especially when running on
 * a single-threaded microcontroller environment.
 */
class Delay {
private:
    /**
     * @brief The number of times the Delay instance has been triggered.
     */
    unsigned long count = 0;

    /**
     * @brief The time interval (in milliseconds) after which the Delay
     * object becomes ready.
     */
    unsigned long interval = 0;

    /**
     * @brief The last time (in milliseconds) the Delay object was
     * triggered or initialized.
     */
    unsigned long timestamp = 0;

    /**
     * @brief The time (in milliseconds) the Delay object was suspended.
     */
    unsigned long suspendTime = 0;

    /**
     * @brief Stores the callback function to be invoked when the
     * timer expires.
     *
     * This member variable holds a pointer to a callback function that is
     * invoked when the timer expires.
     */
    CallbackFunction callbackFunction = nullptr;

public:
    /**
     * @brief Indicates whether the timer is active.
     *
     * The `isActive` flag is used to control the timer's activity status.
     * When set to `true`, the timer is active and will trigger based on
     * the configured delay interval. When set to `false`, the timer is
     * inactive and will not trigger, regardless of the interval setting.
     */
    bool isActive = true;

    /**
     * @brief Constructs a new Delay object.
     *
     * Initializes a new Delay object and optionally sets the delay
     * interval during object creation.
     *
     * @param[in] interval The delay time in milliseconds. Defaults to 0.
     * @param[in] isActive Indicates whether the timer is active.
     */
    Delay(unsigned long interval = 0, bool isActive = true);

    /**
     * @brief Destructor.
     *
     * Destroys the Delay object, performing any necessary cleanup.
     */
    ~Delay() = default;

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
    void enable();

    /**
     * @brief Disables the Delay object and cancels any active suspend state.
     *
     * This method sets the `isActive` flag to `false`, thereby disabling the
     * Delay object. Additionally, any active suspend state is cancelled, and
     * the `suspendTime` is set to 0.
     *
     * @note If the object is in a suspended state when this method is called,
     * the suspend state will be cancelled, but the timer will remain disabled.
     */
    void disable();

    /**
     * @brief Suspends the Delay object for a specified amount of time.
     *
     * This method suspends the Delay object for a specified amount of time
     * (in milliseconds). The object will not become active until the
     * specified amount of time has elapsed. Additionally, the timer's state
     * is reset, and the interval will start anew after the suspendTime has
     * elapsed.
     *
     * @note Calling suspend will reset the timer, effectively losing any time
     * that has already elapsed towards the next interval.
     *
     * @param[in] suspendTime The amount of time to suspend the Delay object,
     * in milliseconds.
     */
    void suspend(unsigned long suspendTime);

    /**
     * @brief Configures the delay interval for the Delay object.
     *
     * Sets the amount of time (in milliseconds) the Delay object
     * will wait before transitioning to an active state.
     *
     * @param[in] interval The desired delay time in milliseconds.
     */
    void setInterval(unsigned long interval);

    /**
     * @brief Retrieves the configured delay interval of the Delay obj.
     *
     * Gets the amount of time (in milliseconds) that the Delay object
     * will wait before becoming active.
     *
     * @return The configured delay time in milliseconds.
     */
    unsigned long getInterval();

    /**
     * @brief Sets the callback function for the timer.
     *
     * This method sets a callback function that will be invoked
     * when the timer reaches the 'done' state.
     *
     * @param[in] fn The callback function.
     */
    void setCallback(CallbackFunction fn);

    /**
     * @brief Checks if a callback function is set.
     *
     * This method returns true if a callback function has been set
     * for this timer.
     *
     * @return True if a callback function is set, false otherwise.
     */
    bool hasCallback();

    /**
     * @brief Retrieves the current callback function.
     *
     * This method returns the current callback function set for this timer.
     *
     * @return The current callback function.
     */
    CallbackFunction getCallback();

    /** @brief Executes the callback function.
     *
     * This method executes the callback function if it is set and the timer
     * has reached or exceeded the specified delay interval.
     *
     * @return True if the callback function was executed, false otherwise.
     */
    bool execCallback();

    /**
     * @brief Resets the internal timestamp to the current time.
     *
     * Updates the internal timestamp with the current time from millis().
     * This effectively resets any counting towards the next activation period.
     *
     * @return void
     */
    void resetTime();

    /**
     * @brief Calculates the time elapsed since the last reset.
     *
     * This function returns the difference, in milliseconds, between the
     * current system time (as obtained by millis()) and the internal timestamp
     * set by the last call to resetTime() or during object initialization.
     * This can be useful for understanding how close the object is to
     * transitioning to its 'ready' or 'done' state.
     *
     * @return The time difference in milliseconds.
     */
    unsigned long getDelta();

    /**
     * @brief Checks if the delay interval has expired.
     *
     * @retval true if the delay interval has expired.
     * @retval false otherwise.
     *
     * This function will not return `true` if the `isActive` is `false`.
     *
     * This function automatically reset the timer when has the `true` status.
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
     */
    bool isDone();

    /**
     * @brief Gets the number of times the object has become active.
     *
     * This function returns the count of times the object's delay interval
     * has expired.
     *
     * @return The number of times the object has been active.
     */
    unsigned long getCount();

    /**
     * @brief Resets the activation count to zero.
     *
     * Resets the internal counter that keeps track of the number of times
     * the object has become active.
     *
     * @return void
     */
    void resetCount();

    /**
     * @brief Checks if the object has been active an even number of times.
     *
     * @retval true If the internal counter is an even number.
     * @retval false Otherwise, including when the counter is zero.
     */
    bool isEven();

    /**
     * @brief Checks if the object has been active an odd number of times.
     *
     * @retval true If the internal counter is an odd number.
     * @retval false Otherwise, including when the counter is zero.
     */
    bool isOdd();

    /**
     * @brief Checks if the object has never been active.
     *
     * @retval true If the internal counter is zero, indicating the object
     * has never been ready.
     * @retval false otherwise.
     */
    bool isNever();
};

#endif  // _DELAY_H
