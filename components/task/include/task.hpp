/// @file

#ifndef __TASK_HPP
#define __TASK_HPP

extern "C" {
    #include "freertos/FreeRTOS.h"
    #include "freertos/task.h"

    #include "esp_log.h"
}

#define FIRST_CORE      0
#define SECOND_CORE     1
#define AVAILABLE_CORE  tskNO_AFFINITY

/// @brief Abstract task class
/// @details
/// One can inherit from this class to ensure the easy use of task functions
/// by overriding the abstract function task(). As much as possible is private
/// to decrease the posibility of the user doing things that shouldn't be possible.
class task_c {
    private:
        virtual void task() = 0;
        static void task_stub(void* pv_parameter);
    public:
        task_c(char const*name, unsigned portBASE_TYPE priority, unsigned short stack_depth, const portBASE_TYPE core_id = 0);
        ~task_c();  

        xTaskHandle handle;             				//!< The handle of this task.
};

#endif //__TASK_HPP