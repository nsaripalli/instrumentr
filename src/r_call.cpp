#include "../inst/include/Call.hpp"
#include "../inst/include/Function.hpp"
#include "../inst/include/Parameter.hpp"
#include "r_api.h"
#include "utilities.h"

using lightr::Argument;
using lightr::ArgumentSPtr;
using lightr::Call;
using lightr::CallSPtr;
using lightr::Function;
using lightr::FunctionSPtr;
using lightr::Parameter;
using lightr::ParameterSPtr;

ArgumentSPtr create_argument(SEXP r_argument_name,
                             SEXP r_argument_value,
                             SEXP r_environment) {
    /* value (promise optimized away by compiler)  */
    if (TYPEOF(r_argument_value) != PROMSXP) {
        SEXP r_argument_name_str =
            PROTECT(mkString(CHAR(PRINTNAME(r_argument_name))));

        r_argument_value = delayed_assign(r_argument_name,
                                          r_argument_name_str,
                                          r_argument_value,
                                          r_environment,
                                          r_environment,
                                          r_environment);
        UNPROTECT(1);
    }

    ArgumentSPtr argument =
        std::make_shared<Argument>(CHAR(PRINTNAME(r_argument_name)));

    argument->set_value(r_argument_value);

    return argument;
}

SEXP r_call_create(SEXP r_function, SEXP r_call_obj, SEXP r_environment) {
    FunctionSPtr function = Function::from_sexp(r_function);
    CallSPtr call = std::make_shared<Call>(function, r_call_obj, r_environment);
    SEXP r_function_obj = function->get_object();

    std::string argument_name;
    SEXP r_parameters = FORMALS(r_function_obj);

    for (int parameter_position = 0; r_parameters != R_NilValue;
         ++parameter_position, r_parameters = CDR(r_parameters)) {
        SEXP r_argument_name = TAG(r_parameters);
        SEXP r_argument_value =
            Rf_findVarInFrame(r_environment, r_argument_name);

        ParameterSPtr parameter = std::make_shared<Parameter>(
            R_CHAR(PRINTNAME(r_argument_name)), parameter_position);

        call->add_parameter(parameter);

        /* missing argument */
        if (r_argument_value == R_UnboundValue ||
            r_argument_value == R_MissingArg) {
            /* NOTE: do nothing */
        }
        /* ... parameter */
        else if (TYPEOF(r_argument_value) == DOTSXP) {
            std::vector<SEXP> dot_arguments;
            std::vector<SEXP> dot_tags;

            int index = 0;

            for (SEXP r_dot_argument_pointer = r_argument_value;
                 r_dot_argument_pointer != R_NilValue;
                 r_dot_argument_pointer = CDR(r_dot_argument_pointer)) {
                SEXP r_dot_argument_tag = TAG(r_dot_argument_pointer);

                SEXP r_dot_argument_value = CAR(r_dot_argument_pointer);

                if (TYPEOF(r_dot_argument_value) != PROMSXP) {
                    dot_arguments.push_back(PROTECT(create_promise(
                        r_dot_argument_value,
                        /*FIXME: this should be caller env!*/ r_environment)));
                } else {
                    dot_arguments.push_back(PROTECT(r_dot_argument_value));
                }

                dot_tags.push_back(r_dot_argument_tag);

                ++index;
            }

            SEXP dots = PROTECT(R_NilValue);

            for (int i = dot_arguments.size() - 1; i >= 0; --i) {
                SEXP r_dot_argument_value = dot_arguments[i];
                SEXP r_dot_argument_tag = dot_tags[i];
                std::string argument_name = "";

                if (r_dot_argument_tag == R_NilValue ||
                    r_dot_argument_tag == R_MissingArg ||
                    r_dot_argument_tag == R_UnboundValue) {
                    argument_name = "";
                } else {
                    argument_name = CHAR(PRINTNAME(r_dot_argument_tag));
                }

                UNPROTECT(1);
                dots = PROTECT(CONS(r_dot_argument_value, dots));
                SET_TAG(dots, r_dot_argument_tag);

                ArgumentSPtr argument =
                    std::make_shared<Argument>(argument_name);

                argument->set_value(r_dot_argument_value);

                parameter->add_argument(argument);
            }

            SET_TYPEOF(dots, DOTSXP);

            Rf_setVar(R_DotsSymbol, dots, r_environment);

            UNPROTECT(1);

            UNPROTECT(dot_arguments.size());
        }
        /* non ... parameter  */
        else {
            // argument_name = CHAR(PRINTNAME(r_argument_name));
            parameter->add_argument(create_argument(
                r_argument_name, r_argument_value, r_environment));
        }
    }

    return Call::to_sexp(call);
}

SEXP r_call_get_function(SEXP r_call) {
    CallSPtr call = Call::from_sexp(r_call);
    return Function::to_sexp(call->get_function());
}

SEXP r_call_get_object(SEXP r_call) {
    CallSPtr call = Call::from_sexp(r_call);
    return call->get_object();
}

SEXP r_call_get_environment(SEXP r_call) {
    CallSPtr call = Call::from_sexp(r_call);
    return call->get_environment();
}

SEXP r_call_get_state(SEXP r_call) {
    CallSPtr call = Call::from_sexp(r_call);
    return mkString(call_state_to_string(call->get_state()).c_str());
}

SEXP r_call_get_parameters(SEXP r_call) {
    CallSPtr call = Call::from_sexp(r_call);
    const std::vector<std::shared_ptr<Parameter>>& parameters =
        call->get_parameters();
    SEXP r_parameters = PROTECT(allocVector(VECSXP, parameters.size()));
    for (int i = 0; i < parameters.size(); ++i) {
        SET_VECTOR_ELT(r_parameters, i, Parameter::to_sexp(parameters.at(i)));
    }
    UNPROTECT(1);
    return r_parameters;
}
