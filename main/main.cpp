#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_err.h"
#include "esp_log.h"

#include "dev_config.hpp"
#include "motor.hpp"
#include "chassis.hpp"

#include "touch.hpp"

static const char* TAG = "MAIN";

motor_c left_motor(left_motor_conn, LEDC_CHANNEL_0, LEDC_TIMER_0);
motor_c right_motor(right_motor_conn, LEDC_CHANNEL_1, LEDC_TIMER_0);
chassis_c chassis(left_motor, right_motor);

touch_c touch(touch_pins);

extern "C" void app_main(void){
    while(true){
        ESP_LOGI(TAG, "Moving forward!");
        chassis.move(forward, 100);
        vTaskDelay(pdMS_TO_TICKS(5000));

        ESP_LOGI(TAG, "Moving left!");
        chassis.move(left, 100);
        vTaskDelay(pdMS_TO_TICKS(5000));

        ESP_LOGI(TAG, "Moving right!");
        chassis.move(right, 100);
        vTaskDelay(pdMS_TO_TICKS(5000));

        ESP_LOGI(TAG, "Stopping!");
        chassis.move(stop, 0);
        vTaskDelay(pdMS_TO_TICKS(5000));

        ESP_LOGI(TAG, "Moving backward with speed of 100!");
        chassis.move(backward, 100);
        vTaskDelay(pdMS_TO_TICKS(5000));

        for(uint8_t speed = 75; speed <= 100; speed += 5){
            chassis.move(forward, speed);
            ESP_LOGI(TAG, "Increasing speed to %i!", speed);
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }
}
