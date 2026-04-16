// ISO/IEC 14882:1998(E) - 18.7 Other runtime support

#ifndef _CSIGNAL_
#define _CSIGNAL_

namespace std {

typedef /* implementation-defined */ sig_atomic_t;

void (*signal(int sig, void (*handler)(int)))(int);
int raise(int sig);

} // namespace std

#define SIG_DFL /* implementation-defined */
#define SIG_ERR /* implementation-defined */
#define SIG_IGN /* implementation-defined */
#define SIGABRT /* implementation-defined */
#define SIGFPE  /* implementation-defined */
#define SIGILL  /* implementation-defined */
#define SIGINT  /* implementation-defined */
#define SIGSEGV /* implementation-defined */
#define SIGTERM /* implementation-defined */

#endif