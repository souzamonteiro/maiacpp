// MaiaCpp ios runtime – state management for ios_base and basic_ios<char>
//
// Provides:
//   ios_base    – fmtflags, iostate, openmode, seekdir state and manipulators
//   basic_ios<char> – rdstate, setstate, clear, rdbuf, tie, fill, good/fail/bad/eof
//
// No host imports required; all state is maintained in WASM linear memory.

#include <ios.h>

namespace std {

// ---------------------------------------------------------------------------
// ios_base
// ---------------------------------------------------------------------------

ios_base::ios_base()
    : _flags(dec | skipws), _precision(6), _width(0),
      _state(goodbit), _exceptions(goodbit) {}

// fmtflags
ios_base::fmtflags ios_base::flags() const           { return _flags; }
ios_base::fmtflags ios_base::flags(fmtflags f)        { fmtflags old = _flags; _flags = f; return old; }
ios_base::fmtflags ios_base::setf(fmtflags f)         { fmtflags old = _flags; _flags |= f; return old; }
ios_base::fmtflags ios_base::setf(fmtflags f, fmtflags mask) {
    fmtflags old = _flags;
    _flags = (_flags & ~mask) | (f & mask);
    return old;
}
void ios_base::unsetf(fmtflags mask) { _flags &= ~mask; }

// width / precision
streamsize ios_base::precision() const              { return _precision; }
streamsize ios_base::precision(streamsize p)        { streamsize old = _precision; _precision = p; return old; }
streamsize ios_base::width() const                  { return _width; }
streamsize ios_base::width(streamsize w)            { streamsize old = _width; _width = w; return old; }

// locale / xalloc / iword / pword – minimal stubs (no locale/iarray)
locale ios_base::imbue(const locale&)              { return locale(); }
locale ios_base::getloc() const                    { return locale(); }
int    ios_base::xalloc()                          { return 0; }
long&  ios_base::iword(int)                        { static long dummy = 0; return dummy; }
void*& ios_base::pword(int)                        { static void* dummy = 0; return dummy; }

void   ios_base::register_callback(event_callback, int) {}
bool   ios_base::sync_with_stdio(bool)             { return true; }

// ---------------------------------------------------------------------------
// basic_ios<char>
// ---------------------------------------------------------------------------

template<>
basic_ios<char>::basic_ios(basic_streambuf<char>* sb)
    : _sb(0), _tie(0), _fill(' ') {
    init(sb);
}

template<>
basic_ios<char>::basic_ios()
    : _sb(0), _tie(0), _fill(' ') {}

template<>
basic_ios<char>::~basic_ios() {}

template<>
void basic_ios<char>::init(basic_streambuf<char>* sb) {
    _sb          = sb;
    _tie         = 0;
    _fill        = ' ';
    _state       = sb ? goodbit : badbit;
    _exceptions  = goodbit;
    _flags       = dec | skipws;
    _precision   = 6;
    _width       = 0;
}

// State
template<>
ios_base::iostate basic_ios<char>::rdstate() const { return _state; }

template<>
void basic_ios<char>::clear(iostate state) {
    _state = state;
    if (_state & _exceptions) {
        // In a full implementation we would throw here.
        // In the WASM profile we silently absorb the exception condition.
    }
}

template<>
void basic_ios<char>::setstate(iostate state) {
    clear(_state | state);
}

template<>
bool basic_ios<char>::good() const { return _state == goodbit; }
template<>
bool basic_ios<char>::eof()  const { return (_state & eofbit)  != 0; }
template<>
bool basic_ios<char>::fail() const { return (_state & (failbit | badbit)) != 0; }
template<>
bool basic_ios<char>::bad()  const { return (_state & badbit)  != 0; }

// operator void* / operator!
template<>
basic_ios<char>::operator void*() const { return fail() ? 0 : const_cast<basic_ios<char>*>(this); }
template<>
bool basic_ios<char>::operator!() const { return fail(); }

// exceptions
template<>
ios_base::iostate basic_ios<char>::exceptions() const          { return _exceptions; }
template<>
void basic_ios<char>::exceptions(iostate e)                    { _exceptions = e; }

// rdbuf / tie / fill
template<>
basic_streambuf<char>* basic_ios<char>::rdbuf() const          { return _sb; }

template<>
basic_streambuf<char>* basic_ios<char>::rdbuf(basic_streambuf<char>* sb) {
    basic_streambuf<char>* old = _sb;
    _sb = sb;
    _state = sb ? goodbit : badbit;
    return old;
}

template<>
basic_ostream<char>* basic_ios<char>::tie() const              { return _tie; }

template<>
basic_ostream<char>* basic_ios<char>::tie(basic_ostream<char>* t) {
    basic_ostream<char>* old = _tie;
    _tie = t;
    return old;
}

template<>
char basic_ios<char>::fill() const                             { return _fill; }

template<>
char basic_ios<char>::fill(char c)                             { char old = _fill; _fill = c; return old; }

// copyfmt
template<>
basic_ios<char>& basic_ios<char>::copyfmt(const basic_ios<char>& rhs) {
    _tie        = rhs._tie;
    _fill       = rhs._fill;
    _flags      = rhs._flags;
    _precision  = rhs._precision;
    _width      = rhs._width;
    _exceptions = rhs._exceptions;
    return *this;
}

// narrow / widen (identity for char specialisation)
template<>
char basic_ios<char>::narrow(char c, char) const { return c; }
template<>
char basic_ios<char>::widen(char c)         const { return c; }

// imbue (no-op stub)
template<>
locale basic_ios<char>::imbue(const locale&) { return locale(); }

// ---------------------------------------------------------------------------
// ios_base manipulators
// ---------------------------------------------------------------------------

ios_base& boolalpha(ios_base& s)    { s.setf(ios_base::boolalpha);   return s; }
ios_base& noboolalpha(ios_base& s)  { s.unsetf(ios_base::boolalpha); return s; }
ios_base& showbase(ios_base& s)     { s.setf(ios_base::showbase);    return s; }
ios_base& noshowbase(ios_base& s)   { s.unsetf(ios_base::showbase);  return s; }
ios_base& showpoint(ios_base& s)    { s.setf(ios_base::showpoint);   return s; }
ios_base& noshowpoint(ios_base& s)  { s.unsetf(ios_base::showpoint); return s; }
ios_base& showpos(ios_base& s)      { s.setf(ios_base::showpos);     return s; }
ios_base& noshowpos(ios_base& s)    { s.unsetf(ios_base::showpos);   return s; }
ios_base& skipws(ios_base& s)       { s.setf(ios_base::skipws);      return s; }
ios_base& noskipws(ios_base& s)     { s.unsetf(ios_base::skipws);    return s; }
ios_base& uppercase(ios_base& s)    { s.setf(ios_base::uppercase);   return s; }
ios_base& nouppercase(ios_base& s)  { s.unsetf(ios_base::uppercase); return s; }
ios_base& unitbuf(ios_base& s)      { s.setf(ios_base::unitbuf);     return s; }
ios_base& nounitbuf(ios_base& s)    { s.unsetf(ios_base::unitbuf);   return s; }

ios_base& internal(ios_base& s)     { s.setf(ios_base::internal, ios_base::adjustfield); return s; }
ios_base& left(ios_base& s)         { s.setf(ios_base::left,     ios_base::adjustfield); return s; }
ios_base& right(ios_base& s)        { s.setf(ios_base::right,    ios_base::adjustfield); return s; }

ios_base& dec(ios_base& s)          { s.setf(ios_base::dec, ios_base::basefield); return s; }
ios_base& hex(ios_base& s)          { s.setf(ios_base::hex, ios_base::basefield); return s; }
ios_base& oct(ios_base& s)          { s.setf(ios_base::oct, ios_base::basefield); return s; }

ios_base& fixed(ios_base& s)        { s.setf(ios_base::fixed,      ios_base::floatfield); return s; }
ios_base& scientific(ios_base& s)   { s.setf(ios_base::scientific, ios_base::floatfield); return s; }

} // namespace std
