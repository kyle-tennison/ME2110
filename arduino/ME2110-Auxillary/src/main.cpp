#include <Arduino.h>
#include <myDuino.h>
#include <common.h>
#include <js_io.h>


myDuino* robot = nullptr;

void unspool(){
  debug_println("!Starting unspool");

  bool in1, in2;

  while (true){
      // Read the input from the switches
      in1 = robot->readButton(TUNE_SWITCH_1) == 1; // switches are flipped
      in2 = robot->readButton(TUNE_SWITCH_2) == 0;

      debug_print("!in1: ");
      debug_print(in1);
      debug_print("; in2: ");
      debug_println(in2);


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


TelemetryPayload read_telem(){
  TelemetryPayload data;
  data.in1 = robot->readButton(TUNE_SWITCH_1) == 0;
  data.in2 = robot->readButton(TUNE_SWITCH_2) == 1;
  data.pot = (100*robot->readPOT())/(POTENTIOMETER_MAX);
  data.op = OpMode::BAG;

  return data;
}

uint32_t last_heartbeat = 0;

void loop() {

    TelemetryPayload telem = read_telem();

    send_payload(&telem);

    debug_print("!Available bytes: ");
    debug_println(Serial.available());

    if (Serial.available() > 0){
      read_incoming();
    }

    delay(1000);
}
