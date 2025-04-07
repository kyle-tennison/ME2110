#include <myDuino.h>
#include <timer.h>
#include <common.h>
#include <dog_bone.h>

// Create null pointer to make compiler happy
myDuino* robot = nullptr;

// Variables to track the start state and timing
bool started = false;
bool ended = false;
unsigned long startTime = 0;

void timer_expre(){
  debug_println("timer expired");
}

void setup() {
  Serial.begin(115200);
  debug_println("INFO: Debugging enabled");
  
  // Initialize robot
  robot = new myDuino(1);
  robot->LED(2, 1);

  // Make sure pistons are off at startup
  robot->digital(PNEU_ORE_PIN, 0);
  robot->digital(PNEU_LAUNCH_PIN, 0);
}

/// @brief Called at the end of the match
void on_end(){
  debug_println("DEBUG: End of cycle triggered");
  // Deactivate the pistons
  robot->digital(PNEU_ORE_PIN, 0);
  robot->digital(PNEU_LAUNCH_PIN, 0);

  // Deactivate all motors
  robot->moveMotor(ORE_MOTOR_PIN, 1, 0);
  robot->moveMotor(BAG_MOTOR_PIN, 1, 0);

  // Deactivate the solenoid
  robot->digital(SOLENOID_PIN, 0);
  robot->LED(1, 1);

  // Stall forever
  while (true){}
}

void end_move_bag(){
  debug_println("INFO: Stopping bag motor");

  robot->moveMotor(BAG_MOTOR_PIN, 1, 0);

}

void start_move_bag(){
  debug_println("INFO: Starting bag");
  robot->moveMotor(BAG_MOTOR_PIN, 1, 255);

  // Add timer to turn off in X seconds
  Timer* end_bag_timer = new Timer(Timer::schedule(BAG_MOTOR_DURATION, &end_move_bag));
  TimerManager::register_timer(end_bag_timer);
}


/// @brief Called at the start of the match
void on_start(){
    debug_println("DEBUG: Starting...");
    started=true;
    
    // Register end timer
    Timer* end_timer = new Timer(Timer::schedule(TIMEOUT, &on_end));
    TimerManager::register_timer(end_timer);

    // Register dog bone timer
    register_dog_bone_timer();

    // Register bag timer
    Timer* bag_timer = new Timer(Timer::schedule(BAG_START_DELAY, &start_move_bag));
    TimerManager::register_timer(bag_timer);

    // Activate both pistons upon start
    robot->digital(PNEU_ORE_PIN, 1);
    
    // Move motor at start
    robot->digital(PNEU_LAUNCH_PIN, 1);
    robot->moveMotor(ORE_MOTOR_PIN, 1, 255);
}

void loop() {

  // This code replaces banana clip detection with a button detection
  bool banana_clips_shorted = robot->readButton(BANANA_PIN) == 1; // returns 0 if pressed/shorted, 1 if released/not shorted
  
  // Wait for the start signal: banana plug is shorted (pin goes LOW)
  if (!started && banana_clips_shorted) {
    debug_println("INFO: Banana clips shorted");
    on_start();
  }

  TimerManager::poll_timers();

}
