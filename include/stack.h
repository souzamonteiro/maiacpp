// ISO/IEC 14882:1998(E) - 23.2.3.3 stack

#ifndef _STACK_
#define _STACK_

#include <deque>

namespace std {

template <class T, class Container = deque<T> >
class stack {
public:
    typedef typename Container::value_type value_type;
    typedef typename Container::size_type size_type;
    typedef Container container_type;

protected:
    Container c;

public:
    explicit stack(const Container& = Container());
    const container_type& __container() const { return c; }
    bool empty() const { return c.empty(); }
    size_type size() const { return c.size(); }
    value_type& top() { return c.back(); }
    const value_type& top() const { return c.back(); }
    void push(const value_type& x) { c.push_back(x); }
    void pop() { c.pop_back(); }
};

template <class T, class Container>
bool operator==(const stack<T, Container>& x, const stack<T, Container>& y);

template <class T, class Container>
bool operator<(const stack<T, Container>& x, const stack<T, Container>& y);

template <class T, class Container>
bool operator!=(const stack<T, Container>& x, const stack<T, Container>& y);

template <class T, class Container>
bool operator>(const stack<T, Container>& x, const stack<T, Container>& y);

template <class T, class Container>
bool operator>=(const stack<T, Container>& x, const stack<T, Container>& y);

template <class T, class Container>
bool operator<=(const stack<T, Container>& x, const stack<T, Container>& y);

template <class T, class Container>
inline stack<T,Container>::stack(const Container& x) : c(x) {}

template <class T, class Container>
inline bool operator==(const stack<T,Container>& x, const stack<T,Container>& y) {
    return x.__container() == y.__container();
}

template <class T, class Container>
inline bool operator<(const stack<T,Container>& x, const stack<T,Container>& y) {
    return x.__container() < y.__container();
}

template <class T, class Container>
inline bool operator!=(const stack<T,Container>& x, const stack<T,Container>& y) { return !(x == y); }

template <class T, class Container>
inline bool operator>(const stack<T,Container>& x, const stack<T,Container>& y) { return y < x; }

template <class T, class Container>
inline bool operator>=(const stack<T,Container>& x, const stack<T,Container>& y) { return !(x < y); }

template <class T, class Container>
inline bool operator<=(const stack<T,Container>& x, const stack<T,Container>& y) { return !(y < x); }

} // namespace std

#endif