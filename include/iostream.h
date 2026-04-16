// ISO/IEC 14882:1998(E) - 27.3 Standard iostream objects

#ifndef _IOSTREAM_
#define _IOSTREAM_

#include <ios>
#include <istream>
#include <ostream>

namespace std {

extern istream cin;
extern ostream cout;
extern ostream cerr;
extern ostream clog;
extern wistream wcin;
extern wostream wcout;
extern wostream wcerr;
extern wostream wclog;

} // namespace std

#endif