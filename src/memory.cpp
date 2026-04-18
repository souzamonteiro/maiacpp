// MaiaCpp memory runtime – explicit instantiation anchor
//
// allocator<T> bodies are inline in include/memory.h so they work for all T.
// uninitialized_copy/fill/fill_n are likewise inline in memory.h.
//
// This file forces instantiation of the most common specialisations into the
// runtime library so the linker can resolve them from a single object file.

#include <memory.h>
#include <string.h>

namespace std {

// Force instantiation of allocator<char>
template class allocator<char>;

// Force instantiation of uninitialized helpers for char*
template char* uninitialized_copy<const char*, char*>(const char*, const char*, char*);
template void  uninitialized_fill<char*, char>(char*, char*, const char&);
template void  uninitialized_fill_n<char*, size_t, char>(char*, size_t, const char&);

} // namespace std
