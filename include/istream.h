// ISO/IEC 14882:1998(E) - 27.6.1 Input streams

#ifndef _ISTREAM_
#define _ISTREAM_

#include <ios>

namespace std {

template <class charT, class traits = char_traits<charT> >
class basic_istream : virtual public basic_ios<charT,traits> {
public:
    typedef charT char_type;
    typedef typename traits::int_type int_type;
    typedef typename traits::pos_type pos_type;
    typedef typename traits::off_type off_type;
    typedef traits traits_type;

    explicit basic_istream(basic_streambuf<charT,traits>* sb);
    virtual ~basic_istream();

    class sentry {
    public:
        explicit sentry(basic_istream<charT,traits>& is, bool noskipws = false);
        ~sentry();
        operator bool() const;
    };

    basic_istream<charT,traits>& operator>>(
        basic_istream<charT,traits>& (*pf)(basic_istream<charT,traits>&));
    basic_istream<charT,traits>& operator>>(
        basic_ios<charT,traits>& (*pf)(basic_ios<charT,traits>&));
    basic_istream<charT,traits>& operator>>(ios_base& (*pf)(ios_base&));
    basic_istream<charT,traits>& operator>>(bool& n);
    basic_istream<charT,traits>& operator>>(short& n);
    basic_istream<charT,traits>& operator>>(unsigned short& n);
    basic_istream<charT,traits>& operator>>(int& n);
    basic_istream<charT,traits>& operator>>(unsigned int& n);
    basic_istream<charT,traits>& operator>>(long& n);
    basic_istream<charT,traits>& operator>>(unsigned long& n);
    basic_istream<charT,traits>& operator>>(float& f);
    basic_istream<charT,traits>& operator>>(double& f);
    basic_istream<charT,traits>& operator>>(long double& f);
    basic_istream<charT,traits>& operator>>(void*& p);
    basic_istream<charT,traits>& operator>>(basic_streambuf<char_type,traits>* sb);

    streamsize gcount() const;
    int_type get();
    basic_istream<charT,traits>& get(char_type& c);
    basic_istream<charT,traits>& get(char_type* s, streamsize n);
    basic_istream<charT,traits>& get(char_type* s, streamsize n, char_type delim);
    basic_istream<charT,traits>& get(basic_streambuf<char_type,traits>& sb);
    basic_istream<charT,traits>& get(basic_streambuf<char_type,traits>& sb, char_type delim);
    basic_istream<charT,traits>& getline(char_type* s, streamsize n);
    basic_istream<charT,traits>& getline(char_type* s, streamsize n, char_type delim);
    basic_istream<charT,traits>& ignore(streamsize n = 1, int_type delim = traits::eof());
    int_type peek();
    basic_istream<charT,traits>& read(char_type* s, streamsize n);
    streamsize readsome(char_type* s, streamsize n);
    basic_istream<charT,traits>& putback(char_type c);
    basic_istream<charT,traits>& unget();
    int sync();
    pos_type tellg();
    basic_istream<charT,traits>& seekg(pos_type);
    basic_istream<charT,traits>& seekg(off_type, ios_base::seekdir);
};

template<class charT, class traits>
basic_istream<charT,traits>& operator>>(basic_istream<charT,traits>&, charT&);

template<class traits>
basic_istream<char,traits>& operator>>(basic_istream<char,traits>&, unsigned char&);

template<class traits>
basic_istream<char,traits>& operator>>(basic_istream<char,traits>&, signed char&);

template<class charT, class traits>
basic_istream<charT,traits>& operator>>(basic_istream<charT,traits>&, charT*);

template<class traits>
basic_istream<char,traits>& operator>>(basic_istream<char,traits>&, unsigned char*);

template<class traits>
basic_istream<char,traits>& operator>>(basic_istream<char,traits>&, signed char*);

template <class charT, class traits>
basic_istream<charT,traits>& ws(basic_istream<charT,traits>& is);

template <class charT, class traits = char_traits<charT> >
class basic_iostream : public basic_istream<charT,traits>,
                       public basic_ostream<charT,traits> {
public:
    explicit basic_iostream(basic_streambuf<charT,traits>* sb);
    virtual ~basic_iostream();
};

typedef basic_istream<char> istream;
typedef basic_istream<wchar_t> wistream;
typedef basic_iostream<char> iostream;
typedef basic_iostream<wchar_t> wiostream;

} // namespace std

#endif