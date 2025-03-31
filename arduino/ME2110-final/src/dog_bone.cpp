#include <Arduino.h>
#include <myDuino.h>
#include <common.h>
#include <timer.h>
#include <dog_bone.h>

/// @brief Read the tower information set by the control panel
/// @return A `TowerInfo` struct
TowerInfo read_tower_info(){

    // Read the input from the switches
    bool in1 = robot->readButton(TUNE_SWITCH_1) == 1; // switches are flipped
    bool in2 = robot->readButton(TUNE_SWITCH_2) == 0;

    
    uint8_t equiv = (static_cast<uint8_t>(in2) << 1) | static_cast<uint8_t>(in1);
    
    debug_print("DEBUG: Input switch configuration is: in1=");
    debug_print(in1);
    debug_print("; in2=");
    debug_print(in2);
    debug_print("; equiv=");
    debug_println(equiv);
    
    
    TowerOrientation orientation;

    debug_print("INFO: Tower orientation is: ");

    switch (equiv){
        case 0:
            orientation = TowerOrientation::Facing;
            debug_println("FACING");
            break;
        case 1:
            orientation = TowerOrientation::Left;
            debug_println("LEFT");
            break;
        case 2:
            orientation = TowerOrientation::Right;
            debug_println("RIGHT");
            break;
        case 3:
            orientation = TowerOrientation::Behind;
            debug_println("BEHIND");
            break;
        default:
            debug_println("FATAL: Switch input was not recognized.");
            orientation = TowerOrientation::Facing;  // fallback so the robot doesn't crash
            break;
    }

    // Read the potentiometer
    int32_t pot_value = robot->readPOT();
    debug_print("DEBUG: Read potentiometer as: ");
    debug_println(pot_value);

    int16_t percent_turn = (100*(pot_value + POTENTIOMETER_MIN)) / (POTENTIOMETER_MAX+POTENTIOMETER_MIN);
    debug_print("DEBUG: The calculated percent turn: ");
    debug_print(percent_turn);
    debug_println("%");

    int16_t relative_angle = (TUNABLE_WINDOW * percent_turn)/ 100 - (TUNABLE_WINDOW/2);
    debug_print("DEBUG: Final relative angle is: ");
    debug_println(relative_angle);

    TowerInfo info;
    info.orientation = orientation;
    info.relative_angle = relative_angle;
    return info;

}

/// @brief Compute the delay required to dispense the dog bone at the correct time
/// @return An unsigned long integer of the number of miliseconds to wait *after start*
uint32_t compute_dog_bone_delay(){

    const double rot_period = 60 / PLATE_ANGULAR_VEL;
    const TowerInfo tower_info = read_tower_info();

    double delay = rot_period; // by default allow one rotation to go by

    switch (tower_info.orientation){
        case TowerOrientation::Facing:
            delay += 0;
            break;
        case TowerOrientation::Right:
            delay += (1/4) * rot_period;
            break;
        case TowerOrientation::Behind:
            delay += (2/4) * rot_period;
            break;
        case TowerOrientation::Left:
            delay += (3/4) * rot_period;
            break;
        default:
            break;
    }

    const float percent_wait =  float(tower_info.relative_angle) / (TUNABLE_WINDOW);

    debug_print("INFO: Dog bone tune set to ");
    debug_print(percent_wait);
    debug_println("% wait");


    const uint16_t wait_milliseconds = percent_wait * (rot_period/4);
    debug_print("INFO: Waiting for ");
    debug_print(wait_milliseconds);
    debug_println(" milliseconds to dispense dog bone.");

    return wait_milliseconds;
}

/// @brief Dispense the dog bone by triggering the solenoid release
void dispense_dog_bone(){
    debug_println("INFO: Dispensing dog bone...");
    robot->digital(SOLENOID_PIN, 1);
}

void register_dog_bone_timer(){
    Timer* timer = new Timer(Timer::schedule(compute_dog_bone_delay(), &dispense_dog_bone));
    TimerManager::register_timer(timer);
}