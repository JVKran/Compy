#ifndef __CHASSIS_HPP
#define __CHASSIS_HPP

#include "motor.hpp"

class chassis_c {
private:
    motor_c & left_motor;
    motor_c & right_motor;
public:
    chassis_c(motor_c & left_motor, motor_c & right_motor);

    void move(const direction_t direction, const uint8_t speed);
};

#endif //__CHASSIS_HPP