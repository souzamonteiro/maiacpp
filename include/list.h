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
    typedef size_t   size_type;
    typedef ptrdiff_t difference_type;

    // Internal node type
    struct _Node {
        T      value;
        _Node* prev;
        _Node* next;
        explicit _Node(const T& v, _Node* p = 0, _Node* n = 0) : value(v), prev(p), next(n) {}
    };

    // Bidirectional iterator
    struct iterator {
        typedef T                                value_type;
        typedef T*                               pointer;
        typedef T&                               reference;
        typedef ptrdiff_t                        difference_type;
        typedef std::bidirectional_iterator_tag  iterator_category;
        _Node* _node;
        explicit iterator(_Node* n = 0) : _node(n) {}
        reference  operator*()  const { return _node->value; }
        pointer    operator->() const { return &_node->value; }
        iterator&  operator++()    { _node = _node->next; return *this; }
        iterator   operator++(int) { iterator t(*this); ++*this; return t; }
        iterator&  operator--()    { _node = _node->prev; return *this; }
        iterator   operator--(int) { iterator t(*this); --*this; return t; }
        bool operator==(const iterator& o) const { return _node == o._node; }
        bool operator!=(const iterator& o) const { return _node != o._node; }
    };
    struct const_iterator {
        typedef T                                value_type;
        typedef const T*                         pointer;
        typedef const T&                         reference;
        typedef ptrdiff_t                        difference_type;
        typedef std::bidirectional_iterator_tag  iterator_category;
        const _Node* _node;
        explicit const_iterator(const _Node* n = 0) : _node(n) {}
        const_iterator(const iterator& it) : _node(it._node) {}
        reference       operator*()  const { return _node->value; }
        pointer         operator->() const { return &_node->value; }
        const_iterator& operator++()    { _node = _node->next; return *this; }
        const_iterator  operator++(int) { const_iterator t(*this); ++*this; return t; }
        const_iterator& operator--()    { _node = _node->prev; return *this; }
        const_iterator  operator--(int) { const_iterator t(*this); --*this; return t; }
        bool operator==(const const_iterator& o) const { return _node == o._node; }
        bool operator!=(const const_iterator& o) const { return _node != o._node; }
    };
    typedef std::reverse_iterator<iterator>       reverse_iterator;
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
private:
    // Circular doubly-linked list with a heap-allocated sentinel node.
    // sentinel->next == first element (or sentinel when empty).
    // sentinel->prev == last element  (or sentinel when empty).
    _Node*    _sentinel;
    size_type _size;
    // Node allocation bypasses the element Allocator for simplicity
    // (list<T,A>::_Node cannot be constructed via Allocator<T>).
    _Node* _new_node(const T& v) { return new _Node(v); }
    void   _del_node(_Node* n)   { delete n; }
    void   _init_sentinel() {
        _sentinel = new _Node(T());
        _sentinel->prev = _sentinel;
        _sentinel->next = _sentinel;
    }
    // Splice [first,last) into position (internal, O(1) pointer fixup)
    void _splice_range(_Node* pos, _Node* first, _Node* last_prev) {
        // detach [first .. last_prev] from their current list (caller adjusts sizes)
        _Node* before_first = first->prev;
        _Node* after_last   = last_prev->next;
        before_first->next  = after_last;
        after_last->prev    = before_first;
        // reattach before pos
        _Node* before_pos   = pos->prev;
        before_pos->next    = first;
        first->prev         = before_pos;
        last_prev->next     = pos;
        pos->prev           = last_prev;
    }
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

// ---- list<T,Allocator> inline implementations ----
template<class T, class A>
inline list<T,A>::list(const A&) : _size(0) { _init_sentinel(); }

template<class T, class A>
inline list<T,A>::list(size_type n, const T& v, const A&) : _size(0) {
    _init_sentinel();
    for (size_type i = 0; i < n; ++i) push_back(v);
}

template<class T, class A>
template<class InputIterator>
inline list<T,A>::list(InputIterator first, InputIterator last, const A&) : _size(0) {
    _init_sentinel();
    for (; first != last; ++first) push_back(*first);
}

template<class T, class A>
inline list<T,A>::list(const list<T,A>& x) : _size(0) {
    _init_sentinel();
    for (const_iterator it = x.begin(); it != x.end(); ++it) push_back(*it);
}

template<class T, class A>
inline list<T,A>::~list() {
    clear();
    delete _sentinel;
}

template<class T, class A>
inline list<T,A>& list<T,A>::operator=(const list<T,A>& x) {
    if (this != &x) { list tmp(x); swap(tmp); }
    return *this;
}

