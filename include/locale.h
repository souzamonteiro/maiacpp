// ISO/IEC 14882:1998(E) - 22 Localization library
// Full declarations - multiple facets

#ifndef _LOCALE_
#define _LOCALE_

#include <string>
#include <iosfwd>
#include <ctime>

namespace std {

// 22.1.1 locale
class locale {
public:
    class facet;
    class id;
    typedef int category;
    static const category none = 0;
    static const category collate = 0x010;
    static const category ctype = 0x020;
    static const category monetary = 0x040;
    static const category numeric = 0x080;
    static const category time = 0x100;
    static const category messages = 0x200;
    static const category all = collate | ctype | monetary | numeric | time | messages;

    locale() throw();
    locale(const locale& other) throw();
    explicit locale(const char* std_name);
    locale(const locale& other, const char* std_name, category);
    template <class Facet> locale(const locale& other, Facet* f);
    locale(const locale& other, const locale& one, category);
    ~locale() throw();

    const locale& operator=(const locale& other) throw();
    template <class Facet> locale combine(const locale& other);
    basic_string<char> name() const;
    bool operator==(const locale& other) const;
    bool operator!=(const locale& other) const;
    template <class charT, class Traits, class Allocator>
    bool operator()(const basic_string<charT,Traits,Allocator>& s1,
                    const basic_string<charT,Traits,Allocator>& s2) const;

