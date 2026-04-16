// ISO/IEC 14882:1998(E) - 18.7 Other runtime support

#ifndef _CSTDARG_
#define _CSTDARG_

namespace std {

typedef /* implementation-defined */ va_list;

} // namespace std

#define va_start(ap, param) /* implementation-defined */
#define va_arg(ap, type)    /* implementation-defined */
#define va_end(ap)          /* implementation-defined */
#define va_copy(dest, src)  /* implementation-defined (C99) */

#endif