// MaiaCpp iostream runtime – MaiaC-first implementation
//
// Policy: route all stream I/O through MaiaC C89 stdio (printf/fprintf/fgets).
// No custom buffering or locale logic is reimplemented here; those layers
// are delegated to the MaiaC/webc runtime host bridge.
//
// This file provides:
//   - __MaiacppStdioBuf : concrete basic_streambuf<char> for stdout/stderr/stdin
//   - Definitions of the standard stream objects: cin, cout, cerr, clog

#include <iostream.h>
#include <streambuf.h>
#include <istream.h>
#include <ostream.h>
#include <cstdio.h>
#include <cstring.h>

namespace std {

// ---------------------------------------------------------------------------
// Concrete streambuf backed by MaiaC stdio FILE handles.
// Only xsputn/overflow (output) and underflow/xsgetn (input) are implemented.
// All other virtual hooks return the no-op / error sentinel.
// ---------------------------------------------------------------------------

class __MaiacppStdioBuf : public basic_streambuf<char> {
public:
    // 1 = stdout, 2 = stderr, 3 = stdin (matches MaiaC stdio handle ids)
    explicit __MaiacppStdioBuf(int fd) : _fd(fd) {}

protected:
    // Output path: delegate to MaiaC fwrite
    virtual int_type overflow(int_type c) {
        if (traits_type::eq_int_type(c, traits_type::eof()))
            return traits_type::not_eof(c);
        char ch = traits_type::to_char_type(c);
        if (_fd == 1 || _fd == 2) {
            // Route through MaiaC stdio which MaiaC/webc wires to host write
            ::putchar(ch);
        }
        return c;
    }

    virtual streamsize xsputn(const char* s, streamsize n) {
        if (n <= 0) return 0;
        if (_fd == 1) {
            // fwrite to stdout – MaiaC routes to host write
            return (streamsize)::fwrite(s, 1, (size_t)n, stdout);
        }
        if (_fd == 2) {
            return (streamsize)::fwrite(s, 1, (size_t)n, stderr);
        }
        return 0;
    }

    // Input path: delegate to MaiaC fread / getchar
    virtual int_type underflow() {
        if (_fd != 3) return traits_type::eof();
        int c = ::getchar();
        if (c == EOF) return traits_type::eof();
        return traits_type::to_int_type((char)c);
    }

    virtual streamsize xsgetn(char* s, streamsize n) {
        if (_fd != 3 || n <= 0) return 0;
        streamsize count = 0;
        while (count < n) {
            int c = ::getchar();
            if (c == EOF) break;
            s[count++] = (char)c;
        }
        return count;
    }

    virtual int sync() { return 0; }

private:
    int _fd;
};

// ---------------------------------------------------------------------------
// Standard stream objects
// These are declared as extern in iostream.h; we provide the definitions here.
// ---------------------------------------------------------------------------

static __MaiacppStdioBuf __maiacpp_cout_buf(1);
static __MaiacppStdioBuf __maiacpp_cerr_buf(2);
static __MaiacppStdioBuf __maiacpp_clog_buf(2);
static __MaiacppStdioBuf __maiacpp_cin_buf(3);

ostream  cout(&__maiacpp_cout_buf);
ostream  cerr(&__maiacpp_cerr_buf);
ostream  clog(&__maiacpp_clog_buf);
istream  cin(&__maiacpp_cin_buf);

// Wide-char stream stubs (wired to same buffers; wide conversion is not
// supported in this WASM profile – declared to satisfy link requirements).
wostream wcout(0);
wostream wcerr(0);
wostream wclog(0);
wistream wcin(0);

} // namespace std
