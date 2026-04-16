// ISO/IEC 14882:1998(E) - 23.2.1 Template class deque

#ifndef _DEQUE_
#define _DEQUE_

#include <memory>
#include <cstddef>
#include <iterator>

namespace std {

template <class T, class Allocator = allocator<T> >
class deque {
public:
    typedef typename Allocator::reference reference;
    typedef typename Allocator::const_reference const_reference;
    typedef T value_type;
    typedef Allocator allocator_type;
    typedef typename Allocator::pointer pointer;
    typedef typename Allocator::const_pointer const_pointer;
    typedef pointer iterator;               // provisional contiguous-iterator profile
    typedef const_pointer const_iterator;   // provisional contiguous-iterator profile
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    explicit deque(const Allocator& = Allocator());
    explicit deque(size_type n, const T& value = T(), const Allocator& = Allocator());
    template <class InputIterator>
    deque(InputIterator first, InputIterator last, const Allocator& = Allocator());
    deque(const deque<T,Allocator>& x);
    ~deque();
    deque<T,Allocator>& operator=(const deque<T,Allocator>& x);
    template <class InputIterator>
    void assign(InputIterator first, InputIterator last);
    void assign(size_type n, const T& t);
    allocator_type get_allocator() const;

    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;
    reverse_iterator rbegin();
    const_reverse_iterator rbegin() const;
    reverse_iterator rend();
    const_reverse_iterator rend() const;

    size_type size() const;
    size_type max_size() const;
    void resize(size_type sz, T c = T());
    bool empty() const;

    reference operator[](size_type n);
    const_reference operator[](size_type n) const;
    reference at(size_type n);
    const_reference at(size_type n) const;
    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;

    void push_front(const T& x);
    void push_back(const T& x);
    iterator insert(iterator position, const T& x);
    void insert(iterator position, size_type n, const T& x);
    template <class InputIterator>
    void insert(iterator position, InputIterator first, InputIterator last);
    void pop_front();
    void pop_back();
    iterator erase(iterator position);
    iterator erase(iterator first, iterator last);
    void swap(deque<T,Allocator>&);
    void clear();
};

template <class T, class Allocator>
bool operator==(const deque<T,Allocator>& x, const deque<T,Allocator>& y);

template <class T, class Allocator>
bool operator<(const deque<T,Allocator>& x, const deque<T,Allocator>& y);

template <class T, class Allocator>
bool operator!=(const deque<T,Allocator>& x, const deque<T,Allocator>& y);

template <class T, class Allocator>
bool operator>(const deque<T,Allocator>& x, const deque<T,Allocator>& y);

template <class T, class Allocator>
bool operator>=(const deque<T,Allocator>& x, const deque<T,Allocator>& y);

template <class T, class Allocator>
bool operator<=(const deque<T,Allocator>& x, const deque<T,Allocator>& y);

template <class T, class Allocator>
void swap(deque<T,Allocator>& x, deque<T,Allocator>& y);

} // namespace std

#endif