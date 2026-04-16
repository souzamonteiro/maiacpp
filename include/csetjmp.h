// ISO/IEC 14882:1998(E) - 18.7 Other runtime support

#ifndef _CSETJMP_
#define _CSETJMP_

namespace std {

typedef int jmp_buf[16];

int setjmp(jmp_buf env);

void longjmp(jmp_buf env, int val);

} // namespace std

#define setjmp(env) std::setjmp(env) /* implementation-defined (WASM/MaiaC profile) */

#endif