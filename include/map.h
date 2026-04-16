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
    typedef implementation defined iterator;
    typedef implementation defined const_iterator;
    typedef implementation defined size_type;
    typedef implementation defined difference_type;
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
    typedef implementation defined iterator;
    typedef implementation defined const_iterator;
    typedef implementation defined size_type;
    typedef implementation defined difference_type;
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

} // namespace std

#endif