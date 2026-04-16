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

} // namespace std

#endif