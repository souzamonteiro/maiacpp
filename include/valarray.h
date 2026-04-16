// ISO/IEC 14882:1998(E) - 26.3 Numeric arrays

#ifndef _VALARRAY_
#define _VALARRAY_

#include <cstddef>

namespace std {

template<class T> class valarray;

class slice {
public:
    slice();
    slice(size_t, size_t, size_t);
    size_t start() const;
    size_t size() const;
    size_t stride() const;
};

template<class T> class slice_array {
public:
    typedef T value_type;
    void operator=(const valarray<T>&) const;
    void operator*=(const valarray<T>&) const;
    void operator/=(const valarray<T>&) const;
    void operator%=(const valarray<T>&) const;
    void operator+=(const valarray<T>&) const;
    void operator-=(const valarray<T>&) const;
    void operator^=(const valarray<T>&) const;
    void operator&=(const valarray<T>&) const;
    void operator|=(const valarray<T>&) const;
    void operator<<=(const valarray<T>&) const;
    void operator>>=(const valarray<T>&) const;
    void operator=(const T&);
    ~slice_array();
};

class gslice {
public:
    gslice();
    gslice(size_t s, const valarray<size_t>& l, const valarray<size_t>& d);
    size_t start() const;
    valarray<size_t> size() const;
    valarray<size_t> stride() const;
};

template<class T> class gslice_array {
public:
    typedef T value_type;
    void operator=(const valarray<T>&) const;
    void operator*=(const valarray<T>&) const;
    void operator/=(const valarray<T>&) const;
    void operator%=(const valarray<T>&) const;
    void operator+=(const valarray<T>&) const;
    void operator-=(const valarray<T>&) const;
    void operator^=(const valarray<T>&) const;
    void operator&=(const valarray<T>&) const;
    void operator|=(const valarray<T>&) const;
    void operator<<=(const valarray<T>&) const;
    void operator>>=(const valarray<T>&) const;
    void operator=(const T&);
    ~gslice_array();
};

template<class T> class mask_array {
public:
    typedef T value_type;
    void operator=(const valarray<T>&) const;
    void operator*=(const valarray<T>&) const;
    void operator/=(const valarray<T>&) const;
    void operator%=(const valarray<T>&) const;
    void operator+=(const valarray<T>&) const;
    void operator-=(const valarray<T>&) const;
    void operator^=(const valarray<T>&) const;
    void operator&=(const valarray<T>&) const;
    void operator|=(const valarray<T>&) const;
    void operator<<=(const valarray<T>&) const;
    void operator>>=(const valarray<T>&) const;
    void operator=(const T&);
    ~mask_array();
};

template<class T> class indirect_array {
public:
    typedef T value_type;
    void operator=(const valarray<T>&) const;
    void operator*=(const valarray<T>&) const;
    void operator/=(const valarray<T>&) const;
    void operator%=(const valarray<T>&) const;
    void operator+=(const valarray<T>&) const;
    void operator-=(const valarray<T>&) const;
    void operator^=(const valarray<T>&) const;
    void operator&=(const valarray<T>&) const;
    void operator|=(const valarray<T>&) const;
    void operator<<=(const valarray<T>&) const;
    void operator>>=(const valarray<T>&) const;
    void operator=(const T&);
    ~indirect_array();
};

template<class T> class valarray {
public:
    typedef T value_type;

    valarray();
    explicit valarray(size_t);
    valarray(const T&, size_t);
    valarray(const T*, size_t);
    valarray(const valarray&);
    valarray(const slice_array<T>&);
    valarray(const gslice_array<T>&);
    valarray(const mask_array<T>&);
    valarray(const indirect_array<T>&);
    ~valarray();

    valarray<T>& operator=(const valarray<T>&);
    valarray<T>& operator=(const T&);
    valarray<T>& operator=(const slice_array<T>&);
    valarray<T>& operator=(const gslice_array<T>&);
    valarray<T>& operator=(const mask_array<T>&);
    valarray<T>& operator=(const indirect_array<T>&);

    T operator[](size_t) const;
    T& operator[](size_t);

    valarray<T> operator[](slice) const;
    slice_array<T> operator[](slice);
    valarray<T> operator[](const gslice&) const;
    gslice_array<T> operator[](const gslice&);
    valarray<T> operator[](const valarray<bool>&) const;
    mask_array<T> operator[](const valarray<bool>&);
    valarray<T> operator[](const valarray<size_t>&) const;
    indirect_array<T> operator[](const valarray<size_t>&);

    valarray<T> operator+() const;
    valarray<T> operator-() const;
    valarray<T> operator~() const;
    valarray<bool> operator!() const;

    valarray<T>& operator*=(const T&);
    valarray<T>& operator/=(const T&);
    valarray<T>& operator%=(const T&);
    valarray<T>& operator+=(const T&);
    valarray<T>& operator-=(const T&);
    valarray<T>& operator^=(const T&);
    valarray<T>& operator&=(const T&);
    valarray<T>& operator|=(const T&);
    valarray<T>& operator<<=(const T&);
    valarray<T>& operator>>=(const T&);

    valarray<T>& operator*=(const valarray<T>&);
    valarray<T>& operator/=(const valarray<T>&);
    valarray<T>& operator%=(const valarray<T>&);
    valarray<T>& operator+=(const valarray<T>&);
    valarray<T>& operator-=(const valarray<T>&);
    valarray<T>& operator^=(const valarray<T>&);
    valarray<T>& operator&=(const valarray<T>&);
    valarray<T>& operator|=(const valarray<T>&);
    valarray<T>& operator<<=(const valarray<T>&);
    valarray<T>& operator>>=(const valarray<T>&);

    size_t size() const;
    T sum() const;
    T min() const;
    T max() const;
    valarray<T> shift(int) const;
    valarray<T> cshift(int) const;
    valarray<T> apply(T func(T)) const;
    valarray<T> apply(T func(const T&)) const;
    void resize(size_t sz, T c = T());
};

template<class T> valarray<T> operator*(const valarray<T>&, const valarray<T>&);
template<class T> valarray<T> operator*(const valarray<T>&, const T&);
template<class T> valarray<T> operator*(const T&, const valarray<T>&);
template<class T> valarray<T> operator/(const valarray<T>&, const valarray<T>&);
template<class T> valarray<T> operator/(const valarray<T>&, const T&);
template<class T> valarray<T> operator/(const T&, const valarray<T>&);
template<class T> valarray<T> operator%(const valarray<T>&, const valarray<T>&);
template<class T> valarray<T> operator%(const valarray<T>&, const T&);
template<class T> valarray<T> operator%(const T&, const valarray<T>&);
template<class T> valarray<T> operator+(const valarray<T>&, const valarray<T>&);
template<class T> valarray<T> operator+(const valarray<T>&, const T&);
template<class T> valarray<T> operator+(const T&, const valarray<T>&);
template<class T> valarray<T> operator-(const valarray<T>&, const valarray<T>&);
template<class T> valarray<T> operator-(const valarray<T>&, const T&);
template<class T> valarray<T> operator-(const T&, const valarray<T>&);
template<class T> valarray<T> operator^(const valarray<T>&, const valarray<T>&);
template<class T> valarray<T> operator^(const valarray<T>&, const T&);
template<class T> valarray<T> operator^(const T&, const valarray<T>&);
template<class T> valarray<T> operator&(const valarray<T>&, const valarray<T>&);
template<class T> valarray<T> operator&(const valarray<T>&, const T&);
template<class T> valarray<T> operator&(const T&, const valarray<T>&);
template<class T> valarray<T> operator|(const valarray<T>&, const valarray<T>&);
template<class T> valarray<T> operator|(const valarray<T>&, const T&);
template<class T> valarray<T> operator|(const T&, const valarray<T>&);
template<class T> valarray<T> operator<<(const valarray<T>&, const valarray<T>&);
template<class T> valarray<T> operator<<(const valarray<T>&, const T&);
template<class T> valarray<T> operator<<(const T&, const valarray<T>&);
template<class T> valarray<T> operator>>(const valarray<T>&, const valarray<T>&);
template<class T> valarray<T> operator>>(const valarray<T>&, const T&);
template<class T> valarray<T> operator>>(const T&, const valarray<T>&);

template<class T> valarray<bool> operator==(const valarray<T>&, const valarray<T>&);
template<class T> valarray<bool> operator==(const valarray<T>&, const T&);
template<class T> valarray<bool> operator==(const T&, const valarray<T>&);
template<class T> valarray<bool> operator!=(const valarray<T>&, const valarray<T>&);
template<class T> valarray<bool> operator!=(const valarray<T>&, const T&);
template<class T> valarray<bool> operator!=(const T&, const valarray<T>&);
template<class T> valarray<bool> operator<(const valarray<T>&, const valarray<T>&);
template<class T> valarray<bool> operator<(const valarray<T>&, const T&);
template<class T> valarray<bool> operator<(const T&, const valarray<T>&);
template<class T> valarray<bool> operator>(const valarray<T>&, const valarray<T>&);
template<class T> valarray<bool> operator>(const valarray<T>&, const T&);
template<class T> valarray<bool> operator>(const T&, const valarray<T>&);
template<class T> valarray<bool> operator<=(const valarray<T>&, const valarray<T>&);
template<class T> valarray<bool> operator<=(const valarray<T>&, const T&);
template<class T> valarray<bool> operator<=(const T&, const valarray<T>&);
template<class T> valarray<bool> operator>=(const valarray<T>&, const valarray<T>&);
template<class T> valarray<bool> operator>=(const valarray<T>&, const T&);
template<class T> valarray<bool> operator>=(const T&, const valarray<T>&);
template<class T> valarray<bool> operator&&(const valarray<T>&, const valarray<T>&);
template<class T> valarray<bool> operator&&(const valarray<T>&, const T&);
template<class T> valarray<bool> operator&&(const T&, const valarray<T>&);
template<class T> valarray<bool> operator||(const valarray<T>&, const valarray<T>&);
template<class T> valarray<bool> operator||(const valarray<T>&, const T&);
template<class T> valarray<bool> operator||(const T&, const valarray<T>&);

template<class T> valarray<T> abs(const valarray<T>&);
template<class T> valarray<T> acos(const valarray<T>&);
template<class T> valarray<T> asin(const valarray<T>&);
template<class T> valarray<T> atan(const valarray<T>&);
template<class T> valarray<T> atan2(const valarray<T>&, const valarray<T>&);
template<class T> valarray<T> atan2(const valarray<T>&, const T&);
template<class T> valarray<T> atan2(const T&, const valarray<T>&);
template<class T> valarray<T> cos(const valarray<T>&);
template<class T> valarray<T> cosh(const valarray<T>&);
template<class T> valarray<T> exp(const valarray<T>&);
template<class T> valarray<T> log(const valarray<T>&);
template<class T> valarray<T> log10(const valarray<T>&);
template<class T> valarray<T> pow(const valarray<T>&, const valarray<T>&);
template<class T> valarray<T> pow(const valarray<T>&, const T&);
template<class T> valarray<T> pow(const T&, const valarray<T>&);
template<class T> valarray<T> sin(const valarray<T>&);
template<class T> valarray<T> sinh(const valarray<T>&);
template<class T> valarray<T> sqrt(const valarray<T>&);
template<class T> valarray<T> tan(const valarray<T>&);
template<class T> valarray<T> tanh(const valarray<T>&);

} // namespace std

#endif