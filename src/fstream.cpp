// MaiaCpp fstream runtime – MaiaC-first implementation
//
// Policy: delegate all file I/O to MaiaC C89 stdio (fopen/fclose/fread/fwrite).
// Only the char specialisation is instantiated; wide-char variants are stubs.
//
// This file provides explicit specialisations for:
//   basic_filebuf<char>   (filebuf)
//   basic_ifstream<char>  (ifstream)
//   basic_ofstream<char>  (ofstream)
//   basic_fstream<char>   (fstream)

#include <fstream.h>
#include <cstdio.h>
#include <cstring.h>
#include <cstdlib.h>

namespace std {

// ---------------------------------------------------------------------------
// Helpers: map ios_base::openmode → C fopen mode string
// ---------------------------------------------------------------------------

static const char* __maiacpp_openmode_str(ios_base::openmode m) {
    // Canonical combinations specified by C++98 Table 92.
    // binary flag maps to "b" suffix; we check it separately.
    const int in     = ios_base::in;
    const int out    = ios_base::out;
    const int trunc  = ios_base::trunc;
    const int app    = ios_base::app;
    const int binary = ios_base::binary;

    // Strip binary for the primary lookup, re-add below.
    const int base = m & ~binary;

    const char* mode = 0;

    if (base == in)                           mode = "r";
    else if (base == out)                     mode = "w";
    else if (base == (out | trunc))           mode = "w";
    else if (base == (out | app))             mode = "a";
    else if (base == app)                     mode = "a";
    else if (base == (in | out))              mode = "r+";
    else if (base == (in | out | trunc))      mode = "w+";
    else if (base == (in | out | app))        mode = "a+";
    else if (base == (in | app))              mode = "a+";
    else                                      mode = "r";   // fallback

    if (!(m & binary)) return mode;

    // Append 'b' for binary mode.  Use a small static table to avoid
    // allocating; there are at most 6 distinct base strings.
    if (mode[0] == 'r' && mode[1] == '\0') return "rb";
    if (mode[0] == 'w' && mode[1] == '\0') return "wb";
    if (mode[0] == 'a' && mode[1] == '\0') return "ab";
    if (mode[0] == 'r' && mode[1] == '+')  return "r+b";
    if (mode[0] == 'w' && mode[1] == '+')  return "w+b";
    if (mode[0] == 'a' && mode[1] == '+')  return "a+b";
    return "rb"; // fallback
}

// ---------------------------------------------------------------------------
// basic_filebuf<char> explicit specialisation
// ---------------------------------------------------------------------------

template<>
basic_filebuf<char>::basic_filebuf()
    : _file(0) {}

template<>
basic_filebuf<char>::~basic_filebuf() {
    if (_file) {
        ::fclose(_file);
        _file = 0;
    }
}

template<>
bool basic_filebuf<char>::is_open() const {
    return _file != 0;
}

template<>
basic_filebuf<char>* basic_filebuf<char>::open(const char* s, ios_base::openmode mode) {
    if (_file) return 0;  // already open
    const char* modestr = __maiacpp_openmode_str(mode);
    _file = ::fopen(s, modestr);
    if (!_file) return 0;
    // Handle ate: seek to end after open.
    if (mode & ios_base::ate) {
        if (::fseek(_file, 0, SEEK_END) != 0) {
            ::fclose(_file);
            _file = 0;
            return 0;
        }
    }
    return this;
}

template<>
basic_filebuf<char>* basic_filebuf<char>::close() {
    if (!_file) return 0;
    int result = ::fclose(_file);
    _file = 0;
    return (result == 0) ? this : 0;
}

// --- Input virtuals ---

template<>
basic_filebuf<char>::int_type basic_filebuf<char>::underflow() {
    if (!_file) return traits_type::eof();
    int c = ::fgetc(_file);
    if (c == EOF) return traits_type::eof();
    // Push back so a subsequent read gets the same char (required by underflow).
    ::ungetc(c, _file);
    return traits_type::to_int_type((char)c);
}

template<>
basic_filebuf<char>::int_type basic_filebuf<char>::uflow() {
    if (!_file) return traits_type::eof();
    int c = ::fgetc(_file);
    return (c == EOF) ? traits_type::eof() : traits_type::to_int_type((char)c);
}

template<>
basic_streambuf<char>::streamsize basic_filebuf<char>::xsgetn(char* s, streamsize n) {
    if (!_file || n <= 0) return 0;
    return (streamsize)::fread(s, 1, (size_t)n, _file);
}

template<>
basic_filebuf<char>::int_type basic_filebuf<char>::pbackfail(int_type c) {
    if (!_file) return traits_type::eof();
    if (traits_type::eq_int_type(c, traits_type::eof())) {
        // Attempt simple seek back one byte.
        if (::fseek(_file, -1, SEEK_CUR) != 0) return traits_type::eof();
        return traits_type::not_eof(c);
    }
    if (::ungetc(traits_type::to_char_type(c), _file) == EOF)
        return traits_type::eof();
    return c;
}

// --- Output virtuals ---

template<>
basic_filebuf<char>::int_type basic_filebuf<char>::overflow(int_type c) {
    if (!_file) return traits_type::eof();
    if (traits_type::eq_int_type(c, traits_type::eof())) return traits_type::not_eof(c);
    char ch = traits_type::to_char_type(c);
    if (::fputc((unsigned char)ch, _file) == EOF) return traits_type::eof();
    return c;
}

template<>
basic_streambuf<char>::streamsize basic_filebuf<char>::xsputn(const char* s, streamsize n) {
    if (!_file || n <= 0) return 0;
    return (streamsize)::fwrite(s, 1, (size_t)n, _file);
}

// --- Positioning ---

template<>
basic_filebuf<char>::pos_type
basic_filebuf<char>::seekoff(off_type off, ios_base::seekdir way,
                              ios_base::openmode which) {
    (void)which;
    if (!_file) return pos_type(-1);
    int whence = (way == ios_base::beg) ? SEEK_SET
               : (way == ios_base::cur) ? SEEK_CUR
               :                          SEEK_END;
    if (::fseek(_file, (long)off, whence) != 0) return pos_type(-1);
    long pos = ::ftell(_file);
    return (pos < 0) ? pos_type(-1) : pos_type(pos);
}

template<>
basic_filebuf<char>::pos_type
basic_filebuf<char>::seekpos(pos_type sp, ios_base::openmode which) {
    (void)which;
    if (!_file) return pos_type(-1);
    if (::fseek(_file, (long)sp, SEEK_SET) != 0) return pos_type(-1);
    long pos = ::ftell(_file);
    return (pos < 0) ? pos_type(-1) : pos_type(pos);
}

// --- Miscellaneous ---

template<>
int basic_filebuf<char>::sync() {
    if (!_file) return -1;
    return (::fflush(_file) == 0) ? 0 : -1;
}

template<>
basic_streambuf<char>::streamsize basic_filebuf<char>::showmanyc() {
    return 0; // conservative: no read-ahead estimate
}

template<>
basic_streambuf<char>* basic_filebuf<char>::setbuf(char* s, streamsize n) {
    (void)s;
    (void)n;
    // Buffering managed by MaiaC stdio layer; no-op here.
    return this;
}

template<>
void basic_filebuf<char>::imbue(const locale& loc) {
    (void)loc;
    // Locale imbue is a no-op in WASM profile.
}

// ---------------------------------------------------------------------------
// basic_ifstream<char> explicit specialisation
// ---------------------------------------------------------------------------

template<>
basic_ifstream<char>::basic_ifstream()
    : basic_istream<char>(&sb), sb() {}

template<>
basic_ifstream<char>::basic_ifstream(const char* s, ios_base::openmode mode)
    : basic_istream<char>(&sb), sb() {
    if (!sb.open(s, mode | ios_base::in)) {
        this->setstate(ios_base::failbit);
    }
}

template<>
basic_filebuf<char>* basic_ifstream<char>::rdbuf() const {
    return const_cast<basic_filebuf<char>*>(&sb);
}

template<>
bool basic_ifstream<char>::is_open() {
    return sb.is_open();
}

template<>
void basic_ifstream<char>::open(const char* s, ios_base::openmode mode) {
    if (!sb.open(s, mode | ios_base::in)) {
        this->setstate(ios_base::failbit);
    } else {
        this->clear();
    }
}

template<>
void basic_ifstream<char>::close() {
    if (!sb.close()) {
        this->setstate(ios_base::failbit);
    }
}

// ---------------------------------------------------------------------------
// basic_ofstream<char> explicit specialisation
// ---------------------------------------------------------------------------

template<>
basic_ofstream<char>::basic_ofstream()
    : basic_ostream<char>(&sb), sb() {}

template<>
basic_ofstream<char>::basic_ofstream(const char* s, ios_base::openmode mode)
    : basic_ostream<char>(&sb), sb() {
    if (!sb.open(s, mode | ios_base::out)) {
        this->setstate(ios_base::failbit);
    }
}

template<>
basic_filebuf<char>* basic_ofstream<char>::rdbuf() const {
    return const_cast<basic_filebuf<char>*>(&sb);
}

template<>
bool basic_ofstream<char>::is_open() {
    return sb.is_open();
}

template<>
void basic_ofstream<char>::open(const char* s, ios_base::openmode mode) {
    if (!sb.open(s, mode | ios_base::out)) {
        this->setstate(ios_base::failbit);
    } else {
        this->clear();
    }
}

template<>
void basic_ofstream<char>::close() {
    if (!sb.close()) {
        this->setstate(ios_base::failbit);
    }
}

// ---------------------------------------------------------------------------
// basic_fstream<char> explicit specialisation
// ---------------------------------------------------------------------------

template<>
basic_fstream<char>::basic_fstream()
    : basic_iostream<char>(&sb), sb() {}

template<>
basic_fstream<char>::basic_fstream(const char* s, ios_base::openmode mode)
    : basic_iostream<char>(&sb), sb() {
    if (!sb.open(s, mode)) {
        this->setstate(ios_base::failbit);
    }
}

template<>
basic_filebuf<char>* basic_fstream<char>::rdbuf() const {
    return const_cast<basic_filebuf<char>*>(&sb);
}

template<>
bool basic_fstream<char>::is_open() {
    return sb.is_open();
}

template<>
void basic_fstream<char>::open(const char* s, ios_base::openmode mode) {
    if (!sb.open(s, mode)) {
        this->setstate(ios_base::failbit);
    } else {
        this->clear();
    }
}

template<>
void basic_fstream<char>::close() {
    if (!sb.close()) {
        this->setstate(ios_base::failbit);
    }
}

} // namespace std