template<class T, class A>
template<class InputIterator>
inline void list<T,A>::assign(InputIterator first, InputIterator last) {
    clear(); for (; first != last; ++first) push_back(*first);
}
template<class T, class A>
inline void list<T,A>::assign(size_type n, const T& t) {
    clear(); for (size_type i = 0; i < n; ++i) push_back(t);
}
template<class T, class A>
inline typename list<T,A>::allocator_type list<T,A>::get_allocator() const { return A(); }

template<class T, class A>
inline typename list<T,A>::iterator list<T,A>::begin() { return iterator(_sentinel->next); }
template<class T, class A>
inline typename list<T,A>::const_iterator list<T,A>::begin() const { return const_iterator(_sentinel->next); }
template<class T, class A>
inline typename list<T,A>::iterator list<T,A>::end() { return iterator(_sentinel); }
template<class T, class A>
inline typename list<T,A>::const_iterator list<T,A>::end() const { return const_iterator(_sentinel); }
template<class T, class A>
inline typename list<T,A>::reverse_iterator list<T,A>::rbegin() { return reverse_iterator(end()); }
template<class T, class A>
inline typename list<T,A>::const_reverse_iterator list<T,A>::rbegin() const { return const_reverse_iterator(end()); }
template<class T, class A>
inline typename list<T,A>::reverse_iterator list<T,A>::rend() { return reverse_iterator(begin()); }
template<class T, class A>
inline typename list<T,A>::const_reverse_iterator list<T,A>::rend() const { return const_reverse_iterator(begin()); }

template<class T, class A>
inline bool list<T,A>::empty() const { return _size == 0; }
template<class T, class A>
inline typename list<T,A>::size_type list<T,A>::size() const { return _size; }
template<class T, class A>
inline typename list<T,A>::size_type list<T,A>::max_size() const { return size_type(-1) / sizeof(_Node); }
template<class T, class A>
inline void list<T,A>::resize(size_type sz, T c) {
    while (_size > sz) pop_back();
    while (_size < sz) push_back(c);
}

template<class T, class A>
inline typename list<T,A>::reference list<T,A>::front() { return _sentinel->next->value; }
template<class T, class A>
inline typename list<T,A>::const_reference list<T,A>::front() const { return _sentinel->next->value; }
template<class T, class A>
inline typename list<T,A>::reference list<T,A>::back() { return _sentinel->prev->value; }
template<class T, class A>
inline typename list<T,A>::const_reference list<T,A>::back() const { return _sentinel->prev->value; }

template<class T, class A>
inline void list<T,A>::push_front(const T& x) { insert(begin(), x); }
template<class T, class A>
inline void list<T,A>::pop_front() { erase(begin()); }
template<class T, class A>
inline void list<T,A>::push_back(const T& x) { insert(end(), x); }
template<class T, class A>
inline void list<T,A>::pop_back() { erase(iterator(_sentinel->prev)); }

template<class T, class A>
inline typename list<T,A>::iterator list<T,A>::insert(iterator pos, const T& x) {
    _Node* n = _new_node(x);
    _Node* p = pos._node;
    n->next = p;
    n->prev = p->prev;
    p->prev->next = n;
    p->prev = n;
    ++_size;
    return iterator(n);
}
template<class T, class A>
inline void list<T,A>::insert(iterator pos, size_type n, const T& x) {
    for (size_type i = 0; i < n; ++i) insert(pos, x);
}
template<class T, class A>
template<class InputIterator>
inline void list<T,A>::insert(iterator pos, InputIterator first, InputIterator last) {
    for (; first != last; ++first) insert(pos, *first);
}
template<class T, class A>
inline typename list<T,A>::iterator list<T,A>::erase(iterator pos) {
    _Node* n = pos._node;
    iterator ret(n->next);
    n->prev->next = n->next;
    n->next->prev = n->prev;
    _del_node(n);
    --_size;
    return ret;
}
template<class T, class A>
inline typename list<T,A>::iterator list<T,A>::erase(iterator first, iterator last) {
    while (first != last) first = erase(first);
    return last;
}
template<class T, class A>
inline void list<T,A>::swap(list<T,A>& x) {
    _Node* ts = _sentinel; _sentinel = x._sentinel; x._sentinel = ts;
    size_type tz = _size;  _size     = x._size;     x._size     = tz;
}
template<class T, class A>
inline void list<T,A>::clear() {
    _Node* n = _sentinel->next;
    while (n != _sentinel) { _Node* nx = n->next; _del_node(n); n = nx; }
    _sentinel->next = _sentinel;
    _sentinel->prev = _sentinel;
    _size = 0;
}

