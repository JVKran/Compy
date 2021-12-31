/// @file

#include "task.hpp"

static const char *TAG = "TASK";

/// @brief Construct an instance
/// @details
/// By constructing this instance, a task for FreeRTOS is created with the given parameters.
/// @param name The name of this specific task.
/// @param priority The priority to give this task.
/// @param stack_depth The depth of the stack for this task.
/// @param core_id The core to which the task should be pinned. Defaults to core 0.
task_c::task_c(char const*name, unsigned portBASE_TYPE priority, unsigned short stack_depth, const portBASE_TYPE core_id) {
    if(xTaskCreatePinnedToCore(&task_stub, name, stack_depth, this, priority, &handle, core_id) != pdPASS){
        ESP_LOGE(TAG, "Task \"%s\" could not be created due to insufficient memory.", name);
    }
}

/// @brief Destruct this instance
/// @details
/// When this object is destructed, the task should be deleted; applies RAII.
task_c::~task_c(){
    vTaskDelete(handle);
}

/// @brief Static cast 
/// @details
/// Cast the abstract function task() to task(void* pvParameter) to be able to
/// assign it to a task.
void task_c::task_stub(void* pv_parameter) { 
    (static_cast<task_c*>(pv_parameter))->task(); 
}
