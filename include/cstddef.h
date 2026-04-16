// ISO/IEC 14882:1998(E) - 18.1 Types

#ifndef _CSTDDEF_
#define _CSTDDEF_

namespace std {

typedef int ptrdiff_t;
typedef unsigned int size_t;

} // namespace std

#define NULL 0 /* implementation-defined C++ null pointer constant (WASM/MaiaC profile) */
#define offsetof(type, member) ((std::size_t)&((type*)0)->member) /* implementation-defined (WASM/MaiaC profile) */

#endif