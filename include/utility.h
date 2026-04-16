// ISO/IEC 14882:1998(E) - 20.2 Utility components

#ifndef _UTILITY_
#define _UTILITY_

namespace std {

namespace rel_ops {
    template<class T> bool operator!=(const T&, const T&);
    template<class T> bool operator>(const T&, const T&);
    template<class T> bool operator<=(const T&, const T&);
    template<class T> bool operator>=(const T&, const T&);
}

template <class T1, class T2>
struct pair {
    typedef T1 first_type;
    typedef T2 second_type;
    T1 first;
    T2 second;
    pair();
    pair(const T1& x, const T2& y);
    template<class U, class V> pair(const pair<U, V>& p);
};

template <class T1, class T2>
bool operator==(const pair<T1,T2>&, const pair<T1,T2>&);

template <class T1, class T2>
bool operator<(const pair<T1,T2>&, const pair<T1,T2>&);

template <class T1, class T2>
bool operator!=(const pair<T1,T2>&, const pair<T1,T2>&);

template <class T1, class T2>
bool operator>(const pair<T1,T2>&, const pair<T1,T2>&);

template <class T1, class T2>
bool operator>=(const pair<T1,T2>&, const pair<T1,T2>&);

template <class T1, class T2>
bool operator<=(const pair<T1,T2>&, const pair<T1,T2>&);

template <class T1, class T2> pair<T1,T2> make_pair(const T1&, const T2&);

} // namespace std

#endif