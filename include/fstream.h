// ISO/IEC 14882:1998(E) - 27.8 File streams

#ifndef _FSTREAM_
#define _FSTREAM_

#include <ios>
#include <istream>
#include <ostream>
#include <streambuf>

namespace std {

template <class charT, class traits = char_traits<charT> >
class basic_filebuf : public basic_streambuf<charT,traits> {
public:
    typedef charT char_type;
    typedef typename traits::int_type int_type;
    typedef typename traits::pos_type pos_type;
    typedef typename traits::off_type off_type;
    typedef traits traits_type;

    basic_filebuf();
    virtual ~basic_filebuf();

    bool is_open() const;
    basic_filebuf<charT,traits>* open(const char* s, ios_base::openmode mode);
    basic_filebuf<charT,traits>* close();

protected:
    virtual streamsize showmanyc();
    virtual int_type underflow();
    virtual int_type uflow();
    virtual int_type pbackfail(int_type c = traits::eof());
    virtual int_type overflow(int_type c = traits::eof());
    virtual basic_streambuf<charT,traits>* setbuf(char_type* s, streamsize n);
    virtual pos_type seekoff(off_type off, ios_base::seekdir way,
                             ios_base::openmode which = ios_base::in | ios_base::out);
    virtual pos_type seekpos(pos_type sp,
                             ios_base::openmode which = ios_base::in | ios_base::out);
    virtual int sync();
    virtual void imbue(const locale& loc);
};

template <class charT, class traits = char_traits<charT> >
class basic_ifstream : public basic_istream<charT,traits> {
public:
    typedef charT char_type;
    typedef typename traits::int_type int_type;
    typedef typename traits::pos_type pos_type;
    typedef typename traits::off_type off_type;
    typedef traits traits_type;

    basic_ifstream();
    explicit basic_ifstream(const char* s, ios_base::openmode mode = ios_base::in);

    basic_filebuf<charT,traits>* rdbuf() const;
    bool is_open();
    void open(const char* s, ios_base::openmode mode = ios_base::in);
    void close();

private:
    basic_filebuf<charT,traits> sb;
};

template <class charT, class traits = char_traits<charT> >
class basic_ofstream : public basic_ostream<charT,traits> {
public:
    typedef charT char_type;
    typedef typename traits::int_type int_type;
    typedef typename traits::pos_type pos_type;
    typedef typename traits::off_type off_type;
    typedef traits traits_type;

    basic_ofstream();
    explicit basic_ofstream(const char* s, ios_base::openmode mode = ios_base::out);

    basic_filebuf<charT,traits>* rdbuf() const;
    bool is_open();
    void open(const char* s, ios_base::openmode mode = ios_base::out);
    void close();

private:
    basic_filebuf<charT,traits> sb;
};

template <class charT, class traits = char_traits<charT> >
class basic_fstream : public basic_iostream<charT,traits> {
public:
    typedef charT char_type;
    typedef typename traits::int_type int_type;
    typedef typename traits::pos_type pos_type;
    typedef typename traits::off_type off_type;
    typedef traits traits_type;

    basic_fstream();
    explicit basic_fstream(const char* s, ios_base::openmode mode = ios_base::in | ios_base::out);

    basic_filebuf<charT,traits>* rdbuf() const;
    bool is_open();
    void open(const char* s, ios_base::openmode mode = ios_base::in | ios_base::out);
    void close();

private:
    basic_filebuf<charT,traits> sb;
};

typedef basic_filebuf<char> filebuf;
typedef basic_filebuf<wchar_t> wfilebuf;
typedef basic_ifstream<char> ifstream;
typedef basic_ifstream<wchar_t> wifstream;
typedef basic_ofstream<char> ofstream;
typedef basic_ofstream<wchar_t> wofstream;
typedef basic_fstream<char> fstream;
typedef basic_fstream<wchar_t> wfstream;

} // namespace std

#endif