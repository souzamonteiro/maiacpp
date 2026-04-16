// ISO/IEC 14882:1998(E) - 19.1 Exception classes

#ifndef _STDEXCEPT_
#define _STDEXCEPT_

#include <string>
#include <exception>

namespace std {

class logic_error : public exception {
public:
    explicit logic_error(const string& what_arg);
};

class domain_error : public logic_error {
public:
    explicit domain_error(const string& what_arg);
};

class invalid_argument : public logic_error {
public:
    explicit invalid_argument(const string& what_arg);
};

class length_error : public logic_error {
public:
    explicit length_error(const string& what_arg);
};

class out_of_range : public logic_error {
public:
    explicit out_of_range(const string& what_arg);
};

class runtime_error : public exception {
public:
    explicit runtime_error(const string& what_arg);
};

class range_error : public runtime_error {
public:
    explicit range_error(const string& what_arg);
};

class overflow_error : public runtime_error {
public:
    explicit overflow_error(const string& what_arg);
};

class underflow_error : public runtime_error {
public:
    explicit underflow_error(const string& what_arg);
};

} // namespace std

#endif