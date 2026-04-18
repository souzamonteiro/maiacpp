#include <typeinfo.h>

namespace std {

type_info::~type_info() {}

bool type_info::operator==(const type_info& rhs) const {
    return this == &rhs;
}

bool type_info::operator!=(const type_info& rhs) const {
    return !(*this == rhs);
}

bool type_info::before(const type_info& rhs) const {
    return this < &rhs;
}

const char* type_info::name() const {
    return "type_info";
}

bad_cast::bad_cast() throw() {}
bad_cast::bad_cast(const bad_cast&) throw() {}
bad_cast& bad_cast::operator=(const bad_cast&) throw() { return *this; }
bad_cast::~bad_cast() throw() {}
const char* bad_cast::what() const throw() { return "std::bad_cast"; }

bad_typeid::bad_typeid() throw() {}
bad_typeid::bad_typeid(const bad_typeid&) throw() {}
bad_typeid& bad_typeid::operator=(const bad_typeid&) throw() { return *this; }
bad_typeid::~bad_typeid() throw() {}
const char* bad_typeid::what() const throw() { return "std::bad_typeid"; }

} // namespace std
