#include "touch.hpp"

bool touch_c::s_pad_activated[TOUCH_PAD_MAX - 3];
uint32_t touch_c::s_pad_init_val[TOUCH_PAD_MAX - 3];

static const char* TAG = "TOUCH";

void touch_c::init(){
    ESP_ERROR_CHECK(touch_pad_init());
    // If use interrupt trigger mode, should set touch sensor FSM mode at 'TOUCH_FSM_MODE_TIMER'.
    touch_pad_set_fsm_mode(TOUCH_FSM_MODE_TIMER);
    // Set reference voltage for charging/discharging
    // For most usage scenarios, we recommend using the following combination:
    // the high reference valtage will be 2.7V - 1V = 1.7V, The low reference voltage will be 0.5V.
    touch_pad_set_voltage(TOUCH_HVOLT_2V7, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_1V);
    // Init touch pad IO
    for (int i = 0; i < TOUCH_PAD_MAX - 3; i++) {
        //init RTC IO and mode for touch pad.
        touch_pad_config((touch_pad_t)i, TOUCH_THRESH_NO_USE);
    }
    // Initialize and start a software filter to detect slight change of capacitance.
    touch_pad_filter_start(TOUCHPAD_FILTER_TOUCH_PERIOD);
    // Set thresh hold
    set_thresholds();

    touch_pad_isr_register(touch_rtc_int, NULL);
}

void touch_c::touch_rtc_int(void *arg)
{
    uint32_t pad_intr = touch_pad_get_status();
    //clear interrupt
    touch_pad_clear_status();
    for (int i = 0; i < TOUCH_PAD_MAX - 3; i++) {
        if ((pad_intr >> i) & 0x01) {
            s_pad_activated[i] = true;
        }
    }
}

void touch_c::set_thresholds(void){
    uint16_t touch_value;
    for (int i = 0; i < TOUCH_PAD_MAX - 3; i++) {
        //read filtered value
        touch_pad_read_filtered((touch_pad_t)i, &touch_value);
        s_pad_init_val[i] = touch_value;
        ESP_LOGI(TAG, "test init: touch pad [%d] val is %d", i, touch_value);
        //set interrupt threshold.
        ESP_ERROR_CHECK(touch_pad_set_thresh((touch_pad_t)i, touch_value * 2 / 3));

    }
}

void touch_c::task(){
    static int show_message;
    int change_mode = 0;
    int filter_mode = 0;

    init();
    while (1) {
        if (filter_mode == 0) {
            //interrupt mode, enable touch interrupt
            touch_pad_intr_enable();
            for (int i = 0; i < TOUCH_PAD_MAX - 3; i++) {
                if (s_pad_activated[i] == true) {
                    ESP_LOGI(TAG, "T%d activated!", i);
                    // Wait a while for the pad being released
                    vTaskDelay(200 / portTICK_PERIOD_MS);
                    // Clear information on pad activation
                    s_pad_activated[i] = false;
                    // Reset the counter triggering a message
                    // that application is running
                    show_message = 1;
                }
            }
        } else {
            //filter mode, disable touch interrupt
            touch_pad_intr_disable();
            touch_pad_clear_status();
            for (int i = 0; i < TOUCH_PAD_MAX - 3; i++) {
                uint16_t value = 0;
                touch_pad_read_filtered((touch_pad_t)i, &value);
                if (value < s_pad_init_val[i] * TOUCH_THRESH_PERCENT / 100) {
                    ESP_LOGI(TAG, "T%d activated!", i);
                    ESP_LOGI(TAG, "value: %d; init val: %d", value, s_pad_init_val[i]);
                    vTaskDelay(200 / portTICK_PERIOD_MS);
                    // Reset the counter to stop changing mode.
                    change_mode = 1;
                    show_message = 1;
                }
            }
        }

        vTaskDelay(10 / portTICK_PERIOD_MS);

        // If no pad is touched, every couple of seconds, show a message
        // that application is running
        if (show_message++ % 500 == 0) {
            ESP_LOGI(TAG, "Waiting for any pad being touched...");
        }
        // Change mode if no pad is touched for a long time.
        // We can compare the two different mode.
        if (change_mode++ % 2000 == 0) {
            filter_mode = !filter_mode;
            ESP_LOGW(TAG, "Change mode...%s", filter_mode == 0 ? "interrupt mode" : "filter mode");
        }
    }
}