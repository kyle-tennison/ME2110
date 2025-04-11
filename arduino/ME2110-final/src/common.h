#ifndef COMMON_H
#define COMMON_H

// Pin numbers 
#include "Arduino.h"
#include "myDuino.h"

#define BANANA_PIN 4 // Read; Banana pin input
#define TUNE_SWITCH_1 2 // Read; 1st toggle switch
#define TUNE_SWITCH_2 3 // Read; 2nd toggle switch
#define PNEU_ORE_PIN 4 // Write; 1st pneumatic valve
#define PNEU_LAUNCH_PIN 3 // Write; 2nd pneumatic valve
#define DOG_BONE_PIN 2 // Write; Dog bone solenoid
#define DOG_BONE_PIN 2 // Write; Dog bone solenoid
#define DOG_BONE_LOCK_PIN 1 // Write; Dog bone lock solenoid
#define ORE_MOTOR_PIN 2 // Motor; 
#define BAG_MOTOR_PIN 1 // Motor;

// Timeout duration (40 seconds = 40000 milliseconds)
#define TIMEOUT 38000 // milliseconds

// Potentiometer values
#define POTENTIOMETER_MIN 0
#define POTENTIOMETER_MAX 658

// Dog bone settings
#define TUNABLE_WINDOW 90.0 // Tunable window for potentiometer in degrees
#define PLATE_ROT_PERIOD 10100 // Milliseconds

// Bag settings
#define BAG_MOTOR_DURATION 2500 // milliseconds
#define BAG_START_DELAY 20000 // milliseconds

// Debug command
#define DEBUG
#ifdef DEBUG
  #define debug_print(x) Serial.print(x)
  #define debug_println(x) Serial.println(x)
#else
  // No-op when DEBUG is not defined
  #define debug_print(x) 
  #define debug_println(x) 
#endif

// Robot instance pointer
extern myDuino* robot;

#endif