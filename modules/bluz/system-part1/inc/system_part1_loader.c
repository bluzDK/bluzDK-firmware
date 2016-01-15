#include <stdbool.h>

extern void** dynalib_location_user;

static bool module_user_part_validated = true;

/**
 * Determines if the user module is present and valid.
 * @return
 */
bool is_user_module_valid()
{
    return module_user_part_validated;
}

/**
 * The current start of heap.
 */
extern void* sbrk_heap_top;

/**
 * Global initialization function. Called after memory has been initialized in this module
 * but before C++ constructors are executed and before any dynamic memory has been allocated.
 */
void system_part1_pre_init() {
    
    module_user_part_validated = HAL_Core_Validate_User_Module();
//    module_user_part_validated = true;

    if (is_user_module_valid()) {
        module_user_pre_init();
//        void* new_heap_top = module_user_pre_init();
//        if (new_heap_top>sbrk_heap_top)
//            sbrk_heap_top = new_heap_top;
    }
    else {
        // indicate to the system that it shouldn't run user code
        set_system_mode(SAFE_MODE);
    }
}

/*
 * Invoked after all module-scope instances have been constructed.
 */
void system_part1_init() {

}

void system_part1_post_init() {
    if (is_user_module_valid()) {
        module_user_init();
    }
}

void setup() {
    if (is_user_module_valid()) {
        module_user_setup();
    }
}

void loop() {
    if (is_user_module_valid()) {
        module_user_loop();
    }
}

__attribute__((externally_visible, section(".module_pre_init"))) const void* system_part1_pre_init_fn = (const void*)system_part1_pre_init;
__attribute__((externally_visible, section(".module_init"))) const void* system_part1_init_fn = (const void*)system_part1_init;

