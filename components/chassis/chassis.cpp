#include "chassis.hpp"

chassis_c::chassis_c(motor_c & left_motor, motor_c & right_motor):
    left_motor(left_motor),
    right_motor(right_motor)
{}

void chassis_c::move(const direction_t direction, const uint8_t speed){
    left_motor.set_speed(speed);
    right_motor.set_speed(speed);
    switch(direction){
        case forward:
        case backward:      // Explicit fallthroughs
        case stop:
            left_motor.set_direction(direction);
            right_motor.set_direction(direction);
            break;
        case left: {
            left_motor.set_direction(forward);
            right_motor.set_direction(backward);
            break;
        }
        case right: {
            right_motor.set_direction(forward);
            left_motor.set_direction(backward);
            break;
        }
        default: {
            break;
        }
    }
}