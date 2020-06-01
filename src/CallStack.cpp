#include "../inst/include/CallStack.hpp"

namespace lightr {

SEXP CallStack::class_ = nullptr;

void CallStack::initialize() {
    class_ = mkString("lightr_call_stack");
    R_PreserveObject(class_);
}

SEXP CallStack::get_class() {
    return class_;
}

CallStackSPtr CallStack::from_sexp(SEXP r_call_stack) {
    void* call = R_ExternalPtrAddr(r_call_stack);
    if (call == nullptr) {
        // TODO error;
    }
    return *static_cast<CallStackSPtr*>(call);
}

SEXP CallStack::to_sexp(CallStackSPtr call) {
    SEXP r_call_stack = PROTECT(
        R_MakeExternalPtr(new CallStackSPtr(call), R_NilValue, R_NilValue));

    R_RegisterCFinalizerEx(r_call_stack, CallStack::destroy_sexp, TRUE);

    setAttrib(r_call_stack, R_ClassSymbol, CallStack::get_class());

    UNPROTECT(1);

    return r_call_stack;
}

void CallStack::destroy_sexp(SEXP r_call_stack) {
    delete static_cast<CallStackSPtr*>(R_ExternalPtrAddr(r_call_stack));
    R_SetExternalPtrAddr(r_call_stack, nullptr);
}

} // namespace lightr