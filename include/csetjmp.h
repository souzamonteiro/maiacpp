// ISO/IEC 14882:1998(E) - 18.7 Other runtime support

#ifndef _CSETJMP_
#define _CSETJMP_

namespace std {

typedef /* implementation-defined */ jmp_buf;

void longjmp(jmp_buf env, int val);

} // namespace std

#define setjmp(env) /* implementation-defined */

#endif