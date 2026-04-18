#include <stdexcept.h>

namespace std {

logic_error::logic_error(const string&) {}

domain_error::domain_error(const string& what_arg)
    : logic_error(what_arg) {}

invalid_argument::invalid_argument(const string& what_arg)
    : logic_error(what_arg) {}

length_error::length_error(const string& what_arg)
    : logic_error(what_arg) {}

out_of_range::out_of_range(const string& what_arg)
    : logic_error(what_arg) {}

runtime_error::runtime_error(const string&) {}

range_error::range_error(const string& what_arg)
    : runtime_error(what_arg) {}

overflow_error::overflow_error(const string& what_arg)
    : runtime_error(what_arg) {}

underflow_error::underflow_error(const string& what_arg)
    : runtime_error(what_arg) {}

} // namespace std