    static locale global(const locale&);
    static const locale& classic();
};

class locale::facet {
protected:
    explicit facet(size_t refs = 0);
    virtual ~facet();
};

class locale::id {
public:
    id();
};

template <class Facet> const Facet& use_facet(const locale&);
template <class Facet> bool has_facet(const locale&) throw();

template <class charT> bool isspace(charT c, const locale& loc);
template <class charT> bool isprint(charT c, const locale& loc);
template <class charT> bool iscntrl(charT c, const locale& loc);
template <class charT> bool isupper(charT c, const locale& loc);
template <class charT> bool islower(charT c, const locale& loc);
template <class charT> bool isalpha(charT c, const locale& loc);
template <class charT> bool isdigit(charT c, const locale& loc);
template <class charT> bool ispunct(charT c, const locale& loc);
template <class charT> bool isxdigit(charT c, const locale& loc);
template <class charT> bool isalnum(charT c, const locale& loc);
template <class charT> bool isgraph(charT c, const locale& loc);
template <class charT> charT toupper(charT c, const locale& loc);
template <class charT> charT tolower(charT c, const locale& loc);

// 22.2.1 ctype category
class ctype_base {
public:
    enum mask {
        space = 1<<0, print = 1<<1, cntrl = 1<<2,
        upper = 1<<3, lower = 1<<4, alpha = 1<<5,
        digit = 1<<6, punct = 1<<7, xdigit = 1<<8,
        alnum = alpha|digit, graph = alnum|punct
    };
};

template <class charT> class ctype : public locale::facet, public ctype_base {
public:
    typedef charT char_type;
    explicit ctype(size_t refs = 0);
    bool is(mask m, charT c) const;
    const charT* is(const charT* low, const charT* high, mask* vec) const;
    const charT* scan_is(mask m, const charT* low, const charT* high) const;
    const charT* scan_not(mask m, const charT* low, const charT* high) const;
    charT toupper(charT c) const;
    const charT* toupper(charT* low, const charT* high) const;
    charT tolower(charT c) const;
    const charT* tolower(charT* low, const charT* high) const;
    charT widen(char c) const;
    const char* widen(const char* low, const char* high, charT* to) const;
    char narrow(charT c, char dfault) const;
    const charT* narrow(const charT* low, const charT*, char dfault, char* to) const;
    static locale::id id;
protected:
    ~ctype();
    virtual bool do_is(mask m, charT c) const;
    virtual const charT* do_is(const charT* low, const charT* high, mask* vec) const;
    virtual const charT* do_scan_is(mask m, const charT* low, const charT* high) const;
    virtual const charT* do_scan_not(mask m, const charT* low, const charT* high) const;
    virtual charT do_toupper(charT) const;
    virtual const charT* do_toupper(charT* low, const charT* high) const;
    virtual charT do_tolower(charT) const;
    virtual const charT* do_tolower(charT* low, const charT* high) const;
    virtual charT do_widen(char) const;
    virtual const char* do_widen(const char* low, const char* high, charT* dest) const;
    virtual char do_narrow(charT, char dfault) const;
    virtual const charT* do_narrow(const charT* low, const charT* high, char dfault, char* dest) const;
};

template <> class ctype<char> : public locale::facet, public ctype_base {
public:
    typedef char char_type;
    explicit ctype(const mask* tab = 0, bool del = false, size_t refs = 0);
    bool is(mask m, char c) const;
    const char* is(const char* low, const char* high, mask* vec) const;
    const char* scan_is(mask m, const char* low, const char* high) const;
    const char* scan_not(mask m, const char* low, const char* high) const;
    char toupper(char c) const;
    const char* toupper(char* low, const char* high) const;
    char tolower(char c) const;
    const char* tolower(char* low, const char* high) const;
    char widen(char c) const;
    const char* widen(const char* low, const char* high, char* to) const;
    char narrow(char c, char dfault) const;
    const char* narrow(const char* low, const char* high, char dfault, char* to) const;
    static locale::id id;
    static const size_t table_size = implementation_defined;
protected:
    const mask* table() const throw();
    static const mask* classic_table() throw();
    ~ctype();
    virtual char do_toupper(char c) const;
    virtual const char* do_toupper(char* low, const char* high) const;
    virtual char do_tolower(char c) const;
    virtual const char* do_tolower(char* low, const char* high) const;
    virtual char do_widen(char c) const;
    virtual const char* do_widen(const char* low, const char* high, char* to) const;
    virtual char do_narrow(char c, char dfault) const;
    virtual const char* do_narrow(const char* low, const char* high, char dfault, char* to) const;
};

template <class charT> class ctype_byname : public ctype<charT> {
public:
    typedef typename ctype<charT>::mask mask;
    explicit ctype_byname(const char*, size_t refs = 0);
protected:
    ~ctype_byname();
};

template <> class ctype_byname<char> : public ctype<char> {
public:
    explicit ctype_byname(const char*, size_t refs = 0);
protected:
    ~ctype_byname();
};

class codecvt_base {
public:
    enum result { ok, partial, error, noconv };
};

template <class internT, class externT, class stateT>
class codecvt : public locale::facet, public codecvt_base {
public:
    typedef internT intern_type;
    typedef externT extern_type;
    typedef stateT state_type;
    explicit codecvt(size_t refs = 0);
    result out(stateT& state, const internT* from, const internT* from_end,
               const internT*& from_next, externT* to, externT* to_limit,
               externT*& to_next) const;
    result unshift(stateT& state, externT* to, externT* to_limit, externT*& to_next) const;
    result in(stateT& state, const externT* from, const externT* from_end,
              const externT*& from_next, internT* to, internT* to_limit,
              internT*& to_next) const;
    int encoding() const throw();
    bool always_noconv() const throw();
    int length(const stateT&, const externT* from, const externT* end, size_t max) const;
    int max_length() const throw();
    static locale::id id;
protected:
    ~codecvt();
    virtual result do_out(stateT& state, const internT* from, const internT* from_end,
                          const internT*& from_next, externT* to, externT* to_limit,
                          externT*& to_next) const;
    virtual result do_in(stateT& state, const externT* from, const externT* from_end,
                         const externT*& from_next, internT* to, internT* to_limit,
                         internT*& to_next) const;
    virtual result do_unshift(stateT& state, externT* to, externT* to_limit,
                              externT*& to_next) const;
    virtual int do_encoding() const throw();
    virtual bool do_always_noconv() const throw();
    virtual int do_length(const stateT&, const externT* from, const externT* end, size_t max) const;
    virtual int do_max_length() const throw();
};

template <class internT, class externT, class stateT>
class codecvt_byname : public codecvt<internT, externT, stateT> {
public:
    explicit codecvt_byname(const char*, size_t refs = 0);
protected:
    ~codecvt_byname();
};

// 22.2.2 numeric category
template <class charT, class InputIterator = istreambuf_iterator<charT> >
class num_get : public locale::facet {
public:
    typedef charT char_type;
    typedef InputIterator iter_type;
    explicit num_get(size_t refs = 0);
    iter_type get(iter_type in, iter_type end, ios_base&, ios_base::iostate& err, bool& v) const;
    iter_type get(iter_type in, iter_type end, ios_base&, ios_base::iostate& err, long& v) const;
    iter_type get(iter_type in, iter_type end, ios_base&, ios_base::iostate& err,
                  unsigned short& v) const;
    iter_type get(iter_type in, iter_type end, ios_base&, ios_base::iostate& err,
                  unsigned int& v) const;
    iter_type get(iter_type in, iter_type end, ios_base&, ios_base::iostate& err,
                  unsigned long& v) const;
    iter_type get(iter_type in, iter_type end, ios_base&, ios_base::iostate& err, float& v) const;
    iter_type get(iter_type in, iter_type end, ios_base&, ios_base::iostate& err, double& v) const;
    iter_type get(iter_type in, iter_type end, ios_base&, ios_base::iostate& err,
                  long double& v) const;
    iter_type get(iter_type in, iter_type end, ios_base&, ios_base::iostate& err, void*& v) const;
    static locale::id id;
protected:
    ~num_get();
    virtual iter_type do_get(iter_type, iter_type, ios_base&, ios_base::iostate&, bool&) const;
    virtual iter_type do_get(iter_type, iter_type, ios_base&, ios_base::iostate&, long&) const;
    virtual iter_type do_get(iter_type, iter_type, ios_base&, ios_base::iostate&,
                             unsigned short&) const;
    virtual iter_type do_get(iter_type, iter_type, ios_base&, ios_base::iostate&,
                             unsigned int&) const;
    virtual iter_type do_get(iter_type, iter_type, ios_base&, ios_base::iostate&,
                             unsigned long&) const;
    virtual iter_type do_get(iter_type, iter_type, ios_base&, ios_base::iostate&, float&) const;
    virtual iter_type do_get(iter_type, iter_type, ios_base&, ios_base::iostate&, double&) const;
    virtual iter_type do_get(iter_type, iter_type, ios_base&, ios_base::iostate&,
                             long double&) const;
    virtual iter_type do_get(iter_type, iter_type, ios_base&, ios_base::iostate&, void*&) const;
};

template <class charT, class OutputIterator = ostreambuf_iterator<charT> >
class num_put : public locale::facet {
public:
    typedef charT char_type;
    typedef OutputIterator iter_type;
    explicit num_put(size_t refs = 0);
    iter_type put(iter_type s, ios_base& f, char_type fill, bool v) const;
    iter_type put(iter_type s, ios_base& f, char_type fill, long v) const;
    iter_type put(iter_type s, ios_base& f, char_type fill, unsigned long v) const;
    iter_type put(iter_type s, ios_base& f, char_type fill, double v) const;
    iter_type put(iter_type s, ios_base& f, char_type fill, long double v) const;
    iter_type put(iter_type s, ios_base& f, char_type fill, const void* v) const;
    static locale::id id;
protected:
    ~num_put();
    virtual iter_type do_put(iter_type, ios_base&, char_type fill, bool v) const;
    virtual iter_type do_put(iter_type, ios_base&, char_type fill, long v) const;
    virtual iter_type do_put(iter_type, ios_base&, char_type fill, unsigned long) const;
    virtual iter_type do_put(iter_type, ios_base&, char_type fill, double v) const;
    virtual iter_type do_put(iter_type, ios_base&, char_type fill, long double v) const;
    virtual iter_type do_put(iter_type, ios_base&, char_type fill, const void* v) const;
};

template <class charT> class numpunct : public locale::facet {
public:
    typedef charT char_type;
    typedef basic_string<charT> string_type;
    explicit numpunct(size_t refs = 0);
    char_type decimal_point() const;
    char_type thousands_sep() const;
    string grouping() const;
    string_type truename() const;
    string_type falsename() const;
    static locale::id id;
protected:
    ~numpunct();
    virtual char_type do_decimal_point() const;
    virtual char_type do_thousands_sep() const;
    virtual string do_grouping() const;
    virtual string_type do_truename() const;
    virtual string_type do_falsename() const;
};

template <class charT> class numpunct_byname : public numpunct<charT> {
public:
    typedef basic_string<charT> string_type;
    explicit numpunct_byname(const char*, size_t refs = 0);
protected:
    ~numpunct_byname();
};

// 22.2.4 collate category
template <class charT> class collate : public locale::facet {
public:
    typedef charT char_type;
    typedef basic_string<charT> string_type;
    explicit collate(size_t refs = 0);
    int compare(const charT* low1, const charT* high1,
                const charT* low2, const charT* high2) const;
    string_type transform(const charT* low, const charT* high) const;
    long hash(const charT* low, const charT* high) const;
    static locale::id id;
protected:
    ~collate();
    virtual int do_compare(const charT* low1, const charT* high1,
                           const charT* low2, const charT* high2) const;
    virtual string_type do_transform(const charT* low, const charT* high) const;
    virtual long do_hash(const charT* low, const charT* high) const;
};

template <class charT> class collate_byname : public collate<charT> {
public:
    typedef basic_string<charT> string_type;
    explicit collate_byname(const char*, size_t refs = 0);
protected:
    ~collate_byname();
};

// 22.2.5 time category
class time_base {
public:
    enum dateorder { no_order, dmy, mdy, ymd, ydm };
};

template <class charT, class InputIterator = istreambuf_iterator<charT> >
class time_get : public locale::facet, public time_base {
public:
    typedef charT char_type;
    typedef InputIterator iter_type;
    explicit time_get(size_t refs = 0);
    dateorder date_order() const;
    iter_type get_time(iter_type s, iter_type end, ios_base& f,
                       ios_base::iostate& err, tm* t) const;
    iter_type get_date(iter_type s, iter_type end, ios_base& f,
                       ios_base::iostate& err, tm* t) const;
    iter_type get_weekday(iter_type s, iter_type end, ios_base& f,
                          ios_base::iostate& err, tm* t) const;
    iter_type get_monthname(iter_type s, iter_type end, ios_base& f,
                            ios_base::iostate& err, tm* t) const;
    iter_type get_year(iter_type s, iter_type end, ios_base& f,
                       ios_base::iostate& err, tm* t) const;
    static locale::id id;
protected:
    ~time_get();
    virtual dateorder do_date_order() const;
    virtual iter_type do_get_time(iter_type s, iter_type end, ios_base&,
                                  ios_base::iostate& err, tm* t) const;
    virtual iter_type do_get_date(iter_type s, iter_type end, ios_base&,
                                  ios_base::iostate& err, tm* t) const;
    virtual iter_type do_get_weekday(iter_type s, iter_type end, ios_base&,
                                     ios_base::iostate& err, tm* t) const;
    virtual iter_type do_get_monthname(iter_type s, iter_type end, ios_base&,
                                       ios_base::iostate& err, tm* t) const;
    virtual iter_type do_get_year(iter_type s, iter_type end, ios_base&,
                                  ios_base::iostate& err, tm* t) const;
};

template <class charT, class InputIterator = istreambuf_iterator<charT> >
class time_get_byname : public time_get<charT, InputIterator> {
public:
    explicit time_get_byname(const char*, size_t refs = 0);
protected:
    ~time_get_byname();
};

template <class charT, class OutputIterator = ostreambuf_iterator<charT> >
class time_put : public locale::facet {
public:
    typedef charT char_type;
    typedef OutputIterator iter_type;
    explicit time_put(size_t refs = 0);
    iter_type put(iter_type s, ios_base& f, char_type fill, const tm* tmb,
                  const charT* pattern, const charT* pat_end) const;
    iter_type put(iter_type s, ios_base& f, char_type fill,
                  const tm* tmb, char format, char modifier = 0) const;
    static locale::id id;
protected:
    ~time_put();
    virtual iter_type do_put(iter_type s, ios_base&, char_type, const tm* t,
                             char format, char modifier) const;
};

template <class charT, class OutputIterator = ostreambuf_iterator<charT> >
class time_put_byname : public time_put<charT, OutputIterator> {
public:
    explicit time_put_byname(const char*, size_t refs = 0);
protected:
    ~time_put_byname();
};

// 22.2.6 monetary category
class money_base {
public:
    enum part { none, space, symbol, sign, value };
    struct pattern { char field[4]; };
};

template <class charT, class InputIterator = istreambuf_iterator<charT> >
class money_get : public locale::facet {
public:
    typedef charT char_type;
    typedef InputIterator iter_type;
    typedef basic_string<charT> string_type;
    explicit money_get(size_t refs = 0);
    iter_type get(iter_type s, iter_type end, bool intl, ios_base& f,
                  ios_base::iostate& err, long double& units) const;
    iter_type get(iter_type s, iter_type end, bool intl, ios_base& f,
                  ios_base::iostate& err, string_type& digits) const;
    static locale::id id;
protected:
    ~money_get();
    virtual iter_type do_get(iter_type, iter_type, bool, ios_base&,
                             ios_base::iostate& err, long double& units) const;
    virtual iter_type do_get(iter_type, iter_type, bool, ios_base&,
                             ios_base::iostate& err, string_type& digits) const;
};

template <class charT, class OutputIterator = ostreambuf_iterator<charT> >
class money_put : public locale::facet {
public:
    typedef charT char_type;
    typedef OutputIterator iter_type;
    typedef basic_string<charT> string_type;
    explicit money_put(size_t refs = 0);
    iter_type put(iter_type s, bool intl, ios_base& f,
                  char_type fill, long double units) const;
    iter_type put(iter_type s, bool intl, ios_base& f,
                  char_type fill, const string_type& digits) const;
    static locale::id id;
protected:
    ~money_put();
    virtual iter_type do_put(iter_type, bool, ios_base&, char_type fill,
                             long double units) const;
    virtual iter_type do_put(iter_type, bool, ios_base&, char_type fill,
                             const string_type& digits) const;
};

template <class charT, bool International = false>
class moneypunct : public locale::facet, public money_base {
public:
    typedef charT char_type;
    typedef basic_string<charT> string_type;
    explicit moneypunct(size_t refs = 0);
    charT decimal_point() const;
    charT thousands_sep() const;
    string grouping() const;
    string_type curr_symbol() const;
    string_type positive_sign() const;
    string_type negative_sign() const;
    int frac_digits() const;
    pattern pos_format() const;
    pattern neg_format() const;
    static locale::id id;
    static const bool intl = International;
protected:
    ~moneypunct();
    virtual charT do_decimal_point() const;
    virtual charT do_thousands_sep() const;
    virtual string do_grouping() const;
    virtual string_type do_curr_symbol() const;
    virtual string_type do_positive_sign() const;
    virtual string_type do_negative_sign() const;
    virtual int do_frac_digits() const;
    virtual pattern do_pos_format() const;
    virtual pattern do_neg_format() const;
};

template <class charT, bool Intl = false>
class moneypunct_byname : public moneypunct<charT, Intl> {
public:
    explicit moneypunct_byname(const char*, size_t refs = 0);
protected:
    ~moneypunct_byname();
};

// 22.2.7 messages category
class messages_base {
public:
    typedef int catalog;
};

template <class charT>
class messages : public locale::facet, public messages_base {
public:
    typedef charT char_type;
    typedef basic_string<charT> string_type;
    explicit messages(size_t refs = 0);
    catalog open(const basic_string<char>& fn, const locale&) const;
    string_type get(catalog c, int set, int msgid, const string_type& dfault) const;
    void close(catalog c) const;
    static locale::id id;
protected:
    ~messages();
    virtual catalog do_open(const basic_string<char>&, const locale&) const;
    virtual string_type do_get(catalog, int set, int msgid, const string_type& dfault) const;
    virtual void do_close(catalog) const;
};

template <class charT>
class messages_byname : public messages<charT> {
public:
    explicit messages_byname(const char*, size_t refs = 0);
protected:
    ~messages_byname();
};

} // namespace std

#endif