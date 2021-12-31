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

touch_c touch;

extern "C" void app_main(void){
    while(true){
        ESP_LOGI(TAG, "Moving forward with increasing speed.");
        for(int speed = 0; speed < 100; speed++){
            chassis.move(forward, speed);
            vTaskDelay(1);
        }
        ESP_LOGI(TAG, "Moving left with speed of 50.");
        chassis.move(left, 50);
        vTaskDelay(pdMS_TO_TICKS(5000));
        ESP_LOGI(TAG, "Moving right with speed of 50.");
        chassis.move(right, 50);
        vTaskDelay(pdMS_TO_TICKS(5000));
        ESP_LOGI(TAG, "Stopping!");
        chassis.move(stop, 0);
        vTaskDelay(pdMS_TO_TICKS(5000));
        ESP_LOGI(TAG, "Moving backward with speed of 100.");
        chassis.move(backward, 100);
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}
