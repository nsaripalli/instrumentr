#include "../inst/include/Function.hpp"

namespace lightr {

SEXP Function::class_ = nullptr;

void Function::initialize() {
    class_ = mkString("lightr_function");
    R_PreserveObject(class_);
}

SEXP Function::get_class() {
    return class_;
}

FunctionSPtr Function::from_sexp(SEXP r_function) {
    void* function = R_ExternalPtrAddr(r_function);
    if (function == nullptr) {
        // TODO error;
    }
    return *static_cast<FunctionSPtr*>(function);
}

SEXP Function::to_sexp(FunctionSPtr function) {
    SEXP r_function = PROTECT(
        R_MakeExternalPtr(new FunctionSPtr(function), R_NilValue, R_NilValue));

    R_RegisterCFinalizerEx(r_function, Function::destroy_sexp, TRUE);

    setAttrib(r_function, R_ClassSymbol, Function::get_class());

    UNPROTECT(1);

    return r_function;
}

void Function::destroy_sexp(SEXP r_function) {
    delete static_cast<FunctionSPtr*>(R_ExternalPtrAddr(r_function));
    R_SetExternalPtrAddr(r_function, nullptr);
}

} // namespace lightr