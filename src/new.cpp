#include <new.h>

#include <cstdlib.h>

namespace std {

const nothrow_t nothrow = nothrow_t();

static new_handler __maiacpp_new_handler = 0;

new_handler set_new_handler(new_handler new_p) throw() {
    new_handler old = __maiacpp_new_handler;
    __maiacpp_new_handler = new_p;
    return old;
}

bad_alloc::bad_alloc() throw() {}
bad_alloc::bad_alloc(const bad_alloc&) throw() {}
bad_alloc& bad_alloc::operator=(const bad_alloc&) throw() { return *this; }
bad_alloc::~bad_alloc() throw() {}
const char* bad_alloc::what() const throw() { return "std::bad_alloc"; }

} // namespace std

void* operator new(std::size_t size) throw(std::bad_alloc) {
    if (size == 0) {
        size = 1;
    }

    void* ptr = std::malloc(size);
    while (ptr == 0) {
        std::new_handler h = std::set_new_handler(0);
        std::set_new_handler(h);
        if (h == 0) {
            throw std::bad_alloc();
        }
        h();
        ptr = std::malloc(size);
    }
    return ptr;
}

void* operator new(std::size_t size, const std::nothrow_t&) throw() {
    try {
        return ::operator new(size);
    } catch (...) {
        return 0;
    }
}

void operator delete(void* ptr) throw() {
    std::free(ptr);
}

void operator delete(void* ptr, const std::nothrow_t&) throw() {
    std::free(ptr);
}

void* operator new[](std::size_t size) throw(std::bad_alloc) {
    return ::operator new(size);
}

void* operator new[](std::size_t size, const std::nothrow_t&) throw() {
    return ::operator new(size, std::nothrow);
}

void operator delete[](void* ptr) throw() {
    ::operator delete(ptr);
}

void operator delete[](void* ptr, const std::nothrow_t&) throw() {
    ::operator delete(ptr);
}

void* operator new(std::size_t, void* ptr) throw() {
    return ptr;
}

void* operator new[](std::size_t, void* ptr) throw() {
    return ptr;
}

void operator delete(void*, void*) throw() {}

void operator delete[](void*, void*) throw() {}