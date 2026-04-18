#ifndef _MAIACPP_HOST_BRIDGE_
#define _MAIACPP_HOST_BRIDGE_

// Internal MaiaCpp runtime bridge for host-provided imports.
//
// Policy (MaiaC-first):
// 1. If MaiaC C89 stdlib already handles a feature, lower C++ to C and let
//    MaiaC/webc wire the host import automatically.
// 2. For functionality available in browser/Node but not in C89, prefer
//    declaring a __namespace__funcName host import and wrapping it here
//    rather than reimplementing in C++/WAT.
// 3. All wrappers are opt-in via MAIACPP_ENABLE_HOST_IMPORT_WRAPPERS so
//    native host builds compile without unresolved symbols.
//
// MaiaC C89 stdlib coverage (no wrappers needed, use C headers directly):
//   stdio  – printf, fprintf, fopen, fclose, fread, fwrite, fseek, ...
//   math   – sin, cos, sqrt, pow, floor, ceil, ...
//   time   – time, clock, localtime, gmtime, strftime, ...
//   string – strlen, strcpy, memcpy, memmove, memset, ...
//   stdlib – malloc, free, calloc, realloc, abort, exit, ...
//
// Host-only bridge wrappers below cover capabilities not expressible in C89:

#ifdef MAIACPP_ENABLE_HOST_IMPORT_WRAPPERS
extern "C" {
// --- Console (direct browser/node log channel, bypasses printf buffer) ---
void __console__log(const char* msg);
void __console__error(const char* msg);
void __console__warn(const char* msg);
// --- Performance time (sub-millisecond; Date.now() only gives ms in C time) ---
double __performance__now(void);
}
#endif

#include <cstddef.h>

namespace std {
namespace __maiacpp {

// --- Console wrappers ---

inline void host_console_log(const char* msg) {
#ifdef MAIACPP_ENABLE_HOST_IMPORT_WRAPPERS
    __console__log(msg);
#else
    (void)msg;
#endif
}

inline void host_console_error(const char* msg) {
#ifdef MAIACPP_ENABLE_HOST_IMPORT_WRAPPERS
    __console__error(msg);
#else
    (void)msg;
#endif
}

inline void host_console_warn(const char* msg) {
#ifdef MAIACPP_ENABLE_HOST_IMPORT_WRAPPERS
    __console__warn(msg);
#else
    (void)msg;
#endif
}

// --- Performance time wrapper ---
// Returns milliseconds with sub-ms resolution when the host provides it.
// Falls back to 0.0 in non-host builds.
inline double host_performance_now() {
#ifdef MAIACPP_ENABLE_HOST_IMPORT_WRAPPERS
    return __performance__now();
#else
    return 0.0;
#endif
}

} // namespace __maiacpp
} // namespace std

#endif
