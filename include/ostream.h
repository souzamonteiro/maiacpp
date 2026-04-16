// ISO/IEC 14882:1998(E) - 27.6.2 Output streams

#ifndef _OSTREAM_
#define _OSTREAM_

#include <ios>

namespace std {

template <class charT, class traits = char_traits<charT> >
class basic_ostream : virtual public basic_ios<charT,traits> {
public:
    typedef charT char_type;
    typedef typename traits::int_type int_type;
    typedef typename traits::pos_type pos_type;
    typedef typename traits::off_type off_type;
    typedef traits traits_type;

    explicit basic_ostream(basic_streambuf<char_type,traits>* sb);
    virtual ~basic_ostream();

    class sentry {
    public:
        explicit sentry(basic_ostream<charT,traits>& os);
        ~sentry();
        operator bool() const;
    };

    basic_ostream<charT,traits>& operator<<(
        basic_ostream<charT,traits>& (*pf)(basic_ostream<charT,traits>&));
    basic_ostream<charT,traits>& operator<<(
        basic_ios<charT,traits>& (*pf)(basic_ios<charT,traits>&));
    basic_ostream<charT,traits>& operator<<(ios_base& (*pf)(ios_base&));
    basic_ostream<charT,traits>& operator<<(bool n);
    basic_ostream<charT,traits>& operator<<(short n);
    basic_ostream<charT,traits>& operator<<(unsigned short n);
    basic_ostream<charT,traits>& operator<<(int n);
    basic_ostream<charT,traits>& operator<<(unsigned int n);
    basic_ostream<charT,traits>& operator<<(long n);
    basic_ostream<charT,traits>& operator<<(unsigned long n);
    basic_ostream<charT,traits>& operator<<(float f);
    basic_ostream<charT,traits>& operator<<(double f);
    basic_ostream<charT,traits>& operator<<(long double f);
    basic_ostream<charT,traits>& operator<<(const void* p);
    basic_ostream<charT,traits>& operator<<(basic_streambuf<char_type,traits>* sb);

    basic_ostream<charT,traits>& put(char_type c);
    basic_ostream<charT,traits>& write(const char_type* s, streamsize n);
    basic_ostream<charT,traits>& flush();

    pos_type tellp();
    basic_ostream<charT,traits>& seekp(pos_type);
    basic_ostream<charT,traits>& seekp(off_type, ios_base::seekdir);
};

template<class charT, class traits>
basic_ostream<charT,traits>& operator<<(basic_ostream<charT,traits>&, charT);

template<class charT, class traits>
basic_ostream<charT,traits>& operator<<(basic_ostream<charT,traits>&, char);

template<class traits>
basic_ostream<char,traits>& operator<<(basic_ostream<char,traits>&, char);

template<class traits>
basic_ostream<char,traits>& operator<<(basic_ostream<char,traits>&, signed char);

template<class traits>
basic_ostream<char,traits>& operator<<(basic_ostream<char,traits>&, unsigned char);

template<class charT, class traits>
basic_ostream<charT,traits>& operator<<(basic_ostream<charT,traits>&, const charT*);

template<class charT, class traits>
basic_ostream<charT,traits>& operator<<(basic_ostream<charT,traits>&, const char*);

template<class traits>
basic_ostream<char,traits>& operator<<(basic_ostream<char,traits>&, const char*);

template<class traits>
basic_ostream<char,traits>& operator<<(basic_ostream<char,traits>&, const signed char*);

template<class traits>
basic_ostream<char,traits>& operator<<(basic_ostream<char,traits>&, const unsigned char*);

template <class charT, class traits>
basic_ostream<charT,traits>& endl(basic_ostream<charT,traits>& os);

template <class charT, class traits>
basic_ostream<charT,traits>& ends(basic_ostream<charT,traits>& os);

template <class charT, class traits>
basic_ostream<charT,traits>& flush(basic_ostream<charT,traits>& os);

typedef basic_ostream<char> ostream;
typedef basic_ostream<wchar_t> wostream;

} // namespace std

#endif