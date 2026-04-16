// ISO/IEC 14882:1998(E) - 27.6.3 Standard manipulators

#ifndef _IOMANIP_
#define _IOMANIP_

#include <ios>

namespace std {

typedef unspecified T1;
typedef unspecified T2;
typedef unspecified T3;
typedef unspecified T4;
typedef unspecified T5;
typedef unspecified T6;

T1 resetiosflags(ios_base::fmtflags mask);
T2 setiosflags(ios_base::fmtflags mask);
T3 setbase(int base);
template<class charT> T4 setfill(charT c);
T5 setprecision(int n);
T6 setw(int n);

} // namespace std

#endif