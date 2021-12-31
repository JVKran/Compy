#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_err.h"

#include "dev_config.hpp"
#include "motor.hpp"

motor_c left_motor(left_motor_conn, LEDC_CHANNEL_0, LEDC_TIMER_0);
motor_c right_motor(right_motor_conn, LEDC_CHANNEL_1, LEDC_TIMER_0);
chassis_c chassis(left_motor, right_motor);

extern "C" void app_main(void){
    while(true){
        left_motor.set_direction(forward);
        for(int i = 0; i < 100; i+=1){
            left_motor.set_speed(i);
            vTaskDelay(1);
        }
        left_motor.set_direction(backward);
        for(int i = 100; i >= 0; i-=1){
            left_motor.set_speed(i);
            vTaskDelay(1);
        }
        left_motor.set_speed(100, 1000);             // Fade to max in 1 second.
        vTaskDelay(pdMS_TO_TICKS(1000));
        left_motor.set_speed(0, 1000);               // Fade to 0 in 1 second.
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
