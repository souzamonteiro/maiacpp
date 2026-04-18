// ISO/IEC 14882:1998(E) - 23.3.3 set, 23.3.4 multiset

#ifndef _SET_
#define _SET_

#include <memory>
#include <functional>
#include <iterator>

namespace std {

template <class Key, class Compare = less<Key>,
          class Allocator = allocator<Key> >
class set {
public:
    typedef Key key_type;
    typedef Key value_type;
    typedef Compare key_compare;
    typedef Compare value_compare;
    typedef Allocator allocator_type;
    typedef typename Allocator::reference reference;
    typedef typename Allocator::const_reference const_reference;
    typedef value_type* iterator;               // provisional iterator profile
    typedef const value_type* const_iterator;   // provisional iterator profile
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef typename Allocator::pointer pointer;
    typedef typename Allocator::const_pointer const_pointer;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    explicit set(const Compare& comp = Compare(), const Allocator& = Allocator());
    template <class InputIterator>
    set(InputIterator first, InputIterator last,
        const Compare& comp = Compare(), const Allocator& = Allocator());
    set(const set<Key,Compare,Allocator>& x);
    ~set();
    set<Key,Compare,Allocator>& operator=(const set<Key,Compare,Allocator>& x);
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

    pair<iterator, bool> insert(const value_type& x);
    iterator insert(iterator position, const value_type& x);
    template <class InputIterator>
    void insert(InputIterator first, InputIterator last);
    void erase(iterator position);
    size_type erase(const key_type& x);
    void erase(iterator first, iterator last);
    void swap(set<Key,Compare,Allocator>&);
    void clear();

    key_compare key_comp() const;
    value_compare value_comp() const;

    iterator find(const key_type& x) const;
    size_type count(const key_type& x) const;
    iterator lower_bound(const key_type& x) const;
    iterator upper_bound(const key_type& x) const;
    pair<iterator, iterator> equal_range(const key_type& x) const;
private:
    Key*      _data;
    size_type _size;
    size_type _cap;
    Compare   _comp;
    Allocator _alloc;
    void _reserve(size_type n);
    size_type _lower_idx(const Key& k) const;
};

template <class Key, class Compare, class Allocator>
bool operator==(const set<Key,Compare,Allocator>& x,
                const set<Key,Compare,Allocator>& y);

template <class Key, class Compare, class Allocator>
bool operator<(const set<Key,Compare,Allocator>& x,
               const set<Key,Compare,Allocator>& y);

template <class Key, class Compare, class Allocator>
bool operator!=(const set<Key,Compare,Allocator>& x,
                const set<Key,Compare,Allocator>& y);

template <class Key, class Compare, class Allocator>
bool operator>(const set<Key,Compare,Allocator>& x,
               const set<Key,Compare,Allocator>& y);

template <class Key, class Compare, class Allocator>
bool operator>=(const set<Key,Compare,Allocator>& x,
                const set<Key,Compare,Allocator>& y);

template <class Key, class Compare, class Allocator>
bool operator<=(const set<Key,Compare,Allocator>& x,
                const set<Key,Compare,Allocator>& y);

template <class Key, class Compare, class Allocator>
void swap(set<Key,Compare,Allocator>& x,
          set<Key,Compare,Allocator>& y);

template <class Key, class Compare = less<Key>,
          class Allocator = allocator<Key> >
class multiset {
public:
    // types (same as set)
    typedef Key key_type;
    typedef Key value_type;
    typedef Compare key_compare;
    typedef Compare value_compare;
    typedef Allocator allocator_type;
    typedef typename Allocator::reference reference;
    typedef typename Allocator::const_reference const_reference;
    typedef value_type* iterator;               // provisional iterator profile
    typedef const value_type* const_iterator;   // provisional iterator profile
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef typename Allocator::pointer pointer;
    typedef typename Allocator::const_pointer const_pointer;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    explicit multiset(const Compare& comp = Compare(), const Allocator& = Allocator());
    template <class InputIterator>
    multiset(InputIterator first, InputIterator last,
             const Compare& comp = Compare(), const Allocator& = Allocator());
    multiset(const multiset<Key,Compare,Allocator>& x);
    ~multiset();
    multiset<Key,Compare,Allocator>& operator=(const multiset<Key,Compare,Allocator>& x);
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

    iterator insert(const value_type& x);
    iterator insert(iterator position, const value_type& x);
    template <class InputIterator>
    void insert(InputIterator first, InputIterator last);
    void erase(iterator position);
    size_type erase(const key_type& x);
    void erase(iterator first, iterator last);
    void swap(multiset<Key,Compare,Allocator>&);
    void clear();

