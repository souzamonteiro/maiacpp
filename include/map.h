// ISO/IEC 14882:1998(E) - 23.3.1 Template class map, 23.3.2 Template class multimap

#ifndef _MAP_
#define _MAP_

#include <memory>
#include <functional>
#include <iterator>

namespace std {

template <class Key, class T, class Compare = less<Key>,
          class Allocator = allocator<pair<const Key, T> > >
class map {
public:
    typedef Key key_type;
    typedef T mapped_type;
    typedef pair<const Key, T> value_type;
    typedef Compare key_compare;
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

    class value_compare : public binary_function<value_type, value_type, bool> {
        friend class map;
    protected:
        Compare comp;
        value_compare(Compare c) : comp(c) {}
    public:
        bool operator()(const value_type& x, const value_type& y) const {
            return comp(x.first, y.first);
        }
    };

    explicit map(const Compare& comp = Compare(), const Allocator& = Allocator());
    template <class InputIterator>
    map(InputIterator first, InputIterator last,
        const Compare& comp = Compare(), const Allocator& = Allocator());
    map(const map<Key,T,Compare,Allocator>& x);
    ~map();
    map<Key,T,Compare,Allocator>& operator=(const map<Key,T,Compare,Allocator>& x);

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

    T& operator[](const key_type& x);

    pair<iterator, bool> insert(const value_type& x);
    iterator insert(iterator position, const value_type& x);
    template <class InputIterator>
    void insert(InputIterator first, InputIterator last);
    void erase(iterator position);
    size_type erase(const key_type& x);
    void erase(iterator first, iterator last);
    void swap(map<Key,T,Compare,Allocator>&);
    void clear();

    key_compare key_comp() const;
    value_compare value_comp() const;

    iterator find(const key_type& x);
    const_iterator find(const key_type& x) const;
    size_type count(const key_type& x) const;
    iterator lower_bound(const key_type& x);
    const_iterator lower_bound(const key_type& x) const;
    iterator upper_bound(const key_type& x);
    const_iterator upper_bound(const key_type& x) const;
    pair<iterator,iterator> equal_range(const key_type& x);
    pair<const_iterator,const_iterator> equal_range(const key_type& x) const;
private:
    typedef pair<Key, T> _Pair;
    _Pair*    _data;
    size_type _size;
    size_type _cap;
    Compare   _comp;
    Allocator _alloc;
    void _reserve(size_type n);
    size_type _lower_idx(const Key& k) const;
    iterator       _cast(size_type i)       { return reinterpret_cast<iterator>(_data + i); }
    const_iterator _cast(size_type i) const { return reinterpret_cast<const_iterator>(_data + i); }
};

template <class Key, class T, class Compare, class Allocator>
bool operator==(const map<Key,T,Compare,Allocator>& x,
                const map<Key,T,Compare,Allocator>& y);

template <class Key, class T, class Compare, class Allocator>
bool operator<(const map<Key,T,Compare,Allocator>& x,
               const map<Key,T,Compare,Allocator>& y);

template <class Key, class T, class Compare, class Allocator>
bool operator!=(const map<Key,T,Compare,Allocator>& x,
                const map<Key,T,Compare,Allocator>& y);

template <class Key, class T, class Compare, class Allocator>
bool operator>(const map<Key,T,Compare,Allocator>& x,
               const map<Key,T,Compare,Allocator>& y);

template <class Key, class T, class Compare, class Allocator>
bool operator>=(const map<Key,T,Compare,Allocator>& x,
                const map<Key,T,Compare,Allocator>& y);

template <class Key, class T, class Compare, class Allocator>
bool operator<=(const map<Key,T,Compare,Allocator>& x,
                const map<Key,T,Compare,Allocator>& y);

template <class Key, class T, class Compare, class Allocator>
void swap(map<Key,T,Compare,Allocator>& x,
          map<Key,T,Compare,Allocator>& y);

template <class Key, class T, class Compare = less<Key>,
          class Allocator = allocator<pair<const Key, T> > >
class multimap {
public:
    // types (same as map)
    typedef Key key_type;
    typedef T mapped_type;
    typedef pair<const Key, T> value_type;
    typedef Compare key_compare;
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

