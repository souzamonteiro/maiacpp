// ISO/IEC 14882:1998(E) - 27.2 Forward declarations

#ifndef _IOSFWD_
#define _IOSFWD_

#include <cstddef>

namespace std {

template<class charT> class char_traits;

template <class stateT> class fpos {
public:
    fpos(long off = 0) : __off(off), __state() {}
    fpos(const fpos& other) : __off(other.__off), __state(other.__state) {}
    fpos& operator=(const fpos& other) {
        __off = other.__off;
        __state = other.__state;
        return *this;
    }
    operator long() const { return __off; }
    stateT state() const { return __state; }
    void state(stateT s) { __state = s; }
private:
    long __off;
    stateT __state;
};

template<> class char_traits<char> {
public:
    typedef char char_type;
    typedef unsigned int int_type;
    typedef long off_type;
    typedef int state_type; /* provisional WASM/MaiaC profile */
    typedef fpos<state_type> pos_type;

    static void assign(char_type& c1, const char_type& c2) { c1 = c2; }
    static bool eq(const char_type& c1, const char_type& c2) { return c1 == c2; }
    static bool lt(const char_type& c1, const char_type& c2) { return c1 < c2; }
    static int compare(const char_type* s1, const char_type* s2, size_t n) {
        for (size_t i = 0; i < n; ++i) {
            if (lt(s1[i], s2[i])) return -1;
            if (lt(s2[i], s1[i])) return 1;
        }
        return 0;
    }
    static size_t length(const char_type* s) {
        size_t n = 0;
        while (!eq(s[n], char_type())) ++n;
        return n;
    }
    static const char_type* find(const char_type* s, size_t n, const char_type& a) {
        for (size_t i = 0; i < n; ++i) if (eq(s[i], a)) return s + i;
        return 0;
    }
    static char_type* move(char_type* s1, const char_type* s2, size_t n) {
        if (s1 < s2) {
            for (size_t i = 0; i < n; ++i) s1[i] = s2[i];
        } else if (s1 > s2) {
            for (size_t i = n; i != 0; --i) s1[i - 1] = s2[i - 1];
        }
        return s1;
    }
    static char_type* copy(char_type* s1, const char_type* s2, size_t n) {
        for (size_t i = 0; i < n; ++i) s1[i] = s2[i];
        return s1;
    }
    static char_type* assign(char_type* s, size_t n, char_type a) {
        for (size_t i = 0; i < n; ++i) s[i] = a;
        return s;
    }
    static char_type to_char_type(const int_type& c) { return static_cast<char_type>(c); }
    static int_type to_int_type(const char_type& c) { return static_cast<unsigned char>(c); }
    static bool eq_int_type(const int_type& c1, const int_type& c2) { return c1 == c2; }
    static int_type eof() { return static_cast<int_type>(-1); }
    static int_type not_eof(const int_type& c) { return eq_int_type(c, eof()) ? 0 : c; }
};

template<> class char_traits<wchar_t> {
public:
    typedef wchar_t char_type;
    typedef unsigned int int_type;
    typedef long off_type;
    typedef int state_type; /* provisional WASM/MaiaC profile */
    typedef fpos<state_type> pos_type;

    static void assign(char_type& c1, const char_type& c2) { c1 = c2; }
    static bool eq(const char_type& c1, const char_type& c2) { return c1 == c2; }
    static bool lt(const char_type& c1, const char_type& c2) { return c1 < c2; }
    static int compare(const char_type* s1, const char_type* s2, size_t n) {
        for (size_t i = 0; i < n; ++i) {
            if (lt(s1[i], s2[i])) return -1;
            if (lt(s2[i], s1[i])) return 1;
        }
        return 0;
    }
    static size_t length(const char_type* s) {
        size_t n = 0;
        while (!eq(s[n], char_type())) ++n;
        return n;
    }
    static const char_type* find(const char_type* s, size_t n, const char_type& a) {
        for (size_t i = 0; i < n; ++i) if (eq(s[i], a)) return s + i;
        return 0;
    }
    static char_type* move(char_type* s1, const char_type* s2, size_t n) {
        if (s1 < s2) {
            for (size_t i = 0; i < n; ++i) s1[i] = s2[i];
        } else if (s1 > s2) {
            for (size_t i = n; i != 0; --i) s1[i - 1] = s2[i - 1];
        }
        return s1;
    }
    static char_type* copy(char_type* s1, const char_type* s2, size_t n) {
        for (size_t i = 0; i < n; ++i) s1[i] = s2[i];
        return s1;
    }
    static char_type* assign(char_type* s, size_t n, char_type a) {
        for (size_t i = 0; i < n; ++i) s[i] = a;
        return s;
    }
    static char_type to_char_type(const int_type& c) { return static_cast<char_type>(c); }
    static int_type to_int_type(const char_type& c) { return static_cast<int_type>(c); }
    static bool eq_int_type(const int_type& c1, const int_type& c2) { return c1 == c2; }
    static int_type eof() { return static_cast<int_type>(-1); }
    static int_type not_eof(const int_type& c) { return eq_int_type(c, eof()) ? 0 : c; }
};

template<class T> class allocator;

template <class charT, class traits = char_traits<charT> >
class basic_ios;

template <class charT, class traits = char_traits<charT> >
class basic_streambuf;

template <class charT, class traits = char_traits<charT> >
class basic_istream;

template <class charT, class traits = char_traits<charT> >
class basic_ostream;

template <class charT, class traits = char_traits<charT> >
class basic_iostream;

template <class charT, class traits = char_traits<charT>,
          class Allocator = allocator<charT> >
class basic_stringbuf;

template <class charT, class traits = char_traits<charT>,
          class Allocator = allocator<charT> >
class basic_istringstream;

template <class charT, class traits = char_traits<charT>,
          class Allocator = allocator<charT> >
class basic_ostringstream;

template <class charT, class traits = char_traits<charT>,
          class Allocator = allocator<charT> >
class basic_stringstream;

template <class charT, class traits = char_traits<charT> >
class basic_filebuf;

template <class charT, class traits = char_traits<charT> >
class basic_ifstream;

template <class charT, class traits = char_traits<charT> >
class basic_ofstream;

template <class charT, class traits = char_traits<charT> >
class basic_fstream;

template <class charT, class traits = char_traits<charT> >
class istreambuf_iterator;

template <class charT, class traits = char_traits<charT> >
class ostreambuf_iterator;

typedef basic_ios<char> ios;
typedef basic_ios<wchar_t> wios;
typedef basic_streambuf<char> streambuf;
typedef basic_istream<char> istream;
typedef basic_ostream<char> ostream;
typedef basic_iostream<char> iostream;
typedef basic_stringbuf<char> stringbuf;
typedef basic_istringstream<char> istringstream;
typedef basic_ostringstream<char> ostringstream;
typedef basic_stringstream<char> stringstream;
typedef basic_filebuf<char> filebuf;
typedef basic_ifstream<char> ifstream;
typedef basic_ofstream<char> ofstream;
typedef basic_fstream<char> fstream;
typedef basic_streambuf<wchar_t> wstreambuf;
typedef basic_istream<wchar_t> wistream;
typedef basic_ostream<wchar_t> wostream;
typedef basic_iostream<wchar_t> wiostream;
typedef basic_stringbuf<wchar_t> wstringbuf;
typedef basic_istringstream<wchar_t> wistringstream;
typedef basic_ostringstream<wchar_t> wostringstream;
typedef basic_stringstream<wchar_t> wstringstream;
typedef basic_filebuf<wchar_t> wfilebuf;
typedef basic_ifstream<wchar_t> wifstream;
typedef basic_ofstream<wchar_t> wofstream;
typedef basic_fstream<wchar_t> wfstream;

typedef fpos<char_traits<char>::state_type> streampos;
typedef fpos<char_traits<wchar_t>::state_type> wstreampos;

} // namespace std

#endif