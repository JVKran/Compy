#include "touch.hpp"

bool touch_c::s_pad_activated[TOUCH_PAD_MAX - 3];
uint32_t touch_c::s_pad_init_val[TOUCH_PAD_MAX - 3];

static const char* TAG = "TOUCH";

void touch_c::init(){
    ESP_ERROR_CHECK(touch_pad_init());
    touch_pad_set_fsm_mode(TOUCH_FSM_MODE_TIMER);
    touch_pad_set_voltage(TOUCH_HVOLT_2V7, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_1V);

    for (const auto & pin : touch_pins) {
        touch_pad_config(pin, TOUCH_THRESH_NO_USE);
    }
    touch_pad_filter_start(TOUCHPAD_FILTER_TOUCH_PERIOD);
    set_thresholds();

    touch_pad_isr_register(touch_rtc_int, this);
}

void touch_c::interrupt(){
    uint32_t pad_intr = touch_pad_get_status();
    //clear interrupt
    touch_pad_clear_status();
    for (const auto & pin : touch_pins) {
        if ((pad_intr >> pin) & 0x01) {
            s_pad_activated[pin] = true;
        }
    }
}

void touch_c::touch_rtc_int(void *arg){
    touch_c* touch = (touch_c*)arg;
    touch->interrupt();
}

void touch_c::set_thresholds(void){
    uint16_t touch_value;
    for (const auto & pin : touch_pins) {
        touch_pad_read_filtered(pin, &touch_value);
        s_pad_init_val[pin] = touch_value;
        ESP_LOGI(TAG, "test init: touch pad [%d] val is %d", pin, touch_value);
        ESP_ERROR_CHECK(touch_pad_set_thresh(pin, touch_value * 2 / 3));

    }
}

void touch_c::task(){
    int show_message = 0;
    init();
    while (1) {
        //interrupt mode, enable touch interrupt
        touch_pad_intr_enable();
        for (const auto & pin : touch_pins) {
            if (s_pad_activated[pin] == true) {
                ESP_LOGI(TAG, "T%d activated!", pin);
                // Wait a while for the pad being released
                vTaskDelay(200 / portTICK_PERIOD_MS);
                // Clear information on pad activation
                s_pad_activated[pin] = false;
                // Reset the counter triggering a message
                // that application is running
                show_message = 1;
            }
        }

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}