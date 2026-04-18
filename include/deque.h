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
private:
    // Contiguous-storage deque: elements live in _data[_front .. _front+_size).
    // push_front shifts or reallocates with front headroom; push_back grows at end.
    T*        _data;
    size_type _front;  // index of first live element
    size_type _size;
    size_type _cap;    // total allocated slots
    Allocator _alloc;
    void _grow(size_type extra_front, size_type extra_back);
};

// ---- deque<T,Allocator> inline implementations ----
template<class T, class A>
inline void deque<T,A>::_grow(size_type ef, size_type eb) {
    size_type new_cap = _cap + ef + eb + _cap / 2 + 2;
    T* nd = _alloc.allocate(new_cap);
    size_type new_front = ef + (_cap / 4 < ef ? 0 : _cap / 4);
    if (new_front + _size > new_cap - eb) new_front = 0;
    for (size_type i = 0; i < _size; ++i) {
        _alloc.construct(nd + new_front + i, _data[_front + i]);
        _alloc.destroy(_data + _front + i);
    }
    if (_data) _alloc.deallocate(_data, _cap);
    _data = nd; _front = new_front; _cap = new_cap;
}

template<class T, class A>
inline deque<T,A>::deque(const A& a) : _data(0), _front(0), _size(0), _cap(0), _alloc(a) {}

template<class T, class A>
inline deque<T,A>::deque(size_type n, const T& v, const A& a)
    : _data(0), _front(0), _size(0), _cap(0), _alloc(a) {
    for (size_type i = 0; i < n; ++i) push_back(v);
}

template<class T, class A>
template<class InputIterator>
inline deque<T,A>::deque(InputIterator first, InputIterator last, const A& a)
    : _data(0), _front(0), _size(0), _cap(0), _alloc(a) {
    for (; first != last; ++first) push_back(*first);
}

template<class T, class A>
inline deque<T,A>::deque(const deque<T,A>& x)
    : _data(0), _front(0), _size(0), _cap(0), _alloc(x._alloc) {
    for (size_type i = 0; i < x._size; ++i) push_back(x._data[x._front + i]);
}

template<class T, class A>
inline deque<T,A>::~deque() {
    for (size_type i = 0; i < _size; ++i) _alloc.destroy(_data + _front + i);
    if (_data) _alloc.deallocate(_data, _cap);
}

template<class T, class A>
inline deque<T,A>& deque<T,A>::operator=(const deque<T,A>& x) {
    if (this != &x) { deque tmp(x); swap(tmp); }
    return *this;
}

template<class T, class A>
template<class InputIterator>
inline void deque<T,A>::assign(InputIterator first, InputIterator last) {
    clear(); for (; first != last; ++first) push_back(*first);
}
template<class T, class A>
inline void deque<T,A>::assign(size_type n, const T& t) {
    clear(); for (size_type i = 0; i < n; ++i) push_back(t);
}
template<class T, class A>
inline typename deque<T,A>::allocator_type deque<T,A>::get_allocator() const { return _alloc; }

template<class T, class A>
inline typename deque<T,A>::iterator deque<T,A>::begin() { return _data + _front; }
template<class T, class A>
inline typename deque<T,A>::const_iterator deque<T,A>::begin() const { return _data + _front; }
template<class T, class A>
inline typename deque<T,A>::iterator deque<T,A>::end() { return _data + _front + _size; }
template<class T, class A>
inline typename deque<T,A>::const_iterator deque<T,A>::end() const { return _data + _front + _size; }
template<class T, class A>
inline typename deque<T,A>::reverse_iterator deque<T,A>::rbegin() { return reverse_iterator(end()); }
template<class T, class A>
inline typename deque<T,A>::const_reverse_iterator deque<T,A>::rbegin() const { return const_reverse_iterator(end()); }
template<class T, class A>
inline typename deque<T,A>::reverse_iterator deque<T,A>::rend() { return reverse_iterator(begin()); }
template<class T, class A>
inline typename deque<T,A>::const_reverse_iterator deque<T,A>::rend() const { return const_reverse_iterator(begin()); }

template<class T, class A>
inline typename deque<T,A>::size_type deque<T,A>::size() const { return _size; }
template<class T, class A>
inline typename deque<T,A>::size_type deque<T,A>::max_size() const { return size_type(-1) / sizeof(T); }
template<class T, class A>
inline void deque<T,A>::resize(size_type sz, T c) {
    while (_size > sz) pop_back();
    while (_size < sz) push_back(c);
}
template<class T, class A>
inline bool deque<T,A>::empty() const { return _size == 0; }

