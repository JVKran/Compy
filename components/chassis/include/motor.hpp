#ifndef __MOTOR_HPP
#define __MOTOR_HPP

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_err.h"

typedef enum {
    forward     = 0x00,
    backward    = 0x01,
    stop        = 0x02
} direction_t;

struct motor_connections_t {
    const gpio_num_t forward_pin;               //!< IN1 and IN3
    const gpio_num_t backward_pin;              //!< IN2 and IN4
    const gpio_num_t speed_pin;                 //!< ENA and ENB
};

class motor_c {
private:
    motor_connections_t connections;
    ledc_channel_config_t ledc_channel;

    static bool driver_initialized;
    static bool fade_installed;
public:
    motor_c(const motor_connections_t & motor_connections, const ledc_channel_t pwm_channel, const ledc_timer_t pwm_timer);

    void set_direction(const direction_t & direction);
    void set_speed(const int speed, const int fade_ms = 0);
};

#endif //__MOTOR_HPP