    class value_compare : public binary_function<value_type, value_type, bool> {
        friend class multimap;
    protected:
        Compare comp;
        value_compare(Compare c) : comp(c) {}
    public:
        bool operator()(const value_type& x, const value_type& y) const {
            return comp(x.first, y.first);
        }
    };

    explicit multimap(const Compare& comp = Compare(), const Allocator& = Allocator());
    template <class InputIterator>
    multimap(InputIterator first, InputIterator last,
             const Compare& comp = Compare(), const Allocator& = Allocator());
    multimap(const multimap<Key,T,Compare,Allocator>& x);
    ~multimap();
    multimap<Key,T,Compare,Allocator>& operator=(const multimap<Key,T,Compare,Allocator>& x);

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
    void swap(multimap<Key,T,Compare,Allocator>&);
    void clear();

    key_compare key_comp() const;
    value_compare value_comp() const;

    iterator find(const key_type& x);
    const_iterator find(const key_type& x) const;
    size_type count(const key_type& x) const;
    iterator lower_bound(const key_type& x);
    const_iterator lower_bound(const key_type& x) const;
    iterator upper_bound(const key_type& x);
    const_iterator upper_bound(const key_type& x) const;
    pair<iterator,iterator> equal_range(const key_type& x);
    pair<const_iterator,const_iterator> equal_range(const key_type& x) const;
private:
    typedef pair<Key, T> _Pair;
    _Pair*    _data;
    size_type _size;
    size_type _cap;
    Compare   _comp;
    Allocator _alloc;
    void _reserve(size_type n);
    size_type _lower_idx(const Key& k) const;
    iterator       _cast(size_type i)       { return reinterpret_cast<iterator>(_data + i); }
    const_iterator _cast(size_type i) const { return reinterpret_cast<const_iterator>(_data + i); }
};

template <class Key, class T, class Compare, class Allocator>
bool operator==(const multimap<Key,T,Compare,Allocator>& x,
                const multimap<Key,T,Compare,Allocator>& y);

template <class Key, class T, class Compare, class Allocator>
bool operator<(const multimap<Key,T,Compare,Allocator>& x,
               const multimap<Key,T,Compare,Allocator>& y);

// Additional comparison operators similarly defined

template <class Key, class T, class Compare, class Allocator>
void swap(multimap<Key,T,Compare,Allocator>& x,
          multimap<Key,T,Compare,Allocator>& y);

// ---- map<Key,T,Compare,Allocator> inline implementations ----
template<class K, class V, class C, class A>
inline void map<K,V,C,A>::_reserve(size_type n) {
    if (n <= _cap) return;
    _Pair* nd = reinterpret_cast<_Pair*>(::operator new(n * sizeof(_Pair)));
    for (size_type i = 0; i < _size; ++i) {
        new (nd + i) _Pair(_data[i]);
        _data[i].~_Pair();
    }
    if (_data) ::operator delete(_data);
    _data = nd; _cap = n;
}
template<class K, class V, class C, class A>
inline typename map<K,V,C,A>::size_type map<K,V,C,A>::_lower_idx(const K& k) const {
    size_type lo = 0, hi = _size;
    while (lo < hi) {
        size_type mid = lo + (hi - lo) / 2;
        if (_comp(_data[mid].first, k)) lo = mid + 1; else hi = mid;
    }
    return lo;
}

template<class K, class V, class C, class A>
inline map<K,V,C,A>::map(const C& c, const A& a)
    : _data(0), _size(0), _cap(0), _comp(c), _alloc(a) {}
template<class K, class V, class C, class A>
template<class InputIterator>
inline map<K,V,C,A>::map(InputIterator first, InputIterator last, const C& c, const A& a)
    : _data(0), _size(0), _cap(0), _comp(c), _alloc(a) {
    for (; first != last; ++first) insert(*first);
}
template<class K, class V, class C, class A>
inline map<K,V,C,A>::map(const map<K,V,C,A>& x)
    : _data(0), _size(0), _cap(0), _comp(x._comp), _alloc(x._alloc) {
    for (size_type i = 0; i < x._size; ++i) {
        value_type vt(x._data[i].first, x._data[i].second);
        insert(vt);
    }
}
template<class K, class V, class C, class A>
inline map<K,V,C,A>::~map() {
    for (size_type i = 0; i < _size; ++i) _data[i].~_Pair();
    if (_data) ::operator delete(_data);
}
template<class K, class V, class C, class A>
inline map<K,V,C,A>& map<K,V,C,A>::operator=(const map<K,V,C,A>& x) {
    if (this != &x) { map tmp(x); swap(tmp); }
    return *this;
}

template<class K, class V, class C, class A>
inline typename map<K,V,C,A>::iterator map<K,V,C,A>::begin() { return _cast(0); }
template<class K, class V, class C, class A>
inline typename map<K,V,C,A>::const_iterator map<K,V,C,A>::begin() const { return _cast(0); }
template<class K, class V, class C, class A>
inline typename map<K,V,C,A>::iterator map<K,V,C,A>::end() { return _cast(_size); }
template<class K, class V, class C, class A>
inline typename map<K,V,C,A>::const_iterator map<K,V,C,A>::end() const { return _cast(_size); }
template<class K, class V, class C, class A>
inline typename map<K,V,C,A>::reverse_iterator map<K,V,C,A>::rbegin() { return reverse_iterator(end()); }
template<class K, class V, class C, class A>
inline typename map<K,V,C,A>::const_reverse_iterator map<K,V,C,A>::rbegin() const { return const_reverse_iterator(end()); }
template<class K, class V, class C, class A>
inline typename map<K,V,C,A>::reverse_iterator map<K,V,C,A>::rend() { return reverse_iterator(begin()); }
template<class K, class V, class C, class A>
inline typename map<K,V,C,A>::const_reverse_iterator map<K,V,C,A>::rend() const { return const_reverse_iterator(begin()); }

template<class K, class V, class C, class A>
inline bool map<K,V,C,A>::empty() const { return _size == 0; }
template<class K, class V, class C, class A>
inline typename map<K,V,C,A>::size_type map<K,V,C,A>::size() const { return _size; }
template<class K, class V, class C, class A>
inline typename map<K,V,C,A>::size_type map<K,V,C,A>::max_size() const { return size_type(-1) / sizeof(_Pair); }

template<class K, class V, class C, class A>
inline V& map<K,V,C,A>::operator[](const K& x) {
    size_type idx = _lower_idx(x);
    if (idx < _size && !_comp(x, _data[idx].first)) return _data[idx].second;
    // Insert default-constructed value
    _reserve(_cap + _cap / 2 + 2);
    for (size_type i = _size; i > idx; --i) {
        new (_data + i) _Pair(_data[i - 1]); _data[i - 1].~_Pair();
    }
    new (_data + idx) _Pair(x, V());
    ++_size;
    return _data[idx].second;
}

template<class K, class V, class C, class A>
inline pair<typename map<K,V,C,A>::iterator, bool> map<K,V,C,A>::insert(const value_type& x) {
    size_type idx = _lower_idx(x.first);
    if (idx < _size && !_comp(x.first, _data[idx].first))
        return pair<iterator,bool>(_cast(idx), false);
    _reserve(_cap + _cap / 2 + 2);
    for (size_type i = _size; i > idx; --i) {
        new (_data + i) _Pair(_data[i - 1]); _data[i - 1].~_Pair();
    }
    new (_data + idx) _Pair(x.first, x.second);
    ++_size;
    return pair<iterator,bool>(_cast(idx), true);
}
template<class K, class V, class C, class A>
inline typename map<K,V,C,A>::iterator map<K,V,C,A>::insert(iterator, const value_type& x) {
    return insert(x).first;
}
template<class K, class V, class C, class A>
template<class InputIterator>
inline void map<K,V,C,A>::insert(InputIterator first, InputIterator last) {
    for (; first != last; ++first) insert(*first);
}

template<class K, class V, class C, class A>
inline void map<K,V,C,A>::erase(iterator pos) {
    size_type idx = size_type(reinterpret_cast<_Pair*>(pos) - _data);
    _data[idx].~_Pair();
    for (size_type i = idx; i + 1 < _size; ++i) {
        new (_data + i) _Pair(_data[i + 1]); _data[i + 1].~_Pair();
    }
    --_size;
}
template<class K, class V, class C, class A>
inline typename map<K,V,C,A>::size_type map<K,V,C,A>::erase(const K& x) {
    size_type idx = _lower_idx(x);
    if (idx >= _size || _comp(x, _data[idx].first)) return 0;
    erase(_cast(idx)); return 1;
}
template<class K, class V, class C, class A>
inline void map<K,V,C,A>::erase(iterator first, iterator last) {
    while (first != last) { iterator nx = first; ++nx; erase(first); first = nx; }
}
template<class K, class V, class C, class A>
inline void map<K,V,C,A>::swap(map<K,V,C,A>& x) {
    _Pair* td = _data; _data = x._data; x._data = td;
    size_type ts = _size; _size = x._size; x._size = ts;
    size_type tc = _cap;  _cap  = x._cap;  x._cap  = tc;
    C tc2 = _comp; _comp = x._comp; x._comp = tc2;
}
template<class K, class V, class C, class A>
inline void map<K,V,C,A>::clear() {
    for (size_type i = 0; i < _size; ++i) _data[i].~_Pair();
    _size = 0;
}

template<class K, class V, class C, class A>
inline typename map<K,V,C,A>::key_compare map<K,V,C,A>::key_comp() const { return _comp; }
template<class K, class V, class C, class A>
inline typename map<K,V,C,A>::value_compare map<K,V,C,A>::value_comp() const { return value_compare(_comp); }

template<class K, class V, class C, class A>
inline typename map<K,V,C,A>::iterator map<K,V,C,A>::find(const K& x) {
    size_type idx = _lower_idx(x);
    if (idx < _size && !_comp(x, _data[idx].first)) return _cast(idx);
    return end();
}
template<class K, class V, class C, class A>
inline typename map<K,V,C,A>::const_iterator map<K,V,C,A>::find(const K& x) const {
    size_type idx = _lower_idx(x);
    if (idx < _size && !_comp(x, _data[idx].first)) return _cast(idx);
    return end();
}
template<class K, class V, class C, class A>
inline typename map<K,V,C,A>::size_type map<K,V,C,A>::count(const K& x) const {
    return find(x) != end() ? 1 : 0;
}
template<class K, class V, class C, class A>
inline typename map<K,V,C,A>::iterator map<K,V,C,A>::lower_bound(const K& x) {
    return _cast(_lower_idx(x));
}
template<class K, class V, class C, class A>
inline typename map<K,V,C,A>::const_iterator map<K,V,C,A>::lower_bound(const K& x) const {
    return _cast(_lower_idx(x));
}
template<class K, class V, class C, class A>
inline typename map<K,V,C,A>::iterator map<K,V,C,A>::upper_bound(const K& x) {
    size_type idx = _lower_idx(x);
    if (idx < _size && !_comp(x, _data[idx].first)) ++idx;
    return _cast(idx);
}
template<class K, class V, class C, class A>
inline typename map<K,V,C,A>::const_iterator map<K,V,C,A>::upper_bound(const K& x) const {
    size_type idx = _lower_idx(x);
    if (idx < _size && !_comp(x, _data[idx].first)) ++idx;
    return _cast(idx);
}
template<class K, class V, class C, class A>
inline pair<typename map<K,V,C,A>::iterator,typename map<K,V,C,A>::iterator>
map<K,V,C,A>::equal_range(const K& x) {
    return pair<iterator,iterator>(lower_bound(x), upper_bound(x));
}
template<class K, class V, class C, class A>
inline pair<typename map<K,V,C,A>::const_iterator,typename map<K,V,C,A>::const_iterator>
map<K,V,C,A>::equal_range(const K& x) const {
    return pair<const_iterator,const_iterator>(lower_bound(x), upper_bound(x));
}

// ---- non-member operators (map) ----
template<class K, class V, class C, class A>
inline bool operator==(const map<K,V,C,A>& x, const map<K,V,C,A>& y) {
    if (x.size() != y.size()) return false;
    typename map<K,V,C,A>::const_iterator xi = x.begin(), yi = y.begin();
    for (; xi != x.end(); ++xi, ++yi)
        if (!(xi->first == yi->first && xi->second == yi->second)) return false;
    return true;
}
template<class K, class V, class C, class A>
inline bool operator<(const map<K,V,C,A>& x, const map<K,V,C,A>& y) {
    typename map<K,V,C,A>::const_iterator xi = x.begin(), yi = y.begin();
    for (; xi != x.end() && yi != y.end(); ++xi, ++yi) {
        if (xi->first < yi->first) return true;
        if (yi->first < xi->first) return false;
    }
    return xi == x.end() && yi != y.end();
}
template<class K, class V, class C, class A>
inline bool operator!=(const map<K,V,C,A>& x, const map<K,V,C,A>& y) { return !(x == y); }
template<class K, class V, class C, class A>
inline bool operator>(const map<K,V,C,A>& x, const map<K,V,C,A>& y)  { return y < x; }
template<class K, class V, class C, class A>
inline bool operator>=(const map<K,V,C,A>& x, const map<K,V,C,A>& y) { return !(x < y); }
template<class K, class V, class C, class A>
inline bool operator<=(const map<K,V,C,A>& x, const map<K,V,C,A>& y) { return !(y < x); }
template<class K, class V, class C, class A>
inline void swap(map<K,V,C,A>& x, map<K,V,C,A>& y) { x.swap(y); }

// ---- multimap<Key,T,Compare,Allocator> inline implementations ----
template<class K, class V, class C, class A>
inline void multimap<K,V,C,A>::_reserve(size_type n) {
    if (n <= _cap) return;
    _Pair* nd = reinterpret_cast<_Pair*>(::operator new(n * sizeof(_Pair)));
    for (size_type i = 0; i < _size; ++i) { new (nd + i) _Pair(_data[i]); _data[i].~_Pair(); }
    if (_data) ::operator delete(_data);
    _data = nd; _cap = n;
}
template<class K, class V, class C, class A>
inline typename multimap<K,V,C,A>::size_type multimap<K,V,C,A>::_lower_idx(const K& k) const {
    size_type lo = 0, hi = _size;
    while (lo < hi) { size_type mid = lo + (hi - lo) / 2; if (_comp(_data[mid].first, k)) lo = mid + 1; else hi = mid; }
    return lo;
}
template<class K, class V, class C, class A>
inline multimap<K,V,C,A>::multimap(const C& c, const A& a) : _data(0), _size(0), _cap(0), _comp(c), _alloc(a) {}
template<class K, class V, class C, class A>
template<class InputIterator>
inline multimap<K,V,C,A>::multimap(InputIterator first, InputIterator last, const C& c, const A& a)
    : _data(0), _size(0), _cap(0), _comp(c), _alloc(a) { for (; first != last; ++first) insert(*first); }
template<class K, class V, class C, class A>
inline multimap<K,V,C,A>::multimap(const multimap<K,V,C,A>& x)
    : _data(0), _size(0), _cap(0), _comp(x._comp), _alloc(x._alloc) {
    for (size_type i = 0; i < x._size; ++i) { value_type vt(x._data[i].first, x._data[i].second); insert(vt); }
}
template<class K, class V, class C, class A>
inline multimap<K,V,C,A>::~multimap() {
    for (size_type i = 0; i < _size; ++i) _data[i].~_Pair();
    if (_data) ::operator delete(_data);
}
template<class K, class V, class C, class A>
inline multimap<K,V,C,A>& multimap<K,V,C,A>::operator=(const multimap<K,V,C,A>& x) {
    if (this != &x) { multimap tmp(x); swap(tmp); } return *this;
}
template<class K, class V, class C, class A>
inline typename multimap<K,V,C,A>::iterator multimap<K,V,C,A>::begin() { return _cast(0); }
template<class K, class V, class C, class A>
inline typename multimap<K,V,C,A>::const_iterator multimap<K,V,C,A>::begin() const { return _cast(0); }
template<class K, class V, class C, class A>
inline typename multimap<K,V,C,A>::iterator multimap<K,V,C,A>::end() { return _cast(_size); }
template<class K, class V, class C, class A>
inline typename multimap<K,V,C,A>::const_iterator multimap<K,V,C,A>::end() const { return _cast(_size); }
template<class K, class V, class C, class A>
inline typename multimap<K,V,C,A>::reverse_iterator multimap<K,V,C,A>::rbegin() { return reverse_iterator(end()); }
template<class K, class V, class C, class A>
inline typename multimap<K,V,C,A>::const_reverse_iterator multimap<K,V,C,A>::rbegin() const { return const_reverse_iterator(end()); }
template<class K, class V, class C, class A>
inline typename multimap<K,V,C,A>::reverse_iterator multimap<K,V,C,A>::rend() { return reverse_iterator(begin()); }
template<class K, class V, class C, class A>
inline typename multimap<K,V,C,A>::const_reverse_iterator multimap<K,V,C,A>::rend() const { return const_reverse_iterator(begin()); }
template<class K, class V, class C, class A>
inline bool multimap<K,V,C,A>::empty() const { return _size == 0; }
template<class K, class V, class C, class A>
inline typename multimap<K,V,C,A>::size_type multimap<K,V,C,A>::size() const { return _size; }
template<class K, class V, class C, class A>
inline typename multimap<K,V,C,A>::size_type multimap<K,V,C,A>::max_size() const { return size_type(-1) / sizeof(_Pair); }
template<class K, class V, class C, class A>
inline typename multimap<K,V,C,A>::iterator multimap<K,V,C,A>::insert(const value_type& x) {
    size_type idx = _lower_idx(x.first);
    // advance past equal keys (multimap allows duplicates; insert at end of equal range)
    while (idx < _size && !_comp(x.first, _data[idx].first)) ++idx;
    _reserve(_cap + _cap / 2 + 2);
    for (size_type i = _size; i > idx; --i) { new (_data + i) _Pair(_data[i-1]); _data[i-1].~_Pair(); }
    new (_data + idx) _Pair(x.first, x.second);
    ++_size;
    return _cast(idx);
}
template<class K, class V, class C, class A>
inline typename multimap<K,V,C,A>::iterator multimap<K,V,C,A>::insert(iterator, const value_type& x) { return insert(x); }
template<class K, class V, class C, class A>
template<class InputIterator>
inline void multimap<K,V,C,A>::insert(InputIterator first, InputIterator last) { for (; first != last; ++first) insert(*first); }
template<class K, class V, class C, class A>
inline void multimap<K,V,C,A>::erase(iterator pos) {
    size_type idx = size_type(reinterpret_cast<_Pair*>(pos) - _data);
    _data[idx].~_Pair();
    for (size_type i = idx; i + 1 < _size; ++i) { new (_data+i) _Pair(_data[i+1]); _data[i+1].~_Pair(); }
    --_size;
}
template<class K, class V, class C, class A>
inline typename multimap<K,V,C,A>::size_type multimap<K,V,C,A>::erase(const K& x) {
    size_type cnt = 0;
    size_type lo = _lower_idx(x);
    while (lo < _size && !_comp(x, _data[lo].first)) { erase(_cast(lo)); ++cnt; }
    return cnt;
}
template<class K, class V, class C, class A>
inline void multimap<K,V,C,A>::erase(iterator first, iterator last) {
    while (first != last) { iterator nx = first; ++nx; erase(first); first = nx; }
}
template<class K, class V, class C, class A>
inline void multimap<K,V,C,A>::swap(multimap<K,V,C,A>& x) {
    _Pair* td = _data; _data = x._data; x._data = td;
    size_type ts = _size; _size = x._size; x._size = ts;
    size_type tc = _cap;  _cap  = x._cap;  x._cap  = tc;
    C tc2 = _comp; _comp = x._comp; x._comp = tc2;
}
template<class K, class V, class C, class A>
inline void multimap<K,V,C,A>::clear() { for (size_type i = 0; i < _size; ++i) _data[i].~_Pair(); _size = 0; }
template<class K, class V, class C, class A>
inline typename multimap<K,V,C,A>::key_compare multimap<K,V,C,A>::key_comp() const { return _comp; }
template<class K, class V, class C, class A>
inline typename multimap<K,V,C,A>::value_compare multimap<K,V,C,A>::value_comp() const { return value_compare(_comp); }
template<class K, class V, class C, class A>
inline typename multimap<K,V,C,A>::iterator multimap<K,V,C,A>::find(const K& x) {
    size_type idx = _lower_idx(x);
    if (idx < _size && !_comp(x, _data[idx].first)) return _cast(idx);
    return end();
}
template<class K, class V, class C, class A>
inline typename multimap<K,V,C,A>::const_iterator multimap<K,V,C,A>::find(const K& x) const {
    size_type idx = _lower_idx(x);
    if (idx < _size && !_comp(x, _data[idx].first)) return _cast(idx);
    return end();
}
template<class K, class V, class C, class A>
inline typename multimap<K,V,C,A>::size_type multimap<K,V,C,A>::count(const K& x) const {
    size_type lo = _lower_idx(x), hi = lo;
    while (hi < _size && !_comp(x, _data[hi].first)) ++hi;
    return hi - lo;
}
template<class K, class V, class C, class A>
inline typename multimap<K,V,C,A>::iterator multimap<K,V,C,A>::lower_bound(const K& x) { return _cast(_lower_idx(x)); }
template<class K, class V, class C, class A>
inline typename multimap<K,V,C,A>::const_iterator multimap<K,V,C,A>::lower_bound(const K& x) const { return _cast(_lower_idx(x)); }
template<class K, class V, class C, class A>
inline typename multimap<K,V,C,A>::iterator multimap<K,V,C,A>::upper_bound(const K& x) {
    size_type idx = _lower_idx(x);
    while (idx < _size && !_comp(x, _data[idx].first)) ++idx;
    return _cast(idx);
}
template<class K, class V, class C, class A>
inline typename multimap<K,V,C,A>::const_iterator multimap<K,V,C,A>::upper_bound(const K& x) const {
    size_type idx = _lower_idx(x);
    while (idx < _size && !_comp(x, _data[idx].first)) ++idx;
    return _cast(idx);
}
template<class K, class V, class C, class A>
inline pair<typename multimap<K,V,C,A>::iterator,typename multimap<K,V,C,A>::iterator>
multimap<K,V,C,A>::equal_range(const K& x) { return pair<iterator,iterator>(lower_bound(x), upper_bound(x)); }
template<class K, class V, class C, class A>
inline pair<typename multimap<K,V,C,A>::const_iterator,typename multimap<K,V,C,A>::const_iterator>
multimap<K,V,C,A>::equal_range(const K& x) const { return pair<const_iterator,const_iterator>(lower_bound(x), upper_bound(x)); }

template<class K, class V, class C, class A>
inline bool operator==(const multimap<K,V,C,A>& x, const multimap<K,V,C,A>& y) {
    if (x.size() != y.size()) return false;
    typename multimap<K,V,C,A>::const_iterator xi = x.begin(), yi = y.begin();
    for (; xi != x.end(); ++xi, ++yi)
        if (!(xi->first == yi->first && xi->second == yi->second)) return false;
    return true;
}
template<class K, class V, class C, class A>
inline bool operator<(const multimap<K,V,C,A>& x, const multimap<K,V,C,A>& y) {
    typename multimap<K,V,C,A>::const_iterator xi = x.begin(), yi = y.begin();
    for (; xi != x.end() && yi != y.end(); ++xi, ++yi) {
        if (xi->first < yi->first) return true; if (yi->first < xi->first) return false;
    }
    return xi == x.end() && yi != y.end();
}
template<class K, class V, class C, class A>
inline void swap(multimap<K,V,C,A>& x, multimap<K,V,C,A>& y) { x.swap(y); }

} // namespace std

#endif