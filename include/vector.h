// ISO/IEC 14882:1998(E) - 23.2.4 vector, 23.2.5 vector<bool>

#ifndef _VECTOR_
#define _VECTOR_

#include <memory>
#include <cstddef>
#include <iterator>

namespace std {

template <class T, class Allocator = allocator<T> >
class vector {
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

    explicit vector(const Allocator& = Allocator());
    explicit vector(size_type n, const T& value = T(), const Allocator& = Allocator());
    template <class InputIterator>
    vector(InputIterator first, InputIterator last, const Allocator& = Allocator());
    vector(const vector<T,Allocator>& x);
    ~vector();
    vector<T,Allocator>& operator=(const vector<T,Allocator>& x);
    template <class InputIterator>
    void assign(InputIterator first, InputIterator last);
    void assign(size_type n, const T& u);
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
    size_type capacity() const;
    bool empty() const;
    void reserve(size_type n);

    reference operator[](size_type n);
    const_reference operator[](size_type n) const;
    reference at(size_type n);
    const_reference at(size_type n) const;
    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;

    void push_back(const T& x);
    void pop_back();
    iterator insert(iterator position, const T& x);
    void insert(iterator position, size_type n, const T& x);
    template <class InputIterator>
    void insert(iterator position, InputIterator first, InputIterator last);
    iterator erase(iterator position);
    iterator erase(iterator first, iterator last);
    void swap(vector<T,Allocator>&);
    void clear();
private:
    T*        _data;
    size_type _size;
    size_type _cap;
    Allocator _alloc;
};

template <class T, class Allocator>
bool operator==(const vector<T,Allocator>& x, const vector<T,Allocator>& y);

template <class T, class Allocator>
bool operator<(const vector<T,Allocator>& x, const vector<T,Allocator>& y);

template <class T, class Allocator>
bool operator!=(const vector<T,Allocator>& x, const vector<T,Allocator>& y);

template <class T, class Allocator>
bool operator>(const vector<T,Allocator>& x, const vector<T,Allocator>& y);

template <class T, class Allocator>
bool operator>=(const vector<T,Allocator>& x, const vector<T,Allocator>& y);

template <class T, class Allocator>
bool operator<=(const vector<T,Allocator>& x, const vector<T,Allocator>& y);

template <class T, class Allocator>
void swap(vector<T,Allocator>& x, vector<T,Allocator>& y);

// ---- vector<T,Allocator> inline implementations ----
template<class T, class A>
inline vector<T,A>::vector(const A& a) : _data(0), _size(0), _cap(0), _alloc(a) {}

template<class T, class A>
inline vector<T,A>::vector(size_type n, const T& v, const A& a)
    : _data(0), _size(0), _cap(0), _alloc(a) {
    if (n > 0) {
        _data = _alloc.allocate(n); _cap = n;
        for (size_type i = 0; i < n; ++i) _alloc.construct(_data + i, v);
        _size = n;
    }
}

template<class T, class A>
template<class InputIterator>
inline vector<T,A>::vector(InputIterator first, InputIterator last, const A& a)
    : _data(0), _size(0), _cap(0), _alloc(a) {
    for (; first != last; ++first) push_back(*first);
}

template<class T, class A>
inline vector<T,A>::vector(const vector<T,A>& x)
    : _data(0), _size(0), _cap(0), _alloc(x._alloc) {
    if (x._size > 0) {
        _data = _alloc.allocate(x._size); _cap = x._size;
        for (size_type i = 0; i < x._size; ++i) _alloc.construct(_data + i, x._data[i]);
        _size = x._size;
    }
}

template<class T, class A>
inline vector<T,A>::~vector() {
    for (size_type i = 0; i < _size; ++i) _alloc.destroy(_data + i);
    if (_data) _alloc.deallocate(_data, _cap);
}

template<class T, class A>
inline vector<T,A>& vector<T,A>::operator=(const vector<T,A>& x) {
    if (this != &x) { vector tmp(x); swap(tmp); }
    return *this;
}

template<class T, class A>
template<class InputIterator>
inline void vector<T,A>::assign(InputIterator first, InputIterator last) {
    clear(); for (; first != last; ++first) push_back(*first);
}

template<class T, class A>
inline void vector<T,A>::assign(size_type n, const T& u) {
    clear(); for (size_type i = 0; i < n; ++i) push_back(u);
}

template<class T, class A>
inline typename vector<T,A>::allocator_type vector<T,A>::get_allocator() const { return _alloc; }

template<class T, class A>
inline typename vector<T,A>::iterator vector<T,A>::begin() { return _data; }
template<class T, class A>
inline typename vector<T,A>::const_iterator vector<T,A>::begin() const { return _data; }
template<class T, class A>
inline typename vector<T,A>::iterator vector<T,A>::end() { return _data + _size; }
template<class T, class A>
inline typename vector<T,A>::const_iterator vector<T,A>::end() const { return _data + _size; }
template<class T, class A>
inline typename vector<T,A>::reverse_iterator vector<T,A>::rbegin() { return reverse_iterator(end()); }
template<class T, class A>
inline typename vector<T,A>::const_reverse_iterator vector<T,A>::rbegin() const { return const_reverse_iterator(end()); }
template<class T, class A>
inline typename vector<T,A>::reverse_iterator vector<T,A>::rend() { return reverse_iterator(begin()); }
template<class T, class A>
inline typename vector<T,A>::const_reverse_iterator vector<T,A>::rend() const { return const_reverse_iterator(begin()); }

template<class T, class A>
inline typename vector<T,A>::size_type vector<T,A>::size() const { return _size; }
template<class T, class A>
inline typename vector<T,A>::size_type vector<T,A>::max_size() const { return size_type(-1) / sizeof(T); }
template<class T, class A>
inline void vector<T,A>::resize(size_type sz, T c) {
    while (_size > sz) pop_back();
    while (_size < sz) push_back(c);
}
template<class T, class A>
inline typename vector<T,A>::size_type vector<T,A>::capacity() const { return _cap; }
template<class T, class A>
inline bool vector<T,A>::empty() const { return _size == 0; }
template<class T, class A>
inline void vector<T,A>::reserve(size_type n) {
    if (n <= _cap) return;
    T* nd = _alloc.allocate(n);
    for (size_type i = 0; i < _size; ++i) _alloc.construct(nd + i, _data[i]);
    for (size_type i = 0; i < _size; ++i) _alloc.destroy(_data + i);
    if (_data) _alloc.deallocate(_data, _cap);
    _data = nd; _cap = n;
}

template<class T, class A>
inline typename vector<T,A>::reference vector<T,A>::operator[](size_type n) { return _data[n]; }
template<class T, class A>
inline typename vector<T,A>::const_reference vector<T,A>::operator[](size_type n) const { return _data[n]; }
template<class T, class A>
inline typename vector<T,A>::reference vector<T,A>::at(size_type n) { return _data[n]; }
template<class T, class A>
inline typename vector<T,A>::const_reference vector<T,A>::at(size_type n) const { return _data[n]; }
template<class T, class A>
inline typename vector<T,A>::reference vector<T,A>::front() { return _data[0]; }
template<class T, class A>
inline typename vector<T,A>::const_reference vector<T,A>::front() const { return _data[0]; }
template<class T, class A>
inline typename vector<T,A>::reference vector<T,A>::back() { return _data[_size - 1]; }
template<class T, class A>
inline typename vector<T,A>::const_reference vector<T,A>::back() const { return _data[_size - 1]; }

template<class T, class A>
inline void vector<T,A>::push_back(const T& x) {
    if (_size == _cap) reserve(_cap + _cap / 2 + 1);
    _alloc.construct(_data + _size, x);
    ++_size;
}
template<class T, class A>
inline void vector<T,A>::pop_back() {
    _alloc.destroy(_data + _size - 1);
    --_size;
}

template<class T, class A>
inline typename vector<T,A>::iterator vector<T,A>::insert(iterator pos, const T& x) {
    size_type idx = size_type(pos - _data);
    if (_size == _cap) reserve(_cap + _cap / 2 + 1);
    if (idx < _size) {
        _alloc.construct(_data + _size, _data[_size - 1]);
        for (size_type i = _size - 1; i > idx; --i) _data[i] = _data[i - 1];
        _data[idx] = x;
    } else {
        _alloc.construct(_data + _size, x);
    }
    ++_size;
    return _data + idx;
}
template<class T, class A>
inline void vector<T,A>::insert(iterator pos, size_type n, const T& x) {
    size_type idx = size_type(pos - _data);
    for (size_type i = 0; i < n; ++i) insert(_data + idx + i, x);
}
template<class T, class A>
template<class InputIterator>
inline void vector<T,A>::insert(iterator pos, InputIterator first, InputIterator last) {
    size_type idx = size_type(pos - _data);
    for (; first != last; ++first, ++idx) insert(_data + idx, *first);
}

template<class T, class A>
inline typename vector<T,A>::iterator vector<T,A>::erase(iterator pos) {
    size_type idx = size_type(pos - _data);
    _alloc.destroy(_data + idx);
    for (size_type i = idx; i + 1 < _size; ++i) {
        _alloc.construct(_data + i, _data[i + 1]);
        _alloc.destroy(_data + i + 1);
    }
    --_size;
    return _data + idx;
}
template<class T, class A>
inline typename vector<T,A>::iterator vector<T,A>::erase(iterator first, iterator last) {
    size_type idx = size_type(first - _data);
    size_type n   = size_type(last - first);
    for (size_type i = 0; i < n; ++i) erase(_data + idx);
    return _data + idx;
}

template<class T, class A>
inline void vector<T,A>::swap(vector<T,A>& x) {
    T* td = _data; _data = x._data; x._data = td;
    size_type ts = _size; _size = x._size; x._size = ts;
    size_type tc = _cap;  _cap  = x._cap;  x._cap  = tc;
    A ta = _alloc; _alloc = x._alloc; x._alloc = ta;
}
template<class T, class A>
inline void vector<T,A>::clear() {
    for (size_type i = 0; i < _size; ++i) _alloc.destroy(_data + i);
    _size = 0;
}

// ---- non-member operator implementations ----
template<class T, class A>
inline bool operator==(const vector<T,A>& x, const vector<T,A>& y) {
    if (x.size() != y.size()) return false;
    for (typename vector<T,A>::size_type i = 0; i < x.size(); ++i)
        if (!(x[i] == y[i])) return false;
    return true;
}
template<class T, class A>
inline bool operator<(const vector<T,A>& x, const vector<T,A>& y) {
    typename vector<T,A>::size_type n = x.size() < y.size() ? x.size() : y.size();
    for (typename vector<T,A>::size_type i = 0; i < n; ++i) {
        if (x[i] < y[i]) return true;
        if (y[i] < x[i]) return false;
    }
    return x.size() < y.size();
}
template<class T, class A>
inline bool operator!=(const vector<T,A>& x, const vector<T,A>& y) { return !(x == y); }
template<class T, class A>
inline bool operator>(const vector<T,A>& x, const vector<T,A>& y) { return y < x; }
template<class T, class A>
inline bool operator>=(const vector<T,A>& x, const vector<T,A>& y) { return !(x < y); }
template<class T, class A>
inline bool operator<=(const vector<T,A>& x, const vector<T,A>& y) { return !(y < x); }
template<class T, class A>
inline void swap(vector<T,A>& x, vector<T,A>& y) { x.swap(y); }

template <class Allocator>
class vector<bool, Allocator> {
public:
    typedef bool const_reference;
    typedef bool value_type;
    typedef Allocator allocator_type;
    typedef bool* pointer;                  // provisional proxy simplification
    typedef const bool* const_pointer;      // provisional proxy simplification
    typedef pointer iterator;               // provisional contiguous-iterator profile
    typedef const_pointer const_iterator;   // provisional contiguous-iterator profile
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    class reference {
        friend class vector;
        reference();
    public:
        ~reference();
        operator bool() const;
        reference& operator=(const bool x);
        reference& operator=(const reference& x);
        void flip();
    };

