#ifndef TOWER_CONTROL_H
#define TOWER_CONTROL_H

#include <Arduino.h>
#include <myDuino.h>
#include <common.h>
#include <timer.h>

/// @brief The beginning orientation of the tower, relative to the robot
enum TowerOrientation{
    Facing,
    Behind,
    Left,
    Right
};

/// @brief Tower Orientation along with the tuned angle
struct TowerInfo{
    TowerOrientation orientation;
    int16_t relative_angle;
};

/// @brief Read the tower information set by the control panel
/// @return A `TowerInfo` struct
TowerInfo read_tower_info();

/// @brief Compute the delay required to dispense the dog bone at the correct time
/// @return An unsigned long integer of the number of milliseconds to wait *after start*
uint32_t compute_dog_bone_delay();

/// @brief Dispense the dog bone by triggering the solenoid release
void dispense_dog_bone();

/// @brief Register a timer to dispense the dog bone
void register_dog_bone_timer();

#endif // TOWER_CONTROL_H
