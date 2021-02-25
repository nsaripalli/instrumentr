#include "GcUnmarkCallback.hpp"
#include "Application.hpp"
#include "Context.hpp"

namespace instrumentr {

typedef void (*callback_t)(ContextSPtr context,
                           ApplicationSPtr application,
                           SEXP r_object);

SEXP GcUnmarkCallback::class_ = nullptr;

void GcUnmarkCallback::initialize() {
    class_ = Callback::create_class("instrumentr_gc_unmark_callback");
    R_PreserveObject(class_);
}

void GcUnmarkCallback::finalize() {
    R_ReleaseObject(class_);
    class_ = NULL;
}

SEXP GcUnmarkCallback::get_class() {
    return class_;
}

void GcUnmarkCallback::invoke(SEXP r_context,
                              SEXP r_application,
                              SEXP r_object) {
    ContextSPtr context = from_sexp<Context>(r_context);
    fprintf(stderr, "GC Unmark Callback /n");

    if (is_c_callback()) {
        ApplicationSPtr application = from_sexp<Application>(r_application);

        callback_t callback = get_function<callback_t>();
        callback(context, application, r_object);
    }
    /**/
    else {
        SEXP r_function_name = get_function_name();
        SEXP r_environment = context->get_environment();

        Rf_eval(Rf_lang4(r_function_name, r_context, r_application, r_object),
                r_environment);
    }
}

} // namespace instrumentr
