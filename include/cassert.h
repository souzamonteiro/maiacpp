// ISO/IEC 14882:1998(E) - 19.2 Assertions

#ifndef _CASSERT_
#define _CASSERT_

// Same as Standard C library header <assert.h>

#ifdef NDEBUG
#define assert(ignore) ((void)0)
#else
#define assert(expression) /* implementation-defined */
#endif

#endif