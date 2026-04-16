// ISO/IEC 14882:1998(E) - 18.4 Dynamic memory management

#ifndef _NEW_
#define _NEW_

#include <cstddef>
#include <exception>

namespace std {

class bad_alloc : public exception {
public:
    bad_alloc() throw();
    bad_alloc(const bad_alloc&) throw();
    bad_alloc& operator=(const bad_alloc&) throw();
    virtual ~bad_alloc() throw();
    virtual const char* what() const throw();
};

struct nothrow_t {};
extern const nothrow_t nothrow;

typedef void (*new_handler)();
new_handler set_new_handler(new_handler new_p) throw();

} // namespace std

void* operator new(std::size_t size) throw(std::bad_alloc);
void* operator new(std::size_t size, const std::nothrow_t&) throw();
void operator delete(void* ptr) throw();
void operator delete(void* ptr, const std::nothrow_t&) throw();

void* operator new[](std::size_t size) throw(std::bad_alloc);
void* operator new[](std::size_t size, const std::nothrow_t&) throw();
void operator delete[](void* ptr) throw();
void operator delete[](void* ptr, const std::nothrow_t&) throw();

void* operator new(std::size_t size, void* ptr) throw();
void* operator new[](std::size_t size, void* ptr) throw();
void operator delete(void* ptr, void*) throw();
void operator delete[](void* ptr, void*) throw();

#endif