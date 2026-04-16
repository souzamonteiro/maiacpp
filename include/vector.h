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