// ISO/IEC 14882:1998(E) - 23.2.2 Template class list

#ifndef _LIST_
#define _LIST_

#include <memory>
#include <cstddef>
#include <iterator>

namespace std {

template <class T, class Allocator = allocator<T> >
class list {
public:
    typedef typename Allocator::reference reference;
    typedef typename Allocator::const_reference const_reference;
    typedef T value_type;
    typedef Allocator allocator_type;
    typedef typename Allocator::pointer pointer;
    typedef typename Allocator::const_pointer const_pointer;
    typedef pointer iterator;               // provisional iterator profile
    typedef const_pointer const_iterator;   // provisional iterator profile
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    explicit list(const Allocator& = Allocator());
    explicit list(size_type n, const T& value = T(), const Allocator& = Allocator());
    template <class InputIterator>
    list(InputIterator first, InputIterator last, const Allocator& = Allocator());
    list(const list<T,Allocator>& x);
    ~list();
    list<T,Allocator>& operator=(const list<T,Allocator>& x);
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

    bool empty() const;
    size_type size() const;
    size_type max_size() const;
    void resize(size_type sz, T c = T());

    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;

    void push_front(const T& x);
    void pop_front();
    void push_back(const T& x);
    void pop_back();
    iterator insert(iterator position, const T& x);
    void insert(iterator position, size_type n, const T& x);
    template <class InputIterator>
    void insert(iterator position, InputIterator first, InputIterator last);
    iterator erase(iterator position);
    iterator erase(iterator first, iterator last);
    void swap(list<T,Allocator>&);
    void clear();

    void splice(iterator position, list<T,Allocator>& x);
    void splice(iterator position, list<T,Allocator>& x, iterator i);
    void splice(iterator position, list<T,Allocator>& x, iterator first, iterator last);
    void remove(const T& value);
    template <class Predicate> void remove_if(Predicate pred);
    void unique();
    template <class BinaryPredicate> void unique(BinaryPredicate binary_pred);
    void merge(list<T,Allocator>& x);
    template <class Compare> void merge(list<T,Allocator>& x, Compare comp);
    void sort();
    template <class Compare> void sort(Compare comp);
    void reverse();
};

template <class T, class Allocator>
bool operator==(const list<T,Allocator>& x, const list<T,Allocator>& y);

template <class T, class Allocator>
bool operator<(const list<T,Allocator>& x, const list<T,Allocator>& y);

template <class T, class Allocator>
bool operator!=(const list<T,Allocator>& x, const list<T,Allocator>& y);

template <class T, class Allocator>
bool operator>(const list<T,Allocator>& x, const list<T,Allocator>& y);

template <class T, class Allocator>
bool operator>=(const list<T,Allocator>& x, const list<T,Allocator>& y);

template <class T, class Allocator>
bool operator<=(const list<T,Allocator>& x, const list<T,Allocator>& y);

template <class T, class Allocator>
void swap(list<T,Allocator>& x, list<T,Allocator>& y);

} // namespace std

#endif