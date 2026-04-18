// ISO/IEC 14882:1998(E) - 20.4 Memory

#ifndef _MEMORY_
#define _MEMORY_

#include <cstddef>
#include <iterator>
#include <new.h>
#include <utility>

namespace std {

template <class T> class allocator;
template <> class allocator<void>;

template <class T, class U>
bool operator==(const allocator<T>&, const allocator<U>&) throw();

template <class T, class U>
bool operator!=(const allocator<T>&, const allocator<U>&) throw();

template <class T> class allocator {
public:
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T value_type;

    template <class U> struct rebind { typedef allocator<U> other; };

    allocator() throw() {}
    allocator(const allocator&) throw() {}
    template <class U> allocator(const allocator<U>&) throw() {}
    ~allocator() throw() {}

    pointer address(reference x) const { return &x; }
    const_pointer address(const_reference x) const { return &x; }

    pointer allocate(size_type n, allocator<void>::const_pointer = 0) {
        pointer p = (pointer)::operator new(n * sizeof(T));
        if (!p) throw std::bad_alloc();
        return p;
    }
    void deallocate(pointer p, size_type) {
        ::operator delete(p);
    }
    size_type max_size() const throw() {
        return (size_type)(-1) / sizeof(T);
    }
    void construct(pointer p, const T& val) {
        ::new (static_cast<void*>(p)) T(val);
    }
    void destroy(pointer p) {
        p->~T();
    }
};

template <> class allocator<void> {
public:
    typedef void* pointer;
    typedef const void* const_pointer;
    typedef void value_type;
    template <class U> struct rebind { typedef allocator<U> other; };
};

template <class OutputIterator, class T>
class raw_storage_iterator : public iterator<output_iterator_tag, void, void, void, void> {
public:
    explicit raw_storage_iterator(OutputIterator x);
    raw_storage_iterator<OutputIterator,T>& operator*();
    raw_storage_iterator<OutputIterator,T>& operator=(const T& element);
    raw_storage_iterator<OutputIterator,T>& operator++();
    raw_storage_iterator<OutputIterator,T> operator++(int);
};

template <class T>
inline pair<T*, ptrdiff_t> get_temporary_buffer(ptrdiff_t n) {
    if (n <= 0) return pair<T*, ptrdiff_t>(static_cast<T*>(0), ptrdiff_t(0));
    T* p = static_cast<T*>(::operator new((size_t)n * sizeof(T), std::nothrow));
    if (!p) return pair<T*, ptrdiff_t>(static_cast<T*>(0), ptrdiff_t(0));
    return pair<T*, ptrdiff_t>(p, n);
}

template <class T>
inline void return_temporary_buffer(T* p) {
    ::operator delete(p);
}

template <class InputIterator, class ForwardIterator>
inline ForwardIterator uninitialized_copy(InputIterator first, InputIterator last,
                                          ForwardIterator result) {
    for (; first != last; ++first, ++result) {
        ::new (static_cast<void*>(&*result))
            typename iterator_traits<ForwardIterator>::value_type(*first);
    }
    return result;
}

template <class ForwardIterator, class T>
inline void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x) {
    for (; first != last; ++first) {
        ::new (static_cast<void*>(&*first))
            typename iterator_traits<ForwardIterator>::value_type(x);
    }
}

template <class ForwardIterator, class Size, class T>
inline void uninitialized_fill_n(ForwardIterator first, Size n, const T& x) {
    for (; n > 0; ++first, --n) {
        ::new (static_cast<void*>(&*first))
            typename iterator_traits<ForwardIterator>::value_type(x);
    }
}

template<class X> class auto_ptr {
    template <class Y> struct auto_ptr_ref {};

public:
    typedef X element_type;

    explicit auto_ptr(X* p = 0) throw();
    auto_ptr(auto_ptr&) throw();
    template<class Y> auto_ptr(auto_ptr<Y>&) throw();
    auto_ptr& operator=(auto_ptr&) throw();
    template<class Y> auto_ptr& operator=(auto_ptr<Y>&) throw();
    ~auto_ptr() throw();

    X& operator*() const throw();
    X* operator->() const throw();
    X* get() const throw();
    X* release() throw();
    void reset(X* p = 0) throw();

    auto_ptr(auto_ptr_ref<X>) throw();
    template<class Y> operator auto_ptr_ref<Y>() throw();
    template<class Y> operator auto_ptr<Y>() throw();
};

template <class T, class U>
bool operator==(const allocator<T>&, const allocator<U>&) throw() {
    return true;
}

template <class T, class U>
bool operator!=(const allocator<T>&, const allocator<U>&) throw() {
    return false;
}

template <class T>
allocator<T>::allocator() throw() {}

template <class T>
allocator<T>::allocator(const allocator&) throw() {}

template <class T>
template <class U>
allocator<T>::allocator(const allocator<U>&) throw() {}

template <class T>
allocator<T>::~allocator() throw() {}

template <class T>
typename allocator<T>::pointer allocator<T>::address(reference x) const {
    return &x;
}

template <class T>
typename allocator<T>::const_pointer allocator<T>::address(const_reference x) const {
    return &x;
}

template <class T>
typename allocator<T>::pointer allocator<T>::allocate(size_type n, allocator<void>::const_pointer) {
    if (n == 0) {
        return 0;
    }
    return static_cast<pointer>(::operator new(n * sizeof(T)));
}

template <class T>
void allocator<T>::deallocate(pointer p, size_type) {
    ::operator delete(p);
}

template <class T>
typename allocator<T>::size_type allocator<T>::max_size() const throw() {
    return static_cast<size_type>(-1) / sizeof(T);
}

template <class T>
void allocator<T>::construct(pointer p, const T& val) {
    ::new (static_cast<void*>(p)) T(val);
}

template <class T>
void allocator<T>::destroy(pointer p) {
    p->~T();
}

} // namespace std

#endif