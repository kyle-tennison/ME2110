#include <Arduino.h>
#include <myDuino.h>
#include <common.h>


myDuino* robot = nullptr;

void unspool(){
  Serial.println("Starting unspool");

  bool in1, in2;

  while (true){
      // Read the input from the switches
      in1 = robot->readButton(TUNE_SWITCH_1) == 1; // switches are flipped
      in2 = robot->readButton(TUNE_SWITCH_2) == 0;

      Serial.print("in1: ");
      Serial.print(in1);
      Serial.print("; in2: ");
      Serial.println(in2);


      if ((!in1 && !in2) || (in1 && in2)){
        // do nothing
        robot->moveMotor(BAG_MOTOR_PIN, 1, 0);
        continue;
      }
      else if (in1){
        robot->moveMotor(BAG_MOTOR_PIN, 1, 255);
      }
      else if (in2){
        robot->moveMotor(BAG_MOTOR_PIN, 2, 255);
      }
  }

}

void setup() {
  Serial.begin(9600);
  
  // Initialize robot
  robot = new myDuino(1);
  robot->LED(2, 1);

  // unspool();
}

void loop() {
    Serial.println("Hello from Arduino");
    delay(1000);

}
