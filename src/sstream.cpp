// MaiaCpp sstream runtime – MaiaC-first implementation
//
// Provides explicit char specialisations for:
//   basic_stringbuf<char>        — streambuf backed by basic_string<char>
//   basic_istringstream<char>    — input-only string stream
//   basic_ostringstream<char>    — output-only string stream
//   basic_stringstream<char>     — bidirectional string stream
//
// All string storage is delegated to the basic_string<char> runtime in
// src/string.cpp.  No host imports or JS bridges are required.

#include <sstream.h>

namespace std {

// ---------------------------------------------------------------------------
// basic_stringbuf<char> – string-backed streambuf
// ---------------------------------------------------------------------------

template<>
basic_stringbuf<char>::basic_stringbuf(ios_base::openmode which)
    : _mode(which), _buf(), _gpos(0) {}

template<>
basic_stringbuf<char>::basic_stringbuf(const string& str, ios_base::openmode which)
    : _mode(which), _buf(str), _gpos(0) {}

template<>
string basic_stringbuf<char>::str() const {
    return _buf;
}

template<>
void basic_stringbuf<char>::str(const string& s) {
    _buf  = s;
    _gpos = 0;
}

// --- Input ---

template<>
basic_stringbuf<char>::int_type basic_stringbuf<char>::underflow() {
    if (!(_mode & ios_base::in)) return traits_type::eof();
    if (_gpos >= _buf.size()) return traits_type::eof();
    return traits_type::to_int_type(_buf[_gpos]);
}

template<>
basic_stringbuf<char>::int_type basic_stringbuf<char>::pbackfail(int_type c) {
    if (!(_mode & ios_base::in) || _gpos == 0) return traits_type::eof();
    --_gpos;
    if (!traits_type::eq_int_type(c, traits_type::eof())) {
        _buf[_gpos] = traits_type::to_char_type(c);
    }
    return traits_type::not_eof(c);
}

// --- Output ---

template<>
basic_stringbuf<char>::int_type basic_stringbuf<char>::overflow(int_type c) {
    if (!(_mode & ios_base::out)) return traits_type::eof();
    if (traits_type::eq_int_type(c, traits_type::eof())) return traits_type::not_eof(c);
    _buf.push_back(traits_type::to_char_type(c));
    return c;
}

// --- Seeking ---

template<>
basic_stringbuf<char>::pos_type
basic_stringbuf<char>::seekoff(off_type off, ios_base::seekdir way,
                                ios_base::openmode which) {
    size_t sz = _buf.size();
    size_t base = 0;
    if (way == ios_base::beg)       base = 0;
    else if (way == ios_base::cur)  base = _gpos;
    else if (way == ios_base::end)  base = sz;

    long newpos = (long)base + (long)off;
    if (newpos < 0 || (size_t)newpos > sz) return pos_type(-1);

    if (which & ios_base::in)  _gpos = (size_t)newpos;
    return pos_type(newpos);
}

template<>
basic_stringbuf<char>::pos_type
basic_stringbuf<char>::seekpos(pos_type sp, ios_base::openmode which) {
    return seekoff((off_type)sp, ios_base::beg, which);
}

template<>
basic_streambuf<char>* basic_stringbuf<char>::setbuf(char*, streamsize) {
    return this; // no-op for string-backed buffer
}

// ---------------------------------------------------------------------------
// basic_istringstream<char>
// ---------------------------------------------------------------------------

template<>
basic_istringstream<char>::basic_istringstream(ios_base::openmode which)
    : basic_istream<char>(&sb), sb(which | ios_base::in) {}

template<>
basic_istringstream<char>::basic_istringstream(const string& str, ios_base::openmode which)
    : basic_istream<char>(&sb), sb(str, which | ios_base::in) {}

template<>
basic_stringbuf<char>* basic_istringstream<char>::rdbuf() const {
    return const_cast<basic_stringbuf<char>*>(&sb);
}

template<>
string basic_istringstream<char>::str() const {
    return sb.str();
}

template<>
void basic_istringstream<char>::str(const string& s) {
    sb.str(s);
}

// ---------------------------------------------------------------------------
// basic_ostringstream<char>
// ---------------------------------------------------------------------------

template<>
basic_ostringstream<char>::basic_ostringstream(ios_base::openmode which)
    : basic_ostream<char>(&sb), sb(which | ios_base::out) {}

template<>
basic_ostringstream<char>::basic_ostringstream(const string& str, ios_base::openmode which)
    : basic_ostream<char>(&sb), sb(str, which | ios_base::out) {}

template<>
basic_stringbuf<char>* basic_ostringstream<char>::rdbuf() const {
    return const_cast<basic_stringbuf<char>*>(&sb);
}

template<>
string basic_ostringstream<char>::str() const {
    return sb.str();
}

template<>
void basic_ostringstream<char>::str(const string& s) {
    sb.str(s);
}

// ---------------------------------------------------------------------------
// basic_stringstream<char>
// ---------------------------------------------------------------------------

template<>
basic_stringstream<char>::basic_stringstream(ios_base::openmode which)
    : basic_iostream<char>(&sb), sb(which) {}

template<>
basic_stringstream<char>::basic_stringstream(const string& str, ios_base::openmode which)
    : basic_iostream<char>(&sb), sb(str, which) {}

template<>
basic_stringbuf<char>* basic_stringstream<char>::rdbuf() const {
    return const_cast<basic_stringbuf<char>*>(&sb);
}

template<>
string basic_stringstream<char>::str() const {
    return sb.str();
}

template<>
void basic_stringstream<char>::str(const string& s) {
    sb.str(s);
}

} // namespace std
