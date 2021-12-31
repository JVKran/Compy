#include "motor.hpp"

bool motor_c::driver_initialized = false;
bool motor_c::fade_installed = false;

static void driver_initialize(const ledc_timer_t pwm_timer){
    ledc_timer_config_t ledc_timer = {
        .speed_mode         = LEDC_HIGH_SPEED_MODE,           // timer mode
        .duty_resolution    = LEDC_TIMER_11_BIT, // resolution of PWM duty
        .timer_num          = pwm_timer,            // timer index
        .freq_hz            = 20000,                      // frequency of PWM signal
        .clk_cfg            = LEDC_AUTO_CLK,              // Auto select the source clock
    };
    ledc_timer_config(&ledc_timer);
}

motor_c::motor_c(const motor_connections_t & motor_connections, const ledc_channel_t pwm_channel, const ledc_timer_t pwm_timer):
    connections(motor_connections)
{
    if(!driver_initialized){
        driver_initialized = true;
        driver_initialize(pwm_timer);
    }

    gpio_config_t io_conf;
    io_conf.intr_type       = GPIO_INTR_DISABLE;
    io_conf.mode            = GPIO_MODE_OUTPUT;
    io_conf.pull_up_en      = GPIO_PULLUP_DISABLE;                           // Add external 4.7kOhm pull-up.
    io_conf.pull_down_en    = GPIO_PULLDOWN_DISABLE;

    io_conf.pin_bit_mask    = 1ULL << connections.forward_pin;
    gpio_config(&io_conf);

    io_conf.pin_bit_mask    = 1ULL << connections.backward_pin;
    gpio_config(&io_conf);

    ledc_channel.channel    = pwm_channel;
    ledc_channel.duty       = 0;
    ledc_channel.gpio_num   = connections.speed_pin;
    ledc_channel.speed_mode = LEDC_HIGH_SPEED_MODE;
    ledc_channel.hpoint     = 0;
    ledc_channel.timer_sel  = pwm_timer;
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
    
    set_direction(stop);
}

void motor_c::set_direction(const direction_t & direction){
    switch(direction){
        case forward: {
            gpio_set_level(connections.forward_pin, 1);
            gpio_set_level(connections.backward_pin, 0);
            break;
        }
        case backward: {
            gpio_set_level(connections.forward_pin, 0);
            gpio_set_level(connections.backward_pin, 1);
            break;
        }
        case stop: {
            gpio_set_level(connections.forward_pin, 0);
            gpio_set_level(connections.backward_pin, 0);
            break;
        }
    }
}

void motor_c::set_speed(const int speed, const int fade_ms){
    uint32_t duty = (speed < 100) ? speed * 20 : 2000;                   // Duty cycle of 2000 is maximum.
    if(fade_ms){
        if(!fade_installed){
            fade_installed = true;
            ledc_fade_func_install(0);
        }
        ledc_set_fade_with_time(ledc_channel.speed_mode,
                    ledc_channel.channel, duty, fade_ms);
        ledc_fade_start(ledc_channel.speed_mode,
                    ledc_channel.channel, LEDC_FADE_NO_WAIT);
    } else {
        ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, duty);
        ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
    }
}