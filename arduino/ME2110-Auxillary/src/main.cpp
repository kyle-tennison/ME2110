#include <Arduino.h>
#include <myDuino.h>
#include <common.h>
#include <js_io.h>


myDuino* robot = nullptr;
uint32_t last_heartbeat = 0;
bool animation_started = false;

void unspool(){
  bool in1, in2;

    // Read the input from the switches
    in1 = robot->readButton(TUNE_SWITCH_1) == 1;
    in2 = robot->readButton(TUNE_SWITCH_2) == 1;


    if ((!in1 && !in2) || (in1 && in2)){
      // do nothing
      robot->moveMotor(BAG_MOTOR_PIN, 1, 0);
      return;
    }
    else if (in1){
      robot->moveMotor(BAG_MOTOR_PIN, 1, 255);
    }
    else if (in2){
      robot->moveMotor(BAG_MOTOR_PIN, 2, 255);
    }

}

void pwm_solenoid(){

  bool in1, in2;
  in1 = robot->readButton(TUNE_SWITCH_1) == 1;
  in2 = robot->readButton(TUNE_SWITCH_2) == 1;

  if (in1 && in2){
    robot->digital(PNEU_ORE_PIN, 1);
    delay(220);
    robot->digital(PNEU_ORE_PIN, 0);
    delay(20);
  }

}

void animate_bag(){
  bool in1, in2;
  in1 = robot->readButton(TUNE_SWITCH_1) == 1;
  in2 = robot->readButton(TUNE_SWITCH_2) == 1;

  if (!animation_started && in1 && in2){
    animation_started = true;

    robot->moveMotor(BAG_MOTOR_PIN, 1, 255);
    delay(500);
    robot->moveMotor(BAG_MOTOR_PIN, 1, 0);
    delay(200);
  
      for (uint8_t i=0; i<5; i++){
        robot->moveMotor(BAG_MOTOR_PIN, 1, 255);
        delay(200);
        robot->moveMotor(BAG_MOTOR_PIN, 1, 0);
        delay(200);
      }

      robot->moveMotor(BAG_MOTOR_PIN, 1, 255);
      delay(1600);
      robot->moveMotor(BAG_MOTOR_PIN, 1, 0);
  }

}

void setup() {
  Serial.begin(115200);
  
  // Initialize robot
  robot = new myDuino(1);
  robot->LED(1, 1);
}

/// @brief Read the telemetry of the robot
/// @return A TelemetryPayload struct
TelemetryPayload read_telem(){
  TelemetryPayload data;
  data.in1 = robot->readButton(TUNE_SWITCH_1) == 1;
  data.in2 = robot->readButton(TUNE_SWITCH_2) == 1;
  data.pot = static_cast<int16_t>((100* static_cast<int32_t>(robot->readPOT()))/POTENTIOMETER_MAX);
  
  if (data.pot < 33){
    data.op = OpMode::BAG;
  }
  else if (data.pot >= 33 && data.pot < 66){
    data.op = OpMode::PWM;
  }
  else if (data.pot >= 66){
    data.op = OpMode::BAG_ANIMATE;
  }
  

  return data;
}

/// @brief Handle an incoming command
/// @param payload The incomming command
void dispatch_command(CommandPayload payload){
  debug_println("!Dispatching command");
  
  if (payload.db){
    debug_println("!Triggerring dog bone ON");
    robot->digital(SOLENOID_PIN, 1);
  }
  else{
    debug_println("!Triggerring dog bone OFF");
    robot->digital(SOLENOID_PIN, 0);
  };
  if (payload.go){
    debug_println("!Triggerring bottom ore collector ON");
    robot->digital(PNEU_ORE_PIN, 1);
  }
  else {
    debug_println("!Triggerring bottom ore collector OFF");
    robot->digital(PNEU_ORE_PIN, 0);
  }
  if (payload.to){
    debug_println("!Triggering top ore collector ON");
    robot->moveMotor(ORE_MOTOR_PIN, 1, 255);
  }
  else {
    debug_println("!Triggering top ore collector OFF");
    robot->moveMotor(ORE_MOTOR_PIN, 1, 0);
  }
  if (payload.la){
    debug_println("!Triggerring launch piston ON");
    robot->digital(PNEU_LAUNCH_PIN, 1);
  }
  else{
    debug_println("!Triggerring launch piston OFF");
    robot->digital(PNEU_LAUNCH_PIN, 0);
  }

}



void loop() {

  TelemetryPayload telem = read_telem();
    if (millis() - last_heartbeat > 100){
      send_payload(&telem);
      debug_print("!Available bytes: ");
      debug_println(Serial.available());
      last_heartbeat=millis();
    }


    if (Serial.available() > 0){
      delay(50);
      CommandPayload command = read_incoming();
      dispatch_command(command);
    }



    if (telem.op == OpMode::BAG){
      unspool();
    }
    if (telem.op == OpMode::PWM){
      pwm_solenoid();
    }
    if (telem.op == OpMode::BAG_ANIMATE){
      animate_bag();
    }
    else{
      animation_started = false;
    }

}