    key_compare key_comp() const;
    value_compare value_comp() const;

    iterator find(const key_type& x) const;
    size_type count(const key_type& x) const;
    iterator lower_bound(const key_type& x) const;
    iterator upper_bound(const key_type& x) const;
    pair<iterator, iterator> equal_range(const key_type& x) const;
private:
    Key*      _data;
    size_type _size;
    size_type _cap;
    Compare   _comp;
    Allocator _alloc;
    void _reserve(size_type n);
    size_type _lower_idx(const Key& k) const;
};

template <class Key, class Compare, class Allocator>
bool operator==(const multiset<Key,Compare,Allocator>& x,
                const multiset<Key,Compare,Allocator>& y);

template <class Key, class Compare, class Allocator>
bool operator<(const multiset<Key,Compare,Allocator>& x,
               const multiset<Key,Compare,Allocator>& y);

// Additional comparison operators similarly defined

template <class Key, class Compare, class Allocator>
void swap(multiset<Key,Compare,Allocator>& x,
          multiset<Key,Compare,Allocator>& y);

// ---- set<Key,Compare,Allocator> inline implementations ----
template<class K, class C, class A>
inline void set<K,C,A>::_reserve(size_type n) {
    if (n <= _cap) return;
    K* nd = reinterpret_cast<K*>(::operator new(n * sizeof(K)));
    for (size_type i = 0; i < _size; ++i) { new (nd + i) K(_data[i]); _data[i].~K(); }
    if (_data) ::operator delete(_data);
    _data = nd; _cap = n;
}
template<class K, class C, class A>
inline typename set<K,C,A>::size_type set<K,C,A>::_lower_idx(const K& k) const {
    size_type lo = 0, hi = _size;
    while (lo < hi) { size_type mid = lo + (hi-lo)/2; if (_comp(_data[mid], k)) lo = mid+1; else hi = mid; }
    return lo;
}
template<class K, class C, class A>
inline set<K,C,A>::set(const C& c, const A& a) : _data(0), _size(0), _cap(0), _comp(c), _alloc(a) {}
template<class K, class C, class A>
template<class InputIterator>
inline set<K,C,A>::set(InputIterator first, InputIterator last, const C& c, const A& a)
    : _data(0), _size(0), _cap(0), _comp(c), _alloc(a) { for (; first != last; ++first) insert(*first); }
template<class K, class C, class A>
inline set<K,C,A>::set(const set<K,C,A>& x) : _data(0), _size(0), _cap(0), _comp(x._comp), _alloc(x._alloc) {
    for (size_type i = 0; i < x._size; ++i) insert(x._data[i]);
}
template<class K, class C, class A>
inline set<K,C,A>::~set() { for (size_type i = 0; i < _size; ++i) _data[i].~K(); if (_data) ::operator delete(_data); }
template<class K, class C, class A>
inline set<K,C,A>& set<K,C,A>::operator=(const set<K,C,A>& x) { if (this != &x) { set tmp(x); swap(tmp); } return *this; }
template<class K, class C, class A>
inline typename set<K,C,A>::allocator_type set<K,C,A>::get_allocator() const { return _alloc; }
template<class K, class C, class A>
inline typename set<K,C,A>::iterator set<K,C,A>::begin() { return _data; }
template<class K, class C, class A>
inline typename set<K,C,A>::const_iterator set<K,C,A>::begin() const { return _data; }
template<class K, class C, class A>
inline typename set<K,C,A>::iterator set<K,C,A>::end() { return _data + _size; }
template<class K, class C, class A>
inline typename set<K,C,A>::const_iterator set<K,C,A>::end() const { return _data + _size; }
template<class K, class C, class A>
inline typename set<K,C,A>::reverse_iterator set<K,C,A>::rbegin() { return reverse_iterator(end()); }
template<class K, class C, class A>
inline typename set<K,C,A>::const_reverse_iterator set<K,C,A>::rbegin() const { return const_reverse_iterator(end()); }
template<class K, class C, class A>
inline typename set<K,C,A>::reverse_iterator set<K,C,A>::rend() { return reverse_iterator(begin()); }
template<class K, class C, class A>
inline typename set<K,C,A>::const_reverse_iterator set<K,C,A>::rend() const { return const_reverse_iterator(begin()); }
template<class K, class C, class A>
inline bool set<K,C,A>::empty() const { return _size == 0; }
template<class K, class C, class A>
inline typename set<K,C,A>::size_type set<K,C,A>::size() const { return _size; }
template<class K, class C, class A>
inline typename set<K,C,A>::size_type set<K,C,A>::max_size() const { return size_type(-1) / sizeof(K); }
template<class K, class C, class A>
inline pair<typename set<K,C,A>::iterator, bool> set<K,C,A>::insert(const K& x) {
    size_type idx = _lower_idx(x);
    if (idx < _size && !_comp(x, _data[idx])) return pair<iterator,bool>(_data + idx, false);
    _reserve(_cap + _cap/2 + 2);
    for (size_type i = _size; i > idx; --i) { new (_data+i) K(_data[i-1]); _data[i-1].~K(); }
    new (_data + idx) K(x);
    ++_size;
    return pair<iterator,bool>(_data + idx, true);
}
template<class K, class C, class A>
inline typename set<K,C,A>::iterator set<K,C,A>::insert(iterator, const K& x) { return insert(x).first; }
template<class K, class C, class A>
template<class InputIterator>
inline void set<K,C,A>::insert(InputIterator first, InputIterator last) { for (; first != last; ++first) insert(*first); }
template<class K, class C, class A>
inline void set<K,C,A>::erase(iterator pos) {
    size_type idx = size_type(pos - _data);
    _data[idx].~K();
    for (size_type i = idx; i+1 < _size; ++i) { new (_data+i) K(_data[i+1]); _data[i+1].~K(); }
    --_size;
}
template<class K, class C, class A>
inline typename set<K,C,A>::size_type set<K,C,A>::erase(const K& x) {
    size_type idx = _lower_idx(x);
    if (idx >= _size || _comp(x, _data[idx])) return 0;
    erase(_data + idx); return 1;
}
template<class K, class C, class A>
inline void set<K,C,A>::erase(iterator first, iterator last) {
    while (first != last) { iterator nx = first; ++nx; erase(first); first = nx; }
}
template<class K, class C, class A>
inline void set<K,C,A>::swap(set<K,C,A>& x) {
    K* td = _data; _data = x._data; x._data = td;
    size_type ts = _size; _size = x._size; x._size = ts;
    size_type tc = _cap;  _cap  = x._cap;  x._cap  = tc;
    C tc2 = _comp; _comp = x._comp; x._comp = tc2;
}
template<class K, class C, class A>
inline void set<K,C,A>::clear() { for (size_type i = 0; i < _size; ++i) _data[i].~K(); _size = 0; }
template<class K, class C, class A>
inline typename set<K,C,A>::key_compare set<K,C,A>::key_comp() const { return _comp; }
template<class K, class C, class A>
inline typename set<K,C,A>::value_compare set<K,C,A>::value_comp() const { return _comp; }
template<class K, class C, class A>
inline typename set<K,C,A>::iterator set<K,C,A>::find(const K& x) const {
    size_type idx = _lower_idx(x);
    return (idx < _size && !_comp(x, _data[idx])) ? const_cast<K*>(_data + idx) : const_cast<K*>(_data + _size);
}
template<class K, class C, class A>
inline typename set<K,C,A>::size_type set<K,C,A>::count(const K& x) const { return find(x) != end() ? 1 : 0; }
template<class K, class C, class A>
inline typename set<K,C,A>::iterator set<K,C,A>::lower_bound(const K& x) const { return const_cast<K*>(_data + _lower_idx(x)); }
template<class K, class C, class A>
inline typename set<K,C,A>::iterator set<K,C,A>::upper_bound(const K& x) const {
    size_type idx = _lower_idx(x);
    if (idx < _size && !_comp(x, _data[idx])) ++idx;
    return const_cast<K*>(_data + idx);
}
template<class K, class C, class A>
inline pair<typename set<K,C,A>::iterator, typename set<K,C,A>::iterator>
set<K,C,A>::equal_range(const K& x) const { return pair<iterator,iterator>(lower_bound(x), upper_bound(x)); }

template<class K, class C, class A>
inline bool operator==(const set<K,C,A>& x, const set<K,C,A>& y) {
    if (x.size() != y.size()) return false;
    typename set<K,C,A>::const_iterator xi = x.begin(), yi = y.begin();
    for (; xi != x.end(); ++xi, ++yi) if (!(*xi == *yi)) return false;
    return true;
}
template<class K, class C, class A>
inline bool operator<(const set<K,C,A>& x, const set<K,C,A>& y) {
    typename set<K,C,A>::const_iterator xi = x.begin(), yi = y.begin();
    for (; xi != x.end() && yi != y.end(); ++xi, ++yi) {
        if (*xi < *yi) return true; if (*yi < *xi) return false;
    }
    return xi == x.end() && yi != y.end();
}
template<class K, class C, class A>
inline bool operator!=(const set<K,C,A>& x, const set<K,C,A>& y) { return !(x == y); }
template<class K, class C, class A>
inline bool operator>(const set<K,C,A>& x, const set<K,C,A>& y)  { return y < x; }
template<class K, class C, class A>
inline bool operator>=(const set<K,C,A>& x, const set<K,C,A>& y) { return !(x < y); }
template<class K, class C, class A>
inline bool operator<=(const set<K,C,A>& x, const set<K,C,A>& y) { return !(y < x); }
template<class K, class C, class A>
inline void swap(set<K,C,A>& x, set<K,C,A>& y) { x.swap(y); }

// ---- multiset<Key,Compare,Allocator> inline implementations ----
template<class K, class C, class A>
inline void multiset<K,C,A>::_reserve(size_type n) {
    if (n <= _cap) return;
    K* nd = reinterpret_cast<K*>(::operator new(n * sizeof(K)));
    for (size_type i = 0; i < _size; ++i) { new (nd+i) K(_data[i]); _data[i].~K(); }
    if (_data) ::operator delete(_data);
    _data = nd; _cap = n;
}
template<class K, class C, class A>
inline typename multiset<K,C,A>::size_type multiset<K,C,A>::_lower_idx(const K& k) const {
    size_type lo = 0, hi = _size;
    while (lo < hi) { size_type mid = lo + (hi-lo)/2; if (_comp(_data[mid], k)) lo = mid+1; else hi = mid; }
    return lo;
}
template<class K, class C, class A>
inline multiset<K,C,A>::multiset(const C& c, const A& a) : _data(0), _size(0), _cap(0), _comp(c), _alloc(a) {}
template<class K, class C, class A>
template<class InputIterator>
inline multiset<K,C,A>::multiset(InputIterator first, InputIterator last, const C& c, const A& a)
    : _data(0), _size(0), _cap(0), _comp(c), _alloc(a) { for (; first != last; ++first) insert(*first); }
template<class K, class C, class A>
inline multiset<K,C,A>::multiset(const multiset<K,C,A>& x) : _data(0), _size(0), _cap(0), _comp(x._comp), _alloc(x._alloc) {
    for (size_type i = 0; i < x._size; ++i) insert(x._data[i]);
}
template<class K, class C, class A>
inline multiset<K,C,A>::~multiset() { for (size_type i = 0; i < _size; ++i) _data[i].~K(); if (_data) ::operator delete(_data); }
template<class K, class C, class A>
inline multiset<K,C,A>& multiset<K,C,A>::operator=(const multiset<K,C,A>& x) { if (this != &x) { multiset tmp(x); swap(tmp); } return *this; }
template<class K, class C, class A>
inline typename multiset<K,C,A>::allocator_type multiset<K,C,A>::get_allocator() const { return _alloc; }
template<class K, class C, class A>
inline typename multiset<K,C,A>::iterator multiset<K,C,A>::begin() { return _data; }
template<class K, class C, class A>
inline typename multiset<K,C,A>::const_iterator multiset<K,C,A>::begin() const { return _data; }
template<class K, class C, class A>
inline typename multiset<K,C,A>::iterator multiset<K,C,A>::end() { return _data + _size; }
template<class K, class C, class A>
inline typename multiset<K,C,A>::const_iterator multiset<K,C,A>::end() const { return _data + _size; }
template<class K, class C, class A>
inline typename multiset<K,C,A>::reverse_iterator multiset<K,C,A>::rbegin() { return reverse_iterator(end()); }
template<class K, class C, class A>
inline typename multiset<K,C,A>::const_reverse_iterator multiset<K,C,A>::rbegin() const { return const_reverse_iterator(end()); }
template<class K, class C, class A>
inline typename multiset<K,C,A>::reverse_iterator multiset<K,C,A>::rend() { return reverse_iterator(begin()); }
template<class K, class C, class A>
inline typename multiset<K,C,A>::const_reverse_iterator multiset<K,C,A>::rend() const { return const_reverse_iterator(begin()); }
template<class K, class C, class A>
inline bool multiset<K,C,A>::empty() const { return _size == 0; }
template<class K, class C, class A>
inline typename multiset<K,C,A>::size_type multiset<K,C,A>::size() const { return _size; }
template<class K, class C, class A>
inline typename multiset<K,C,A>::size_type multiset<K,C,A>::max_size() const { return size_type(-1) / sizeof(K); }
template<class K, class C, class A>
inline typename multiset<K,C,A>::iterator multiset<K,C,A>::insert(const K& x) {
    size_type idx = _lower_idx(x);
    while (idx < _size && !_comp(x, _data[idx])) ++idx;
    _reserve(_cap + _cap/2 + 2);
    for (size_type i = _size; i > idx; --i) { new (_data+i) K(_data[i-1]); _data[i-1].~K(); }
    new (_data + idx) K(x);
    ++_size;
    return _data + idx;
}
template<class K, class C, class A>
inline typename multiset<K,C,A>::iterator multiset<K,C,A>::insert(iterator, const K& x) { return insert(x); }
template<class K, class C, class A>
template<class InputIterator>
inline void multiset<K,C,A>::insert(InputIterator first, InputIterator last) { for (; first != last; ++first) insert(*first); }
template<class K, class C, class A>
inline void multiset<K,C,A>::erase(iterator pos) {
    size_type idx = size_type(pos - _data);
    _data[idx].~K();
    for (size_type i = idx; i+1 < _size; ++i) { new (_data+i) K(_data[i+1]); _data[i+1].~K(); }
    --_size;
}
template<class K, class C, class A>
inline typename multiset<K,C,A>::size_type multiset<K,C,A>::erase(const K& x) {
    size_type cnt = 0, lo = _lower_idx(x);
    while (lo < _size && !_comp(x, _data[lo])) { erase(_data + lo); ++cnt; }
    return cnt;
}
template<class K, class C, class A>
inline void multiset<K,C,A>::erase(iterator first, iterator last) {
    while (first != last) { iterator nx = first; ++nx; erase(first); first = nx; }
}
template<class K, class C, class A>
inline void multiset<K,C,A>::swap(multiset<K,C,A>& x) {
    K* td = _data; _data = x._data; x._data = td;
    size_type ts = _size; _size = x._size; x._size = ts;
    size_type tc = _cap;  _cap  = x._cap;  x._cap  = tc;
    C tc2 = _comp; _comp = x._comp; x._comp = tc2;
}
template<class K, class C, class A>
inline void multiset<K,C,A>::clear() { for (size_type i = 0; i < _size; ++i) _data[i].~K(); _size = 0; }
template<class K, class C, class A>
inline typename multiset<K,C,A>::key_compare multiset<K,C,A>::key_comp() const { return _comp; }
template<class K, class C, class A>
inline typename multiset<K,C,A>::value_compare multiset<K,C,A>::value_comp() const { return _comp; }
template<class K, class C, class A>
inline typename multiset<K,C,A>::iterator multiset<K,C,A>::find(const K& x) const {
    size_type idx = _lower_idx(x);
    return (idx < _size && !_comp(x, _data[idx])) ? const_cast<K*>(_data + idx) : const_cast<K*>(_data + _size);
}
template<class K, class C, class A>
inline typename multiset<K,C,A>::size_type multiset<K,C,A>::count(const K& x) const {
    size_type lo = _lower_idx(x), hi = lo;
    while (hi < _size && !_comp(x, _data[hi])) ++hi;
    return hi - lo;
}
template<class K, class C, class A>
inline typename multiset<K,C,A>::iterator multiset<K,C,A>::lower_bound(const K& x) const { return const_cast<K*>(_data + _lower_idx(x)); }
template<class K, class C, class A>
inline typename multiset<K,C,A>::iterator multiset<K,C,A>::upper_bound(const K& x) const {
    size_type idx = _lower_idx(x);
    while (idx < _size && !_comp(x, _data[idx])) ++idx;
    return const_cast<K*>(_data + idx);
}
template<class K, class C, class A>
inline pair<typename multiset<K,C,A>::iterator, typename multiset<K,C,A>::iterator>
multiset<K,C,A>::equal_range(const K& x) const { return pair<iterator,iterator>(lower_bound(x), upper_bound(x)); }

template<class K, class C, class A>
inline bool operator==(const multiset<K,C,A>& x, const multiset<K,C,A>& y) {
    if (x.size() != y.size()) return false;
    typename multiset<K,C,A>::const_iterator xi = x.begin(), yi = y.begin();
    for (; xi != x.end(); ++xi, ++yi) if (!(*xi == *yi)) return false;
    return true;
}
template<class K, class C, class A>
inline bool operator<(const multiset<K,C,A>& x, const multiset<K,C,A>& y) {
    typename multiset<K,C,A>::const_iterator xi = x.begin(), yi = y.begin();
    for (; xi != x.end() && yi != y.end(); ++xi, ++yi) {
        if (*xi < *yi) return true; if (*yi < *xi) return false;
    }
    return xi == x.end() && yi != y.end();
}
template<class K, class C, class A>
inline void swap(multiset<K,C,A>& x, multiset<K,C,A>& y) { x.swap(y); }

} // namespace std

#endif