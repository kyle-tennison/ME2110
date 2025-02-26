#include <myDuino.h>

// Pin numbers 
#define BANANA_PIN 0
#define MOTOR1_PIN 1    // Output for motor 1 (M1)
#define MOTOR2_PIN 2    // Output for motor 2 (M2)
#define PISTON1_PIN 1   // Output for pneumatic valve 1 (DO1)
#define PISTON2_PIN 2   // Output for piston 2 (DO2)

// Timeout duration (40 seconds = 40000 milliseconds)
#define TIMEOUT 39000

// Variables to track the start state and timing
bool started = false;
bool ended = false;
unsigned long startTime = 0;

// Robot instance pointer
myDuino* robot;

void setup() {
  Serial.begin(9600);
  
  // Initialize robot
  robot = new myDuino(1);

  // Make sure pistons are off at startup
  robot->digital(PISTON1_PIN, 0);
  robot->digital(PISTON2_PIN, 0);
}


// Called at the start of the match
void on_start(){
    // Activate both pistons upon start
    robot->digital(PISTON1_PIN, 1);
    robot->digital(PISTON2_PIN, 1);

    // Record the start time
    startTime = millis();

    // Move motor at start
    robot->moveMotor(MOTOR1_PIN, 1, 255);
}

// Called at the end of the match
void on_end(){
  // Deactivate the pistons
  robot->digital(PISTON1_PIN, 1);
  robot->digital(PISTON2_PIN, 2);

  // Deactivate the motor
  robot->moveMotor(MOTOR1_PIN, 1, 0);
}


void loop() {

  if (ended){
    return;
  }

  // This code replaces banana clip detection with a button detection
  bool banana_clips_shorted = robot->readButton(2) == 1; // returns 0 if pressed/shorted, 1 if released/not shorted

  // Wait for the start signal: banana plug is shorted (pin goes LOW)
  if (!started && banana_clips_shorted) {
    on_start();
    started=true;
  }
  
  // Check if 40 seconds have elapsed since the start signal
  if (started && (millis() - startTime >= TIMEOUT)) {
    on_end();
    ended=true;
  }
}
