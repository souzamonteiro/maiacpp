// ISO/IEC 14882:1998(E) - 27.6.3 Standard manipulators

#ifndef _IOMANIP_
#define _IOMANIP_

#include <ios>

namespace std {

struct _Resetiosflags {
	explicit _Resetiosflags(ios_base::fmtflags m) : mask(m) {}
	ios_base::fmtflags mask;
};

struct _Setiosflags {
	explicit _Setiosflags(ios_base::fmtflags m) : mask(m) {}
	ios_base::fmtflags mask;
};

struct _Setbase {
	explicit _Setbase(int b) : base(b) {}
	int base;
};

template<class charT>
struct _Setfill {
	explicit _Setfill(charT c) : fill(c) {}
	charT fill;
};

struct _Setprecision {
	explicit _Setprecision(int n) : precision(n) {}
	int precision;
};

struct _Setw {
	explicit _Setw(int n) : width(n) {}
	int width;
};

inline _Resetiosflags resetiosflags(ios_base::fmtflags mask) { return _Resetiosflags(mask); }
inline _Setiosflags setiosflags(ios_base::fmtflags mask) { return _Setiosflags(mask); }
inline _Setbase setbase(int base) { return _Setbase(base); }
template<class charT> inline _Setfill<charT> setfill(charT c) { return _Setfill<charT>(c); }
inline _Setprecision setprecision(int n) { return _Setprecision(n); }
inline _Setw setw(int n) { return _Setw(n); }

} // namespace std

#endif