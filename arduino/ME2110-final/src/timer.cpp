#include <Arduino.h>
#include <timer.h>
#include <common.h>

#define MAX_TIMER_COUNT 16 // number of timers to allocate in memory

Timer::Timer(uint32_t _expiration_time, void (*_callback)()){
    expiration_time = _expiration_time;
    callback = _callback;
    activated = false;
}

/// @brief Schedule a timer to go off in the future
/// @param in_millis The number of milliseconds in the future to wait for the timer
/// @param callback The function to call upon the timer expring
/// @return A constructed `Timer` object
Timer Timer::schedule(uint32_t in_millis, void (*callback)()){
    uint32_t _expiration_time = millis() + in_millis;
    debug_print("DEBUG: Local time is ");
    debug_println(millis());
    debug_print("DEBUG: Expiration time set to ");
    debug_println(_expiration_time);
    return Timer(_expiration_time, callback);
}

/// @brief Register a timer in the list of timers
/// @param timer The timer to register
void TimerManager::register_timer(Timer* timer){

    for (uint8_t i = 0; i < MAX_TIMER_COUNT; i++){
        if (TimerManager::timers[i] == nullptr) {
            TimerManager::timers[i] = timer;
            break;
        }
    }
    
}

/// @brief Poll the timers registered in the loop
void TimerManager::poll_timers(){
    for (uint8_t i = 0; i<MAX_TIMER_COUNT; i++){
        Timer* timer = TimerManager::timers[i];

        if (timer == nullptr){
            continue;
        }

        // invoke callbacks in the timers
        if (!timer->activated && timer->expiration_time < millis()){
            timer->callback();
            timer->activated = true;
        }

        // unregister times that have already been activated
        if (timer->activated){
            TimerManager::timers[i] = nullptr;
        }
    }
}

Timer* TimerManager::timers[MAX_TIMER_COUNT] = {nullptr};