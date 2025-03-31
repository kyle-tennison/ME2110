#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>

#define MAX_TIMER_COUNT 16 // Number of timers to allocate in memory

/// @brief A callback function registered to go off at a certain point
class Timer {
public:
    uint32_t expiration_time;
    void (*callback)();
    bool activated;

    Timer(uint32_t _expiration_time, void (*_callback)());

    /// @brief Schedule a timer to go off in the future
    /// @param in_millis The number of milliseconds in the future to wait for the timer
    /// @param callback The function to call upon the timer expiring
    /// @return A constructed `Timer` object
    static Timer schedule(uint32_t in_millis, void (*callback)());
};


/// @brief Manages and polls multiple timers
class TimerManager {
public:
    static Timer* timers[MAX_TIMER_COUNT];

    /// @brief Register a timer in the list of timers
    /// @param timer The timer to register
    static void register_timer(Timer* timer);

    /// @brief Poll the timers registered in the loop
    static void poll_timers();
};

#endif // TIMER_H
