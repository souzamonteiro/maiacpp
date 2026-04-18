// ISO/IEC 14882:1998(E) - 23.2.3.1 queue, 23.2.3.2 priority_queue

#ifndef _QUEUE_
#define _QUEUE_

#include <deque>
#include <vector>
#include <functional>

namespace std {

template <class T, class Container = deque<T> >
class queue {
public:
    typedef typename Container::value_type value_type;
    typedef typename Container::size_type size_type;
    typedef Container container_type;

protected:
    Container c;

public:
    explicit queue(const Container& = Container());
    const container_type& __container() const { return c; }
    bool empty() const { return c.empty(); }
    size_type size() const { return c.size(); }
    value_type& front() { return c.front(); }
    const value_type& front() const { return c.front(); }
    value_type& back() { return c.back(); }
    const value_type& back() const { return c.back(); }
    void push(const value_type& x) { c.push_back(x); }
    void pop() { c.pop_front(); }
};

template <class T, class Container>
bool operator==(const queue<T, Container>& x, const queue<T, Container>& y);

template <class T, class Container>
bool operator<(const queue<T, Container>& x, const queue<T, Container>& y);

template <class T, class Container>
bool operator!=(const queue<T, Container>& x, const queue<T, Container>& y);

template <class T, class Container>
bool operator>(const queue<T, Container>& x, const queue<T, Container>& y);

template <class T, class Container>
bool operator>=(const queue<T, Container>& x, const queue<T, Container>& y);

template <class T, class Container>
bool operator<=(const queue<T, Container>& x, const queue<T, Container>& y);

template <class T, class Container = vector<T>,
          class Compare = less<typename Container::value_type> >
class priority_queue {
public:
    typedef typename Container::value_type value_type;
    typedef typename Container::size_type size_type;
    typedef Container container_type;

protected:
    Container c;
    Compare comp;

public:
    explicit priority_queue(const Compare& x = Compare(),
                            const Container& = Container());
    template <class InputIterator>
    priority_queue(InputIterator first, InputIterator last,
                   const Compare& x = Compare(),
                   const Container& = Container());

    bool empty() const { return c.empty(); }
    size_type size() const { return c.size(); }
    const value_type& top() const { return c.front(); }
    void push(const value_type& x);
    void pop();
};

template <class T, class Container>
inline queue<T,Container>::queue(const Container& x) : c(x) {}

template <class T, class Container>
inline bool operator==(const queue<T,Container>& x, const queue<T,Container>& y) {
    return x.__container() == y.__container();
}

template <class T, class Container>
inline bool operator<(const queue<T,Container>& x, const queue<T,Container>& y) {
    return x.__container() < y.__container();
}

template <class T, class Container>
inline bool operator!=(const queue<T,Container>& x, const queue<T,Container>& y) { return !(x == y); }

template <class T, class Container>
inline bool operator>(const queue<T,Container>& x, const queue<T,Container>& y) { return y < x; }

template <class T, class Container>
inline bool operator>=(const queue<T,Container>& x, const queue<T,Container>& y) { return !(x < y); }

template <class T, class Container>
inline bool operator<=(const queue<T,Container>& x, const queue<T,Container>& y) { return !(y < x); }

template <class T, class Container, class Compare>
inline priority_queue<T,Container,Compare>::priority_queue(const Compare& x, const Container& cont)
    : c(cont), comp(x) {
    if (c.size() < 2) return;
    for (size_type i = 1; i < c.size(); ++i) {
        size_type child = i;
        while (child > 0) {
            size_type parent = (child - 1) / 2;
            if (!comp(c[parent], c[child])) break;
            value_type tmp = c[parent];
            c[parent] = c[child];
            c[child] = tmp;
            child = parent;
        }
    }
}

template <class T, class Container, class Compare>
template <class InputIterator>
inline priority_queue<T,Container,Compare>::priority_queue(InputIterator first, InputIterator last,
                                                           const Compare& x, const Container& cont)
    : c(cont), comp(x) {
    for (; first != last; ++first) push(*first);
}

template <class T, class Container, class Compare>
inline void priority_queue<T,Container,Compare>::push(const value_type& x) {
    c.push_back(x);
    size_type child = c.size() - 1;
    while (child > 0) {
        size_type parent = (child - 1) / 2;
        if (!comp(c[parent], c[child])) break;
        value_type tmp = c[parent];
        c[parent] = c[child];
        c[child] = tmp;
        child = parent;
    }
}

template <class T, class Container, class Compare>
inline void priority_queue<T,Container,Compare>::pop() {
    if (c.empty()) return;
    c.front() = c.back();
    c.pop_back();
    size_type parent = 0;
    while (true) {
        size_type left = parent * 2 + 1;
        size_type right = left + 1;
        size_type largest = parent;
        if (left < c.size() && comp(c[largest], c[left])) largest = left;
        if (right < c.size() && comp(c[largest], c[right])) largest = right;
        if (largest == parent) break;
        value_type tmp = c[parent];
        c[parent] = c[largest];
        c[largest] = tmp;
        parent = largest;
    }
}

} // namespace std

#endif