template<class T, class A>
inline typename deque<T,A>::reference deque<T,A>::operator[](size_type n) { return _data[_front + n]; }
template<class T, class A>
inline typename deque<T,A>::const_reference deque<T,A>::operator[](size_type n) const { return _data[_front + n]; }
template<class T, class A>
inline typename deque<T,A>::reference deque<T,A>::at(size_type n) { return _data[_front + n]; }
template<class T, class A>
inline typename deque<T,A>::const_reference deque<T,A>::at(size_type n) const { return _data[_front + n]; }
template<class T, class A>
inline typename deque<T,A>::reference deque<T,A>::front() { return _data[_front]; }
template<class T, class A>
inline typename deque<T,A>::const_reference deque<T,A>::front() const { return _data[_front]; }
template<class T, class A>
inline typename deque<T,A>::reference deque<T,A>::back() { return _data[_front + _size - 1]; }
template<class T, class A>
inline typename deque<T,A>::const_reference deque<T,A>::back() const { return _data[_front + _size - 1]; }

template<class T, class A>
inline void deque<T,A>::push_front(const T& x) {
    if (_front == 0) _grow(8, 0);
    --_front;
    _alloc.construct(_data + _front, x);
    ++_size;
}
template<class T, class A>
inline void deque<T,A>::push_back(const T& x) {
    if (_front + _size >= _cap) _grow(0, 8);
    _alloc.construct(_data + _front + _size, x);
    ++_size;
}
template<class T, class A>
inline void deque<T,A>::pop_front() {
    _alloc.destroy(_data + _front);
    ++_front; --_size;
}
template<class T, class A>
inline void deque<T,A>::pop_back() {
    _alloc.destroy(_data + _front + _size - 1);
    --_size;
}

template<class T, class A>
inline typename deque<T,A>::iterator deque<T,A>::insert(iterator pos, const T& x) {
    size_type idx = size_type(pos - (_data + _front));
    push_back(x); // ensure space
    // shift right from idx
    for (size_type i = _size - 1; i > idx; --i)
        _data[_front + i] = _data[_front + i - 1];
    _data[_front + idx] = x;
    return _data + _front + idx;
}
template<class T, class A>
inline void deque<T,A>::insert(iterator pos, size_type n, const T& x) {
    size_type idx = size_type(pos - (_data + _front));
    for (size_type i = 0; i < n; ++i) insert(_data + _front + idx + i, x);
}
template<class T, class A>
template<class InputIterator>
inline void deque<T,A>::insert(iterator pos, InputIterator first, InputIterator last) {
    size_type idx = size_type(pos - (_data + _front));
    for (; first != last; ++first, ++idx) insert(_data + _front + idx, *first);
}
template<class T, class A>
inline typename deque<T,A>::iterator deque<T,A>::erase(iterator pos) {
    size_type idx = size_type(pos - (_data + _front));
    _alloc.destroy(_data + _front + idx);
    for (size_type i = idx; i + 1 < _size; ++i) {
        _alloc.construct(_data + _front + i, _data[_front + i + 1]);
        _alloc.destroy(_data + _front + i + 1);
    }
    --_size;
    return _data + _front + idx;
}
template<class T, class A>
inline typename deque<T,A>::iterator deque<T,A>::erase(iterator first, iterator last) {
    size_type idx = size_type(first - (_data + _front));
    size_type n   = size_type(last - first);
    for (size_type i = 0; i < n; ++i) erase(_data + _front + idx);
    return _data + _front + idx;
}
template<class T, class A>
inline void deque<T,A>::swap(deque<T,A>& x) {
    T* td = _data; _data = x._data; x._data = td;
    size_type tf = _front; _front = x._front; x._front = tf;
    size_type ts = _size;  _size  = x._size;  x._size  = ts;
    size_type tc = _cap;   _cap   = x._cap;   x._cap   = tc;
    A ta = _alloc; _alloc = x._alloc; x._alloc = ta;
}
template<class T, class A>
inline void deque<T,A>::clear() {
    for (size_type i = 0; i < _size; ++i) _alloc.destroy(_data + _front + i);
    _size = 0; _front = 0;
}

// ---- non-member operators ----
template<class T, class A>
inline bool operator==(const deque<T,A>& x, const deque<T,A>& y) {
    if (x.size() != y.size()) return false;
    for (typename deque<T,A>::size_type i = 0; i < x.size(); ++i)
        if (!(x[i] == y[i])) return false;
    return true;
}
template<class T, class A>
inline bool operator<(const deque<T,A>& x, const deque<T,A>& y) {
    typename deque<T,A>::size_type n = x.size() < y.size() ? x.size() : y.size();
    for (typename deque<T,A>::size_type i = 0; i < n; ++i) {
        if (x[i] < y[i]) return true;
        if (y[i] < x[i]) return false;
    }
    return x.size() < y.size();
}
template<class T, class A>
inline bool operator!=(const deque<T,A>& x, const deque<T,A>& y) { return !(x == y); }
template<class T, class A>
inline bool operator>(const deque<T,A>& x, const deque<T,A>& y)  { return y < x; }
template<class T, class A>
inline bool operator>=(const deque<T,A>& x, const deque<T,A>& y) { return !(x < y); }
template<class T, class A>
inline bool operator<=(const deque<T,A>& x, const deque<T,A>& y) { return !(y < x); }
template<class T, class A>
inline void swap(deque<T,A>& x, deque<T,A>& y) { x.swap(y); }

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