#include "Function.hpp"

namespace instrumentr {

SEXP Function::class_ = NULL;

void Function::initialize() {
    fprintf(stderr, "Function Initalize /n");

    class_ = Object::create_class({"instrumentr_function"});
    R_PreserveObject(class_);
}

void Function::finalize() {
    fprintf(stderr, "Function Finalize /n");

    R_ReleaseObject(class_);
    class_ = NULL;
}

SEXP Function::get_class() {
    return class_;
}

} // namespace instrumentr
