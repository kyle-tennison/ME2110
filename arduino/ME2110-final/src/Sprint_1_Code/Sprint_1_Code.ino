#include <myDuino.h>

myDuino robot(1);

// Pin numbers 
const int bananaPin = 0;
const int motor1Pin = 1;    // Output for motor 1 (M1)
const int motor2Pin = 2;    // Output fpr motor 2 (M2)
const int piston1Pin  = 1;  // Output for peumatic valve 1 (DO1)
const int piston2Pin  = 2;  // Output for piston 2 (DO2)

// Timeout duration (40 seconds = 40000 milliseconds)
const unsigned long timeout = 40000;

// Variables to track the start state and timing
bool started = false;
unsigned long startTime = 0;

void setup() {

  Serial.begin(9600);

  // Make sure pistons are off at startup
  robot.digital(piston1Pin, 0);
  robot.digital(piston2Pin, 0);
}

void loop() {
  // This code replaces with banana clip detection with a button detection
  bool state = robot.readButton(2); // returns 0 if pressed/shorted, 1 if released/not shorted

  // Wait for the start signal: banana plug is shorted (pin goes LOW)
  if (!started && (state == 0)) { //digitalRead(bananaPin) == LOW) {
    // Activate both pistons upon start
    robot.digital(piston1Pin, 1);
    robot.digital(piston2Pin, 1);

    // Record the start time
    startTime = millis();
    started = true;

    // Move motor at start
    robot.moveMotor(motor1Pin,1,255);
  }
  
  // Check if 40 seconds have elapsed since the start signal
  if (started && (millis() - startTime >= timeout)) {
    // Deactivate the pistons
    robot.digital(piston1Pin, 1);
    robot.digital(piston2Pin, 2);

    // Deactivate the motor
    robot.moveMotor(motor1Pin,1,0);
    
    // Optionally, prevent re-triggering by not resetting "started"
    // If you want the process to be restartable, you could reset started here.
  }
}
