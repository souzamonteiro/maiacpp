// ISO/IEC 14882:1998(E) - 24 Iterators library

#ifndef _ITERATOR_
#define _ITERATOR_

#include <cstddef>

namespace std {

template<class Iterator> struct iterator_traits {
    typedef typename Iterator::difference_type difference_type;
    typedef typename Iterator::value_type value_type;
    typedef typename Iterator::pointer pointer;
    typedef typename Iterator::reference reference;
    typedef typename Iterator::iterator_category iterator_category;
};

template<class T> struct iterator_traits<T*> {
    typedef ptrdiff_t difference_type;
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef random_access_iterator_tag iterator_category;
};

template<class T> struct iterator_traits<const T*> {
    typedef ptrdiff_t difference_type;
    typedef T value_type;
    typedef const T* pointer;
    typedef const T& reference;
    typedef random_access_iterator_tag iterator_category;
};

template<class Category, class T, class Distance = ptrdiff_t,
         class Pointer = T*, class Reference = T&>
struct iterator {
    typedef T value_type;
    typedef Distance difference_type;
    typedef Pointer pointer;
    typedef Reference reference;
    typedef Category iterator_category;
};

struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

template <class InputIterator, class Distance>
void advance(InputIterator& i, Distance n);

template <class InputIterator>
typename iterator_traits<InputIterator>::difference_type
distance(InputIterator first, InputIterator last);

template <class Iterator>
class reverse_iterator {
public:
    typedef Iterator                                             iterator_type;
    typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
    typedef typename iterator_traits<Iterator>::value_type        value_type;
    typedef typename iterator_traits<Iterator>::difference_type   difference_type;
    typedef typename iterator_traits<Iterator>::pointer          pointer;
    typedef typename iterator_traits<Iterator>::reference        reference;

    reverse_iterator() : current() {}
    explicit reverse_iterator(iterator_type x) : current(x) {}
    template <class U>
    reverse_iterator(const reverse_iterator<U>& x) : current(x.base()) {}

    iterator_type base() const { return current; }
    reference operator*() const { Iterator tmp = current; --tmp; return *tmp; }
    pointer operator->() const { return &(operator*()); }

    reverse_iterator& operator++() { --current; return *this; }
    reverse_iterator operator++(int) { reverse_iterator tmp(*this); --current; return tmp; }
    reverse_iterator& operator--() { ++current; return *this; }
    reverse_iterator operator--(int) { reverse_iterator tmp(*this); ++current; return tmp; }