template<class T, class A>
inline void list<T,A>::splice(iterator pos, list<T,A>& x) {
    if (x.empty()) return;
    _splice_range(pos._node, x._sentinel->next, x._sentinel->prev);
    _size += x._size;
    x._size = 0;
    x._sentinel->next = x._sentinel;
    x._sentinel->prev = x._sentinel;
}
template<class T, class A>
inline void list<T,A>::splice(iterator pos, list<T,A>& x, iterator i) {
    _splice_range(pos._node, i._node, i._node);
    ++_size; --x._size;
}
template<class T, class A>
inline void list<T,A>::splice(iterator pos, list<T,A>& x, iterator first, iterator last) {
    if (first == last) return;
    size_type n = 0;
    for (iterator it = first; it != last; ++it) ++n;
    _splice_range(pos._node, first._node, last._node->prev);
    _size += n; x._size -= n;
}

template<class T, class A>
inline void list<T,A>::remove(const T& v) {
    for (iterator it = begin(); it != end(); ) {
        if (*it == v) it = erase(it);
        else ++it;
    }
}
template<class T, class A>
template<class Predicate>
inline void list<T,A>::remove_if(Predicate pred) {
    for (iterator it = begin(); it != end(); ) {
        if (pred(*it)) it = erase(it);
        else ++it;
    }
}
template<class T, class A>
inline void list<T,A>::unique() {
    if (empty()) return;
    iterator it = begin(), nx = it; ++nx;
    while (nx != end()) {
        if (*it == *nx) nx = erase(nx);
        else { it = nx; ++nx; }
    }
}
template<class T, class A>
template<class BinaryPredicate>
inline void list<T,A>::unique(BinaryPredicate bp) {
    if (empty()) return;
    iterator it = begin(), nx = it; ++nx;
    while (nx != end()) {
        if (bp(*it, *nx)) nx = erase(nx);
        else { it = nx; ++nx; }
    }
}

template<class T, class A>
inline void list<T,A>::merge(list<T,A>& x) {
    iterator i = begin(), j = x.begin();
    while (i != end() && j != x.end()) {
        if (*j < *i) { iterator nj = j; ++nj; splice(i, x, j); j = nj; }
        else ++i;
    }
    if (!x.empty()) splice(end(), x);
}
template<class T, class A>
template<class Compare>
inline void list<T,A>::merge(list<T,A>& x, Compare comp) {
    iterator i = begin(), j = x.begin();
    while (i != end() && j != x.end()) {
        if (comp(*j, *i)) { iterator nj = j; ++nj; splice(i, x, j); j = nj; }
        else ++i;
    }
    if (!x.empty()) splice(end(), x);
}

template<class T, class A>
inline void list<T,A>::sort() {
    if (_size < 2) return;
    // Merge sort via splitting into two halves
    list<T,A> left, right;
    size_type half = _size / 2;
    iterator mid = begin();
    for (size_type i = 0; i < half; ++i) ++mid;
    right.splice(right.end(), *this, mid, end());
    left.splice(left.end(), *this);
    left.sort(); right.sort();
    left.merge(right);
    swap(left);
}
template<class T, class A>
template<class Compare>
inline void list<T,A>::sort(Compare comp) {
    if (_size < 2) return;
    list<T,A> left, right;
    size_type half = _size / 2;
    iterator mid = begin();
    for (size_type i = 0; i < half; ++i) ++mid;
    right.splice(right.end(), *this, mid, end());
    left.splice(left.end(), *this);
    left.sort(comp); right.sort(comp);
    left.merge(right, comp);
    swap(left);
}
template<class T, class A>
inline void list<T,A>::reverse() {
    _Node* n = _sentinel;
    do { _Node* tmp = n->next; n->next = n->prev; n->prev = tmp; n = n->prev; }
    while (n != _sentinel);
}

// ---- non-member operators ----
template<class T, class A>
inline bool operator==(const list<T,A>& x, const list<T,A>& y) {
    if (x.size() != y.size()) return false;
    typename list<T,A>::const_iterator xi = x.begin(), yi = y.begin();
    for (; xi != x.end(); ++xi, ++yi) if (!(*xi == *yi)) return false;
    return true;
}
template<class T, class A>
inline bool operator<(const list<T,A>& x, const list<T,A>& y) {
    typename list<T,A>::const_iterator xi = x.begin(), yi = y.begin();
    for (; xi != x.end() && yi != y.end(); ++xi, ++yi) {
        if (*xi < *yi) return true;
        if (*yi < *xi) return false;
    }
    return xi == x.end() && yi != y.end();
}
template<class T, class A>
inline bool operator!=(const list<T,A>& x, const list<T,A>& y) { return !(x == y); }
template<class T, class A>
inline bool operator>(const list<T,A>& x, const list<T,A>& y)  { return y < x; }
template<class T, class A>
inline bool operator>=(const list<T,A>& x, const list<T,A>& y) { return !(x < y); }
template<class T, class A>
inline bool operator<=(const list<T,A>& x, const list<T,A>& y) { return !(y < x); }
template<class T, class A>
inline void swap(list<T,A>& x, list<T,A>& y) { x.swap(y); }

} // namespace std

#endif