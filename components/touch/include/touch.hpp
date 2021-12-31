#ifndef __TOUCH_HPP
#define __TOUCH_HPP

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"

#include "driver/touch_pad.h"
#include "soc/rtc_periph.h"
#include "soc/sens_periph.h"

#include "task.hpp"

#define TOUCH_THRESH_NO_USE   (0)
#define TOUCH_THRESH_PERCENT  (80)
#define TOUCHPAD_FILTER_TOUCH_PERIOD (10)

class touch_c : public task_c {
private:
    static bool s_pad_activated[TOUCH_PAD_MAX - 3];
    static uint32_t s_pad_init_val[TOUCH_PAD_MAX - 3];

    void set_thresholds();
    static void touch_rtc_int(void *arg);

    virtual void task() override;
public:
    touch_c():
        task_c("Touch", 1, 2048)
    {}
    void init();
};

#endif //__TOUCH_HPP