    reverse_iterator operator+(difference_type n) const { return reverse_iterator(current - n); }
    reverse_iterator& operator+=(difference_type n) { current -= n; return *this; }
    reverse_iterator operator-(difference_type n) const { return reverse_iterator(current + n); }
    reverse_iterator& operator-=(difference_type n) { current += n; return *this; }
    reference operator[](difference_type n) const { return *(*this + n); }

private:
    Iterator current;
};

template <class Iterator>
bool operator==(const reverse_iterator<Iterator>& x,
                const reverse_iterator<Iterator>& y);

template <class Iterator>
bool operator<(const reverse_iterator<Iterator>& x,
               const reverse_iterator<Iterator>& y);

template <class Iterator>
bool operator!=(const reverse_iterator<Iterator>& x,
                const reverse_iterator<Iterator>& y);

template <class Iterator>
bool operator>(const reverse_iterator<Iterator>& x,
               const reverse_iterator<Iterator>& y);

template <class Iterator>
bool operator>=(const reverse_iterator<Iterator>& x,
                const reverse_iterator<Iterator>& y);

template <class Iterator>
bool operator<=(const reverse_iterator<Iterator>& x,
                const reverse_iterator<Iterator>& y);

template <class Iterator>
typename reverse_iterator<Iterator>::difference_type operator-(
    const reverse_iterator<Iterator>& x,
    const reverse_iterator<Iterator>& y);

template <class Iterator>
reverse_iterator<Iterator> operator+(
    typename reverse_iterator<Iterator>::difference_type n,
    const reverse_iterator<Iterator>& x);

template <class Container> class back_insert_iterator;
template <class Container> back_insert_iterator<Container> back_inserter(Container& x);

template <class Container> class front_insert_iterator;
template <class Container> front_insert_iterator<Container> front_inserter(Container& x);

template <class Container> class insert_iterator;
template <class Container, class Iterator>
insert_iterator<Container> inserter(Container& x, Iterator i);

template <class Container>
class back_insert_iterator
    : public iterator<output_iterator_tag, void, void, void, void> {
protected:
    Container* container;
public:
    explicit back_insert_iterator(Container& x) : container(&x) {}
    back_insert_iterator<Container>& operator=(const typename Container::value_type& value) {
        container->push_back(value);
        return *this;
    }
    back_insert_iterator<Container>& operator*() { return *this; }
    back_insert_iterator<Container>& operator++() { return *this; }
    back_insert_iterator<Container> operator++(int) { return *this; }
};

template <class Container>
class front_insert_iterator
    : public iterator<output_iterator_tag, void, void, void, void> {
protected:
    Container* container;
public:
    explicit front_insert_iterator(Container& x) : container(&x) {}
    front_insert_iterator<Container>& operator=(const typename Container::value_type& value) {
        container->push_front(value);
        return *this;
    }
    front_insert_iterator<Container>& operator*() { return *this; }
    front_insert_iterator<Container>& operator++() { return *this; }
    front_insert_iterator<Container> operator++(int) { return *this; }
};

template <class Container>
class insert_iterator
    : public iterator<output_iterator_tag, void, void, void, void> {
protected:
    Container* container;
    typename Container::iterator iter;
public:
    template<class It>
    insert_iterator(Container& x, It i) : container(&x), iter(i) {}
    insert_iterator<Container>& operator=(const typename Container::value_type& value) {
        iter = container->insert(iter, value);
        ++iter;
        return *this;
    }
    insert_iterator<Container>& operator*() { return *this; }
    insert_iterator<Container>& operator++() { return *this; }
    insert_iterator<Container>& operator++(int) { return *this; }
};

template <class T, class charT = char, class traits = char_traits<charT>,
          class Distance = ptrdiff_t>
class istream_iterator;

template <class T, class charT, class traits, class Distance>
bool operator==(const istream_iterator<T,charT,traits,Distance>& x,
                const istream_iterator<T,charT,traits,Distance>& y);

template <class T, class charT, class traits, class Distance>
bool operator!=(const istream_iterator<T,charT,traits,Distance>& x,
                const istream_iterator<T,charT,traits,Distance>& y);

template <class T, class charT = char, class traits = char_traits<charT> >
class ostream_iterator;

template<class charT, class traits = char_traits<charT> >
class istreambuf_iterator;

template <class charT, class traits>
bool operator==(const istreambuf_iterator<charT,traits>& a,
                const istreambuf_iterator<charT,traits>& b);

template <class charT, class traits>
bool operator!=(const istreambuf_iterator<charT,traits>& a,
                const istreambuf_iterator<charT,traits>& b);

template <class charT, class traits = char_traits<charT> >
class ostreambuf_iterator;

// ---- iterator helpers ----
template<class InputIterator, class Distance>
inline void advance(InputIterator& i, Distance n) {
    while (n > 0) { ++i; --n; }
    while (n < 0) { --i; ++n; }
}

template<class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
distance(InputIterator first, InputIterator last) {
    typename iterator_traits<InputIterator>::difference_type n = 0;
    for (; first != last; ++first) ++n;
    return n;
}

template<class Iterator>
inline bool operator==(const reverse_iterator<Iterator>& x, const reverse_iterator<Iterator>& y) { return x.base() == y.base(); }
template<class Iterator>
inline bool operator<(const reverse_iterator<Iterator>& x, const reverse_iterator<Iterator>& y) { return y.base() < x.base(); }
template<class Iterator>
inline bool operator!=(const reverse_iterator<Iterator>& x, const reverse_iterator<Iterator>& y) { return !(x == y); }
template<class Iterator>
inline bool operator>(const reverse_iterator<Iterator>& x, const reverse_iterator<Iterator>& y) { return y < x; }
template<class Iterator>
inline bool operator>=(const reverse_iterator<Iterator>& x, const reverse_iterator<Iterator>& y) { return !(x < y); }
template<class Iterator>
inline bool operator<=(const reverse_iterator<Iterator>& x, const reverse_iterator<Iterator>& y) { return !(y < x); }
template<class Iterator>
inline typename reverse_iterator<Iterator>::difference_type operator-(const reverse_iterator<Iterator>& x, const reverse_iterator<Iterator>& y) { return y.base() - x.base(); }
template<class Iterator>
inline reverse_iterator<Iterator> operator+(typename reverse_iterator<Iterator>::difference_type n, const reverse_iterator<Iterator>& x) { return x + n; }

template<class Container>
inline back_insert_iterator<Container> back_inserter(Container& x) { return back_insert_iterator<Container>(x); }

template<class Container>
inline front_insert_iterator<Container> front_inserter(Container& x) { return front_insert_iterator<Container>(x); }

template<class Container, class Iterator>
inline insert_iterator<Container> inserter(Container& x, Iterator i) {
    return insert_iterator<Container>(x, i);
}

} // namespace std

#endif