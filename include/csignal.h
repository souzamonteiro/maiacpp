// ISO/IEC 14882:1998(E) - 18.7 Other runtime support

#ifndef _CSIGNAL_
#define _CSIGNAL_

namespace std {

typedef int sig_atomic_t;

void (*signal(int sig, void (*handler)(int)))(int);
int raise(int sig);

} // namespace std

#define SIG_DFL ((void (*)(int))0)  /* implementation-defined (WASM/MaiaC profile) */
#define SIG_ERR ((void (*)(int))-1) /* implementation-defined (WASM/MaiaC profile) */
#define SIG_IGN ((void (*)(int))1)  /* implementation-defined (WASM/MaiaC profile) */
#define SIGABRT 1                   /* implementation-defined (WASM/MaiaC profile) */
#define SIGFPE  2                   /* implementation-defined (WASM/MaiaC profile) */
#define SIGILL  3                   /* implementation-defined (WASM/MaiaC profile) */
#define SIGINT  4                   /* implementation-defined (WASM/MaiaC profile) */
#define SIGSEGV 5                   /* implementation-defined (WASM/MaiaC profile) */
#define SIGTERM 6                   /* implementation-defined (WASM/MaiaC profile) */

#endif