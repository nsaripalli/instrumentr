#include "r_api.h"
#include <R_ext/Rdynload.h>

#include <stdio.h>

extern "C" {
static const R_CallMethodDef CallEntries[] = {
    /* lightr */
    {"lightr_get_application", (DL_FUNC) &r_lightr_get_application, 0},
    {"lightr_get_context", (DL_FUNC) &r_lightr_get_context, 0},
    {"lightr_set_context", (DL_FUNC) &r_lightr_set_context, 1},
    {"lightr_interception_is_enabled", (DL_FUNC) &r_lightr_interception_is_enabled, 0},
    {"lightr_disable_interception", (DL_FUNC) &r_lightr_disable_interception, 0},
    {"lightr_enable_interception", (DL_FUNC) &r_lightr_enable_interception, 0},
    {"lightr_intercept_package_entry", (DL_FUNC) &r_lightr_intercept_package_entry, 1},
    {"lightr_intercept_package_exit", (DL_FUNC) &r_lightr_intercept_package_exit, 1},
    {"lightr_initialize", (DL_FUNC) &r_lightr_initialize, 5},
    {"lightr_finalize", (DL_FUNC) &r_lightr_finalize, 0},
    {"lightr_intercept_call_entry", (DL_FUNC) &r_lightr_intercept_call_entry, 5},
    {"lightr_intercept_call_exit", (DL_FUNC) &r_lightr_intercept_call_exit, 4},

    /* Object */
    {"object_get_id", (DL_FUNC) &r_object_get_id, 1},
    {"object_set_data", (DL_FUNC) &r_object_set_data, 2},
    {"object_get_data", (DL_FUNC) &r_object_get_data, 1},
    {"object_remove_data", (DL_FUNC) &r_object_remove_data, 1},
    {"object_has_data", (DL_FUNC) &r_object_has_data, 1},

    /* Context */
    {"context_create_context", (DL_FUNC) &r_context_create_context, 7},
    {"context_set_initializer", (DL_FUNC) &r_context_set_initializer, 2},
    {"context_get_initializer", (DL_FUNC) &r_context_get_initializer, 1},
    {"context_set_finalizer", (DL_FUNC) &r_context_set_finalizer, 2},
    {"context_get_finalizer", (DL_FUNC) &r_context_get_finalizer, 1},
    {"context_set_package_entry_callback", (DL_FUNC) &r_context_set_package_entry_callback, 2},
    {"context_get_package_entry_callback", (DL_FUNC) &r_context_get_package_entry_callback, 1},
    {"context_set_package_exit_callback", (DL_FUNC) &r_context_set_package_exit_callback, 2},
    {"context_get_package_exit_callback", (DL_FUNC) &r_context_get_package_exit_callback, 1},
    {"context_set_call_entry_callback", (DL_FUNC) &r_context_set_call_entry_callback, 2},
    {"context_get_call_entry_callback", (DL_FUNC) &r_context_get_call_entry_callback, 1},
    {"context_set_call_exit_callback", (DL_FUNC) &r_context_set_call_exit_callback, 2},
    {"context_get_call_exit_callback", (DL_FUNC) &r_context_get_call_exit_callback, 1},
    {"context_set_environment", (DL_FUNC) &r_context_set_environment, 2},
    {"context_get_environment", (DL_FUNC) &r_context_get_environment, 1},

    /* Application */
    {"application_get_name", (DL_FUNC) &r_application_get_name, 1},
    {"application_get_directory", (DL_FUNC) &r_application_get_directory, 1},
    {"application_get_environment", (DL_FUNC) &r_application_get_environment, 1},
    {"application_get_packages", (DL_FUNC) &r_application_get_packages, 1},
    {"application_add_package", (DL_FUNC) &r_application_add_package, 2},

    /* Package */
    {"package_create", (DL_FUNC) &r_package_create, 3},
    {"package_get_name", (DL_FUNC) &r_package_get_name, 1},
    {"package_get_directory", (DL_FUNC) &r_package_get_directory, 1},
    {"package_get_environment", (DL_FUNC) &r_package_get_environment, 1},
    {"package_get_functions", (DL_FUNC) &r_package_get_functions, 1},
    {"package_add_function", (DL_FUNC) &r_package_add_function, 2},

    /* Function */
    {"function_create", (DL_FUNC) &r_function_create, 3},
    {"function_get_name", (DL_FUNC) &r_function_get_name, 1},
    {"function_get_parameter_count", (DL_FUNC) &r_function_get_parameter_count, 1},
    {"function_get_object", (DL_FUNC) &r_function_get_object, 1},

    /* Call */
    {"call_get_function", (DL_FUNC) &r_call_get_function, 1},
    {"call_get_state", (DL_FUNC) &r_call_get_state, 1},
    {"call_get_parameters", (DL_FUNC) &r_call_get_parameters, 1},

    /* Parameter */
    {"parameter_get_name", (DL_FUNC) &r_parameter_get_name, 1},
    {"parameter_get_position", (DL_FUNC) &r_parameter_get_position, 1},
    {"parameter_is_missing", (DL_FUNC) &r_parameter_is_missing, 1},
    {"parameter_is_vararg", (DL_FUNC) &r_parameter_is_vararg, 1},
    {"parameter_get_arguments", (DL_FUNC) &r_parameter_get_arguments, 1},

    /* Argument */
    {"argument_get_name", (DL_FUNC) &r_argument_get_name, 1},
    {"argument_is_evaluated", (DL_FUNC) &r_argument_is_evaluated, 1},

    /* CallStack */
    {"call_stack_get_size", (DL_FUNC) &r_call_stack_get_size, 1},
    {"call_stack_get_frame", (DL_FUNC) &r_call_stack_get_frame, 2},
    {"call_stack_peek_frame", (DL_FUNC) &r_call_stack_peek_frame, 2},

    {NULL, NULL, 0}};

void R_init_lightr(DllInfo* dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
}
