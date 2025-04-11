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
  robot->LED(1, 1);

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
  robot->digital(DOG_BONE_PIN, 0);
  robot->LED(2, 1);

  // Stall forever
  while (true){}
}


void animate_bag(){

  // Define reusable lambdas
  void (*bag_on)() = [](){debug_println("Timer triggered motor ON"); robot->moveMotor(BAG_MOTOR_PIN, 1, 255);};
  void (*bag_off)() = [](){debug_println("Timer triggered motor OFF"); robot->moveMotor(BAG_MOTOR_PIN, 1, 0);};

  // Phase 1: Pull inward for half a second full power
  debug_println("Phase 1 animation start");
  robot->moveMotor(BAG_MOTOR_PIN, 1, 255);
  Timer::schedule(300, bag_off);
  Timer::schedule(300, [](){debug_println("Phase 1 animation done");});


  // Phase 2: For 2 seconds, jitter on and off
  for (uint8_t i=0; i<5; i++){
    Timer::schedule(750 + 400*i, bag_on);
    Timer::schedule(750 + 400*i + 200, bag_off);
  }
  Timer::schedule(2750, [](){debug_println("Phase 2 animation done");});

  // Phase 3, flip the bag all the way up
  Timer::schedule(2750, bag_on);
  Timer::schedule(2750+1350, bag_off);
  Timer::schedule(2750+1350, [](){debug_println("Phase 3 animation done");});
  debug_println("Scheduled animation timers");


}

/// @brief Called at the start of the match
void on_start(){
    debug_println("DEBUG: Starting...");
    started=true;
    
    // Register end timer
    Timer::schedule(TIMEOUT, &on_end);

    // Register dog bone timer
    register_dog_bone_timer();

    // Register bag timer
    Timer::schedule(BAG_START_DELAY, &animate_bag);



    // Activate both pistons upon start
    robot->digital(PNEU_ORE_PIN, 1);

    // Lock the solenoid
    robot->digital(DOG_BONE_LOCK_PIN, 1);

    Timer::schedule(50, [](){
      // Move motor at start
      debug_println("INFO: Launching motors");
      robot->digital(PNEU_LAUNCH_PIN, 1);
      robot->moveMotor(ORE_MOTOR_PIN, 1, 128);
    });
    
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
