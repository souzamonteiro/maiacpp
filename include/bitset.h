// ISO/IEC 14882:1998(E) - 23.3.5 Template class bitset

#ifndef _BITSET_
#define _BITSET_

#include <cstddef>
#include <string>
#include <stdexcept>
#include <iosfwd>

namespace std {

template <size_t N> class bitset {
public:
    class reference {
        friend class bitset;
        reference();
    public:
        ~reference();
        reference& operator=(bool x);
        reference& operator=(const reference&);
        bool operator~() const;
        operator bool() const;
        reference& flip();
    };

    bitset();
    bitset(unsigned long val);
    template<class charT, class traits, class Allocator>
    explicit bitset(const basic_string<charT,traits,Allocator>& str,
                    typename basic_string<charT,traits,Allocator>::size_type pos = 0,
                    typename basic_string<charT,traits,Allocator>::size_type n =
                        basic_string<charT,traits,Allocator>::npos);

    bitset<N>& operator&=(const bitset<N>& rhs);
    bitset<N>& operator|=(const bitset<N>& rhs);
    bitset<N>& operator^=(const bitset<N>& rhs);
    bitset<N>& operator<<=(size_t pos);
    bitset<N>& operator>>=(size_t pos);
    bitset<N>& set();
    bitset<N>& set(size_t pos, int val = 1);
    bitset<N>& reset();
    bitset<N>& reset(size_t pos);
    bitset<N> operator~() const;
    bitset<N>& flip();
    bitset<N>& flip(size_t pos);

    reference operator[](size_t pos);
    unsigned long to_ulong() const;
    template <class charT, class traits, class Allocator>
    basic_string<charT, traits, Allocator> to_string() const;
    size_t count() const;
    size_t size() const;
    bool operator==(const bitset<N>& rhs) const;
    bool operator!=(const bitset<N>& rhs) const;
    bool test(size_t pos) const;
    bool any() const;
    bool none() const;
    bitset<N> operator<<(size_t pos) const;
    bitset<N> operator>>(size_t pos) const;
};

template <size_t N>
bitset<N> operator&(const bitset<N>&, const bitset<N>&);

template <size_t N>
bitset<N> operator|(const bitset<N>&, const bitset<N>&);

template <size_t N>
bitset<N> operator^(const bitset<N>&, const bitset<N>&);

template <class charT, class traits, size_t N>
basic_istream<charT, traits>&
operator>>(basic_istream<charT, traits>& is, bitset<N>& x);

template <class charT, class traits, size_t N>
basic_ostream<charT, traits>&
operator<<(basic_ostream<charT, traits>& os, const bitset<N>& x);

} // namespace std

#endif