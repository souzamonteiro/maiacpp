#include <exception.h>

#include <cstdlib.h>
#include <maiacpp_host_bridge.h>

namespace std {

static void __maiacpp_default_terminate_handler() {
    ::std::abort();
}

static void __maiacpp_default_unexpected_handler() {
    ::std::terminate();
}

static terminate_handler __maiacpp_terminate_handler = __maiacpp_default_terminate_handler;
static unexpected_handler __maiacpp_unexpected_handler = __maiacpp_default_unexpected_handler;

exception::exception() throw() {}
exception::exception(const exception&) throw() {}
exception& exception::operator=(const exception&) throw() { return *this; }
exception::~exception() throw() {}
const char* exception::what() const throw() { return "std::exception"; }

bad_exception::bad_exception() throw() {}
bad_exception::bad_exception(const bad_exception&) throw() {}
bad_exception& bad_exception::operator=(const bad_exception&) throw() { return *this; }
bad_exception::~bad_exception() throw() {}
const char* bad_exception::what() const throw() { return "std::bad_exception"; }

unexpected_handler set_unexpected(unexpected_handler f) throw() {
    unexpected_handler old = __maiacpp_unexpected_handler;
    __maiacpp_unexpected_handler = (f != 0) ? f : __maiacpp_default_unexpected_handler;
    return old;
}

void unexpected() {
    __maiacpp::host_console_error("std::unexpected invoked");
    __maiacpp_unexpected_handler();
}

terminate_handler set_terminate(terminate_handler f) throw() {
    terminate_handler old = __maiacpp_terminate_handler;
    __maiacpp_terminate_handler = (f != 0) ? f : __maiacpp_default_terminate_handler;
    return old;
}

void terminate() {
    __maiacpp::host_console_error("std::terminate invoked");
    __maiacpp_terminate_handler();
    ::std::abort();
}

bool uncaught_exception() {
    return false;
}

} // namespace std
