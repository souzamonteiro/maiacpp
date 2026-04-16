// ISO/IEC 14882:1998(E) - 18.7 Other runtime support

#ifndef _CSTDARG_
#define _CSTDARG_

namespace std {

typedef char* va_list;

} // namespace std

#define va_start(ap, param) ((void)(param), (ap = (std::va_list)__maiac_va_base)) /* implementation-defined (WASM/MaiaC profile) */
#define va_arg(ap, type)    (*(type *)((ap += sizeof(type)) - sizeof(type)))      /* implementation-defined (WASM/MaiaC profile) */
#define va_end(ap)          (ap = (std::va_list)0)                                 /* implementation-defined (WASM/MaiaC profile) */
#define va_copy(dest, src)  ((dest) = (src))                                       /* implementation-defined (WASM/MaiaC profile) */

#endif