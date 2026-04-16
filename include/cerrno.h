// ISO/IEC 14882:1998(E) - 19.3 Error numbers

#ifndef _CERRNO_
#define _CERRNO_

namespace std {
extern int errno;
} // namespace std

#define errno  (::std::errno) /* implementation-defined (WASM/MaiaC profile) */
#define EDOM   1              /* implementation-defined (WASM/MaiaC profile) */
#define ERANGE 2              /* implementation-defined (WASM/MaiaC profile) */

#endif