    explicit vector(const Allocator& = Allocator());
    explicit vector(size_type n, const bool& value = bool(), const Allocator& = Allocator());
    template <class InputIterator>
    vector(InputIterator first, InputIterator last, const Allocator& = Allocator());
    vector(const vector<bool,Allocator>& x);
    ~vector();
    vector<bool,Allocator>& operator=(const vector<bool,Allocator>& x);
    template <class InputIterator>
    void assign(InputIterator first, InputIterator last);
    void assign(size_type n, const bool& t);
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
    void resize(size_type sz, bool c = false);
    size_type capacity() const;
    bool empty() const;
    void reserve(size_type n);

    reference operator[](size_type n);
    const_reference operator[](size_type n) const;
    reference at(size_type n);
    const_reference at(size_type n) const;
    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;

    void push_back(const bool& x);
    void pop_back();
    iterator insert(iterator position, const bool& x);
    void insert(iterator position, size_type n, const bool& x);
    template <class InputIterator>
    void insert(iterator position, InputIterator first, InputIterator last);
    iterator erase(iterator position);
    iterator erase(iterator first, iterator last);
    void swap(vector<bool,Allocator>&);
    static void swap(reference x, reference y);
    void flip();
    void clear();
};

template <class Allocator>
bool operator==(const vector<bool,Allocator>& x, const vector<bool,Allocator>& y);

template <class Allocator>
bool operator<(const vector<bool,Allocator>& x, const vector<bool,Allocator>& y);

template <class Allocator>
bool operator!=(const vector<bool,Allocator>& x, const vector<bool,Allocator>& y);

template <class Allocator>
bool operator>(const vector<bool,Allocator>& x, const vector<bool,Allocator>& y);

template <class Allocator>
bool operator>=(const vector<bool,Allocator>& x, const vector<bool,Allocator>& y);

template <class Allocator>
bool operator<=(const vector<bool,Allocator>& x, const vector<bool,Allocator>& y);

template <class Allocator>
void swap(vector<bool,Allocator>& x, vector<bool,Allocator>& y);

} // namespace std

#endif