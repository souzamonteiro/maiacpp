// ISO/IEC 14882:1998(E) - 27.4 Iostreams base classes

#ifndef _IOS_
#define _IOS_

#include <iosfwd>
#include <locale>

namespace std {

typedef OFF_T streamoff;
typedef SZ_T streamsize;

template <class stateT> class fpos;

class ios_base {
public:
    class failure;
    typedef T1 fmtflags;
    static const fmtflags boolalpha;
    static const fmtflags dec;
    static const fmtflags fixed;
    static const fmtflags hex;
    static const fmtflags internal;
    static const fmtflags left;
    static const fmtflags oct;
    static const fmtflags right;
    static const fmtflags scientific;
    static const fmtflags showbase;
    static const fmtflags showpoint;
    static const fmtflags showpos;
    static const fmtflags skipws;
    static const fmtflags unitbuf;
    static const fmtflags uppercase;
    static const fmtflags adjustfield;
    static const fmtflags basefield;
    static const fmtflags floatfield;

    typedef T2 iostate;
    static const iostate badbit;
    static const iostate eofbit;
    static const iostate failbit;
    static const iostate goodbit;

    typedef T3 openmode;
    static const openmode app;
    static const openmode ate;
    static const openmode binary;
    static const openmode in;
    static const openmode out;
    static const openmode trunc;

    typedef T4 seekdir;
    static const seekdir beg;
    static const seekdir cur;
    static const seekdir end;

    class Init;

    fmtflags flags() const;
    fmtflags flags(fmtflags fmtfl);
    fmtflags setf(fmtflags fmtfl);
    fmtflags setf(fmtflags fmtfl, fmtflags mask);
    void unsetf(fmtflags mask);
    streamsize precision() const;
    streamsize precision(streamsize prec);
    streamsize width() const;
    streamsize width(streamsize wide);

    locale imbue(const locale& loc);
    locale getloc() const;

    static int xalloc();
    long& iword(int index);
    void*& pword(int index);

    virtual ~ios_base();

    enum event { erase_event, imbue_event, copyfmt_event };
    typedef void (*event_callback)(event, ios_base&, int index);
    void register_callback(event_callback fn, int index);
    static bool sync_with_stdio(bool sync = true);

protected:
    ios_base();
};

template <class charT, class traits = char_traits<charT> >
class basic_ios : public ios_base {
public:
    typedef charT char_type;
    typedef typename traits::int_type int_type;
    typedef typename traits::pos_type pos_type;
    typedef typename traits::off_type off_type;
    typedef traits traits_type;

    operator void*() const;
    bool operator!() const;
    iostate rdstate() const;
    void clear(iostate state = goodbit);
    void setstate(iostate state);
    bool good() const;
    bool eof() const;
    bool fail() const;
    bool bad() const;
    iostate exceptions() const;
    void exceptions(iostate except);

    explicit basic_ios(basic_streambuf<charT,traits>* sb);
    virtual ~basic_ios();

    basic_ostream<charT,traits>* tie() const;
    basic_ostream<charT,traits>* tie(basic_ostream<charT,traits>* tiestr);
    basic_streambuf<charT,traits>* rdbuf() const;
    basic_streambuf<charT,traits>* rdbuf(basic_streambuf<charT,traits>* sb);
    basic_ios& copyfmt(const basic_ios& rhs);
    char_type fill() const;
    char_type fill(char_type ch);

    locale imbue(const locale& loc);
    char narrow(char_type c, char dfault) const;
    char_type widen(char c) const;

protected:
    basic_ios();
    void init(basic_streambuf<charT,traits>* sb);
};

ios_base& boolalpha(ios_base& str);
ios_base& noboolalpha(ios_base& str);
ios_base& showbase(ios_base& str);
ios_base& noshowbase(ios_base& str);
ios_base& showpoint(ios_base& str);
ios_base& noshowpoint(ios_base& str);
ios_base& showpos(ios_base& str);
ios_base& noshowpos(ios_base& str);
ios_base& skipws(ios_base& str);
ios_base& noskipws(ios_base& str);
ios_base& uppercase(ios_base& str);
ios_base& nouppercase(ios_base& str);
ios_base& unitbuf(ios_base& str);
ios_base& nounitbuf(ios_base& str);

ios_base& internal(ios_base& str);
ios_base& left(ios_base& str);
ios_base& right(ios_base& str);

ios_base& dec(ios_base& str);
ios_base& hex(ios_base& str);
ios_base& oct(ios_base& str);

ios_base& fixed(ios_base& str);
ios_base& scientific(ios_base& str);

} // namespace std

#endif