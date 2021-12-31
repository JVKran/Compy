#ifndef __DEV_CONFIG_HPP
#define __DEV_CONFIG_HPP

#include "motor.hpp"

motor_connections_t left_motor_conn {
    .forward_pin    = GPIO_NUM_21,
    .backward_pin   = GPIO_NUM_22,
    .speed_pin      = GPIO_NUM_23
};

motor_connections_t right_motor_conn {
    .forward_pin    = GPIO_NUM_25,
    .backward_pin   = GPIO_NUM_26,
    .speed_pin      = GPIO_NUM_27
};

#endif //__DEV_CONFIG_HPP