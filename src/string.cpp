// MaiaCpp string runtime – MaiaC-first implementation
//
// Provides explicit char specialisation of basic_string<char> (= std::string).
// All storage is managed with MaiaC-backed malloc/realloc/free; all character
// operations delegate to C89 <string.h> / <stdlib.h> functions so that
// MaiaC/webc automatically wires them to their JS host equivalents.
//
// Wide-char (wstring) is not specialised in this file; it remains a stub.

#include <string.h>
#include <cstdlib.h>
#include <cstring.h>
#include <cstddef.h>
#include <stdexcept.h>

namespace std {

// ---------------------------------------------------------------------------
// Internal helpers (file-local)
// ---------------------------------------------------------------------------

static inline size_t __str_next_cap(size_t current, size_t needed) {
    size_t cap = current ? current : 15u;
    while (cap < needed) cap = cap + cap / 2 + 1;
    return cap;
}

// ---------------------------------------------------------------------------
// basic_string<char> – storage management helpers
// _data is always NUL-terminated; _cap excludes the NUL slot.
// ---------------------------------------------------------------------------

template<>
basic_string<char>::basic_string(const allocator<char>&)
    : _data(0), _size(0), _cap(0) {
    _data = (char*)::malloc(1);
    if (_data) _data[0] = '\0';
}

template<>
basic_string<char>::basic_string(const basic_string<char>& str,
                                  size_type pos,
                                  size_type n,
                                  const allocator<char>&)
    : _data(0), _size(0), _cap(0) {
    size_type slen = str._size;
    if (pos > slen) pos = slen;
    size_type count = (n == npos || pos + n > slen) ? slen - pos : n;
    _cap = count;
    _data = (char*)::malloc(_cap + 1);
    if (_data) {
        ::memcpy(_data, str._data + pos, count);
        _data[count] = '\0';
        _size = count;
    }
}

template<>
basic_string<char>::basic_string(const char* s, size_type n, const allocator<char>&)
    : _data(0), _size(0), _cap(0) {
    _cap = n;
    _data = (char*)::malloc(_cap + 1);
    if (_data) {
        ::memcpy(_data, s, n);
        _data[n] = '\0';
        _size = n;
    }
}

template<>
basic_string<char>::basic_string(const char* s, const allocator<char>&)
    : _data(0), _size(0), _cap(0) {
    size_type n = s ? (size_type)::strlen(s) : 0;
    _cap = n;
    _data = (char*)::malloc(_cap + 1);
    if (_data) {
        if (s) ::memcpy(_data, s, n);
        _data[n] = '\0';
        _size = n;
    }
}

template<>
basic_string<char>::basic_string(size_type n, char c, const allocator<char>&)
    : _data(0), _size(0), _cap(0) {
    _cap = n;
    _data = (char*)::malloc(_cap + 1);
    if (_data) {
        ::memset(_data, (unsigned char)c, n);
        _data[n] = '\0';
        _size = n;
    }
}

template<>
basic_string<char>::~basic_string() {
    ::free(_data);
    _data = 0;
    _size = 0;
    _cap  = 0;
}

// ---------------------------------------------------------------------------
// Internal grow helper (not a declared member; implemented via reserve)
// ---------------------------------------------------------------------------

template<>
void basic_string<char>::reserve(size_type res_arg) {
    if (res_arg <= _cap) return;
    size_type newcap = __str_next_cap(_cap, res_arg);
    char* p = (char*)::realloc(_data, newcap + 1);
    if (p) {
        _data = p;
        _cap  = newcap;
    }
}

// ---------------------------------------------------------------------------
// Assignment operators
// ---------------------------------------------------------------------------

template<>
basic_string<char>& basic_string<char>::operator=(const basic_string<char>& str) {
    if (this == &str) return *this;
    reserve(str._size);
    ::memcpy(_data, str._data, str._size + 1);
    _size = str._size;
    return *this;
}

template<>
basic_string<char>& basic_string<char>::operator=(const char* s) {
    size_type n = s ? (size_type)::strlen(s) : 0;
    reserve(n);
    if (s) ::memcpy(_data, s, n + 1);
    else   _data[0] = '\0';
    _size = n;
    return *this;
}

template<>
basic_string<char>& basic_string<char>::operator=(char c) {
    reserve(1);
    _data[0] = c;
    _data[1] = '\0';
    _size = 1;
    return *this;
}

// ---------------------------------------------------------------------------
// Iterators
// ---------------------------------------------------------------------------

template<> basic_string<char>::iterator       basic_string<char>::begin()       { return _data; }
template<> basic_string<char>::const_iterator basic_string<char>::begin() const { return _data; }
template<> basic_string<char>::iterator       basic_string<char>::end()         { return _data + _size; }
template<> basic_string<char>::const_iterator basic_string<char>::end()   const { return _data + _size; }

template<> basic_string<char>::reverse_iterator       basic_string<char>::rbegin()       { return reverse_iterator(end()); }
template<> basic_string<char>::const_reverse_iterator basic_string<char>::rbegin() const { return const_reverse_iterator(end()); }
template<> basic_string<char>::reverse_iterator       basic_string<char>::rend()         { return reverse_iterator(begin()); }
template<> basic_string<char>::const_reverse_iterator basic_string<char>::rend()   const { return const_reverse_iterator(begin()); }

// ---------------------------------------------------------------------------
// Size / capacity
// ---------------------------------------------------------------------------

template<> basic_string<char>::size_type basic_string<char>::size()     const { return _size; }
template<> basic_string<char>::size_type basic_string<char>::length()   const { return _size; }
template<> basic_string<char>::size_type basic_string<char>::max_size() const { return (size_type)(-1) / 2; }
template<> basic_string<char>::size_type basic_string<char>::capacity() const { return _cap; }
template<> bool                          basic_string<char>::empty()    const { return _size == 0; }

template<>
void basic_string<char>::resize(size_type n, char c) {
    if (n > _size) {
        reserve(n);
        ::memset(_data + _size, (unsigned char)c, n - _size);
    }
    _size = n;
    _data[n] = '\0';
}

template<>
void basic_string<char>::resize(size_type n) {
    resize(n, '\0');
}

template<>
void basic_string<char>::clear() {
    _size = 0;
    if (_data) _data[0] = '\0';
}

// ---------------------------------------------------------------------------
// Element access
// ---------------------------------------------------------------------------

template<>
basic_string<char>::const_reference basic_string<char>::operator[](size_type pos) const {
    return _data[pos];
}

template<>
basic_string<char>::reference basic_string<char>::operator[](size_type pos) {
    return _data[pos];
}

template<>
basic_string<char>::const_reference basic_string<char>::at(size_type n) const {
    if (n >= _size) throw out_of_range("basic_string::at");
    return _data[n];
}

template<>
basic_string<char>::reference basic_string<char>::at(size_type n) {
    if (n >= _size) throw out_of_range("basic_string::at");
    return _data[n];
}

// ---------------------------------------------------------------------------
// Append
// ---------------------------------------------------------------------------

template<>
basic_string<char>& basic_string<char>::append(const basic_string<char>& str) {
    return append(str._data, str._size);
}

template<>
basic_string<char>& basic_string<char>::append(const basic_string<char>& str,
                                                 size_type pos, size_type n) {
    if (pos > str._size) throw out_of_range("basic_string::append");
    size_type count = (n == npos || pos + n > str._size) ? str._size - pos : n;
    return append(str._data + pos, count);
}

template<>
basic_string<char>& basic_string<char>::append(const char* s, size_type n) {
    reserve(_size + n);
    ::memcpy(_data + _size, s, n);
    _size += n;
    _data[_size] = '\0';
    return *this;
}

template<>
basic_string<char>& basic_string<char>::append(const char* s) {
    return append(s, s ? (size_type)::strlen(s) : 0);
}

template<>
basic_string<char>& basic_string<char>::append(size_type n, char c) {
    reserve(_size + n);
    ::memset(_data + _size, (unsigned char)c, n);
    _size += n;
    _data[_size] = '\0';
    return *this;
}

template<>
void basic_string<char>::push_back(const char c) {
    reserve(_size + 1);
    _data[_size++] = c;
    _data[_size]   = '\0';
}

// ---------------------------------------------------------------------------
// operator+=
// ---------------------------------------------------------------------------

template<>
basic_string<char>& basic_string<char>::operator+=(const basic_string<char>& str) {
    return append(str);
}

template<>
basic_string<char>& basic_string<char>::operator+=(const char* s) {
    return append(s);
}

template<>
basic_string<char>& basic_string<char>::operator+=(char c) {
    push_back(c);
    return *this;
}

// ---------------------------------------------------------------------------
// Assign
// ---------------------------------------------------------------------------

template<>
basic_string<char>& basic_string<char>::assign(const basic_string<char>& str) {
    return operator=(str);
}

template<>
basic_string<char>& basic_string<char>::assign(const basic_string<char>& str,
                                                 size_type pos, size_type n) {
    if (pos > str._size) throw out_of_range("basic_string::assign");
    size_type count = (n == npos || pos + n > str._size) ? str._size - pos : n;
    reserve(count);
    ::memcpy(_data, str._data + pos, count);
    _size = count;
    _data[_size] = '\0';
    return *this;
}

template<>
basic_string<char>& basic_string<char>::assign(const char* s, size_type n) {
    reserve(n);
    ::memcpy(_data, s, n);
    _size = n;
    _data[_size] = '\0';
    return *this;
}

template<>
basic_string<char>& basic_string<char>::assign(const char* s) {
    return operator=(s);
}

template<>
basic_string<char>& basic_string<char>::assign(size_type n, char c) {
    reserve(n);
    ::memset(_data, (unsigned char)c, n);
    _size = n;
    _data[_size] = '\0';
    return *this;
}

// ---------------------------------------------------------------------------
// Insert
// ---------------------------------------------------------------------------

template<>
basic_string<char>& basic_string<char>::insert(size_type pos, const basic_string<char>& str) {
    return insert(pos, str._data, str._size);
}

template<>
basic_string<char>& basic_string<char>::insert(size_type pos,
                                                 const basic_string<char>& str,
                                                 size_type pos2, size_type n) {
    if (pos2 > str._size) throw out_of_range("basic_string::insert");
    size_type count = (n == npos || pos2 + n > str._size) ? str._size - pos2 : n;
    return insert(pos, str._data + pos2, count);
}

template<>
basic_string<char>& basic_string<char>::insert(size_type pos, const char* s, size_type n) {
    if (pos > _size) throw out_of_range("basic_string::insert");
    reserve(_size + n);
    ::memmove(_data + pos + n, _data + pos, _size - pos + 1);
    ::memcpy(_data + pos, s, n);
    _size += n;
    return *this;
}

template<>
basic_string<char>& basic_string<char>::insert(size_type pos, const char* s) {
    return insert(pos, s, s ? (size_type)::strlen(s) : 0);
}

template<>
basic_string<char>& basic_string<char>::insert(size_type pos, size_type n, char c) {
    if (pos > _size) throw out_of_range("basic_string::insert");
    reserve(_size + n);
    ::memmove(_data + pos + n, _data + pos, _size - pos + 1);
    ::memset(_data + pos, (unsigned char)c, n);
    _size += n;
    return *this;
}

template<>
basic_string<char>::iterator basic_string<char>::insert(iterator p, char c) {
    size_type pos = (size_type)(p - _data);
    insert(pos, 1, c);
    return _data + pos;
}

template<>
void basic_string<char>::insert(iterator p, size_type n, char c) {
    size_type pos = (size_type)(p - _data);
    insert(pos, n, c);
}

// ---------------------------------------------------------------------------
// Erase
// ---------------------------------------------------------------------------

template<>
basic_string<char>& basic_string<char>::erase(size_type pos, size_type n) {
    if (pos > _size) throw out_of_range("basic_string::erase");
    size_type count = (n == npos || pos + n > _size) ? _size - pos : n;
    ::memmove(_data + pos, _data + pos + count, _size - pos - count + 1);
    _size -= count;
    return *this;
}

template<>
basic_string<char>::iterator basic_string<char>::erase(iterator position) {
    size_type pos = (size_type)(position - _data);
    erase(pos, 1);
    return _data + pos;
}

template<>
basic_string<char>::iterator basic_string<char>::erase(iterator first, iterator last) {
    size_type pos = (size_type)(first - _data);
    size_type n   = (size_type)(last - first);
    erase(pos, n);
    return _data + pos;
}

// ---------------------------------------------------------------------------
// Replace
// ---------------------------------------------------------------------------

template<>
basic_string<char>& basic_string<char>::replace(size_type pos1, size_type n1,
                                                  const basic_string<char>& str) {
    return replace(pos1, n1, str._data, str._size);
}

template<>
basic_string<char>& basic_string<char>::replace(size_type pos1, size_type n1,
                                                  const basic_string<char>& str,
                                                  size_type pos2, size_type n2) {
    if (pos2 > str._size) throw out_of_range("basic_string::replace");
    size_type count = (n2 == npos || pos2 + n2 > str._size) ? str._size - pos2 : n2;
    return replace(pos1, n1, str._data + pos2, count);
}

template<>
basic_string<char>& basic_string<char>::replace(size_type pos, size_type n1,
                                                  const char* s, size_type n2) {
    if (pos > _size) throw out_of_range("basic_string::replace");
    if (n1 == npos || pos + n1 > _size) n1 = _size - pos;
    size_type newsize = _size - n1 + n2;
    reserve(newsize);
    ::memmove(_data + pos + n2, _data + pos + n1, _size - pos - n1 + 1);
    ::memcpy(_data + pos, s, n2);
    _size = newsize;
    return *this;
}

template<>
basic_string<char>& basic_string<char>::replace(size_type pos, size_type n1, const char* s) {
    return replace(pos, n1, s, s ? (size_type)::strlen(s) : 0);
}

template<>
basic_string<char>& basic_string<char>::replace(size_type pos, size_type n1,
                                                  size_type n2, char c) {
    if (pos > _size) throw out_of_range("basic_string::replace");
    if (n1 == npos || pos + n1 > _size) n1 = _size - pos;
    size_type newsize = _size - n1 + n2;
    reserve(newsize);
    ::memmove(_data + pos + n2, _data + pos + n1, _size - pos - n1 + 1);
    ::memset(_data + pos, (unsigned char)c, n2);
    _size = newsize;
    return *this;
}

template<>
basic_string<char>& basic_string<char>::replace(iterator i1, iterator i2,
                                                  const basic_string<char>& str) {
    return replace((size_type)(i1 - _data), (size_type)(i2 - i1), str);
}

template<>
basic_string<char>& basic_string<char>::replace(iterator i1, iterator i2,
                                                  const char* s, size_type n) {
    return replace((size_type)(i1 - _data), (size_type)(i2 - i1), s, n);
}

template<>
basic_string<char>& basic_string<char>::replace(iterator i1, iterator i2, const char* s) {
    return replace((size_type)(i1 - _data), (size_type)(i2 - i1), s);
}

template<>
basic_string<char>& basic_string<char>::replace(iterator i1, iterator i2,
                                                  size_type n, char c) {
    return replace((size_type)(i1 - _data), (size_type)(i2 - i1), n, c);
}

// ---------------------------------------------------------------------------
// Copy / swap
// ---------------------------------------------------------------------------

template<>
basic_string<char>::size_type basic_string<char>::copy(char* s, size_type n,
                                                         size_type pos) const {
    if (pos > _size) throw out_of_range("basic_string::copy");
    size_type count = (n == npos || pos + n > _size) ? _size - pos : n;
    ::memcpy(s, _data + pos, count);
    // copy() does NOT NUL-terminate per C++98 spec
    return count;
}

template<>
void basic_string<char>::swap(basic_string<char>& other) {
    char*     td = _data;  _data = other._data;  other._data = td;
    size_type ts = _size;  _size = other._size;  other._size = ts;
    size_type tc = _cap;   _cap  = other._cap;   other._cap  = tc;
}

// ---------------------------------------------------------------------------
// C-string access
// ---------------------------------------------------------------------------

template<> const char* basic_string<char>::c_str() const { return _data ? _data : ""; }
template<> const char* basic_string<char>::data()  const { return _data ? _data : ""; }
template<> basic_string<char>::allocator_type basic_string<char>::get_allocator() const { return allocator<char>(); }

// ---------------------------------------------------------------------------
// Find
// ---------------------------------------------------------------------------

template<>
basic_string<char>::size_type basic_string<char>::find(const basic_string<char>& str,
                                                         size_type pos) const {
    return find(str._data, pos, str._size);
}

template<>
basic_string<char>::size_type basic_string<char>::find(const char* s, size_type pos,
                                                         size_type n) const {
    if (!_data || pos > _size) return npos;
    if (n == 0) return pos;
    for (size_type i = pos; i + n <= _size; ++i) {
        if (::memcmp(_data + i, s, n) == 0) return i;
    }
    return npos;
}

template<>
basic_string<char>::size_type basic_string<char>::find(const char* s, size_type pos) const {
    return find(s, pos, s ? (size_type)::strlen(s) : 0);
}

template<>
basic_string<char>::size_type basic_string<char>::find(char c, size_type pos) const {
    for (size_type i = pos; i < _size; ++i) {
        if (_data[i] == c) return i;
    }
    return npos;
}

// rfind

template<>
basic_string<char>::size_type basic_string<char>::rfind(const basic_string<char>& str,
                                                          size_type pos) const {
    return rfind(str._data, pos, str._size);
}

template<>
basic_string<char>::size_type basic_string<char>::rfind(const char* s, size_type pos,
                                                          size_type n) const {
    if (!_data || n > _size) return npos;
    size_type start = (_size - n < pos) ? _size - n : pos;
    for (size_type i = start + 1; i-- > 0; ) {
        if (::memcmp(_data + i, s, n) == 0) return i;
    }
    return npos;
}

template<>
basic_string<char>::size_type basic_string<char>::rfind(const char* s, size_type pos) const {
    return rfind(s, pos, s ? (size_type)::strlen(s) : 0);
}

template<>
basic_string<char>::size_type basic_string<char>::rfind(char c, size_type pos) const {
    if (!_data || _size == 0) return npos;
    size_type start = (pos >= _size) ? _size - 1 : pos;
    for (size_type i = start + 1; i-- > 0; ) {
        if (_data[i] == c) return i;
    }
    return npos;
}

// find_first_of

template<>
basic_string<char>::size_type basic_string<char>::find_first_of(const basic_string<char>& str,
                                                                  size_type pos) const {
    return find_first_of(str._data, pos, str._size);
}

template<>
basic_string<char>::size_type basic_string<char>::find_first_of(const char* s, size_type pos,
                                                                  size_type n) const {
    for (size_type i = pos; i < _size; ++i) {
        for (size_type j = 0; j < n; ++j) {
            if (_data[i] == s[j]) return i;
        }
    }
    return npos;
}

template<>
basic_string<char>::size_type basic_string<char>::find_first_of(const char* s,
                                                                  size_type pos) const {
    return find_first_of(s, pos, s ? (size_type)::strlen(s) : 0);
}

template<>
basic_string<char>::size_type basic_string<char>::find_first_of(char c, size_type pos) const {
    return find(c, pos);
}

// find_last_of

template<>
basic_string<char>::size_type basic_string<char>::find_last_of(const basic_string<char>& str,
                                                                 size_type pos) const {
    return find_last_of(str._data, pos, str._size);
}

template<>
basic_string<char>::size_type basic_string<char>::find_last_of(const char* s, size_type pos,
                                                                 size_type n) const {
    if (!_data || _size == 0) return npos;
    size_type start = (pos >= _size) ? _size - 1 : pos;
    for (size_type i = start + 1; i-- > 0; ) {
        for (size_type j = 0; j < n; ++j) {
            if (_data[i] == s[j]) return i;
        }
    }
    return npos;
}

template<>
basic_string<char>::size_type basic_string<char>::find_last_of(const char* s,
                                                                 size_type pos) const {
    return find_last_of(s, pos, s ? (size_type)::strlen(s) : 0);
}

template<>
basic_string<char>::size_type basic_string<char>::find_last_of(char c, size_type pos) const {
    return rfind(c, pos);
}

// find_first_not_of

template<>
basic_string<char>::size_type basic_string<char>::find_first_not_of(const basic_string<char>& str,
                                                                      size_type pos) const {
    return find_first_not_of(str._data, pos, str._size);
}

template<>
basic_string<char>::size_type basic_string<char>::find_first_not_of(const char* s, size_type pos,
                                                                      size_type n) const {
    for (size_type i = pos; i < _size; ++i) {
        bool found = false;
        for (size_type j = 0; j < n; ++j) {
            if (_data[i] == s[j]) { found = true; break; }
        }
        if (!found) return i;
    }
    return npos;
}

template<>
basic_string<char>::size_type basic_string<char>::find_first_not_of(const char* s,
                                                                      size_type pos) const {
    return find_first_not_of(s, pos, s ? (size_type)::strlen(s) : 0);
}

template<>
basic_string<char>::size_type basic_string<char>::find_first_not_of(char c, size_type pos) const {
    for (size_type i = pos; i < _size; ++i) {
        if (_data[i] != c) return i;
    }
    return npos;
}

// find_last_not_of

template<>
basic_string<char>::size_type basic_string<char>::find_last_not_of(const basic_string<char>& str,
                                                                     size_type pos) const {
    return find_last_not_of(str._data, pos, str._size);
}

template<>
basic_string<char>::size_type basic_string<char>::find_last_not_of(const char* s, size_type pos,
                                                                     size_type n) const {
    if (!_data || _size == 0) return npos;
    size_type start = (pos >= _size) ? _size - 1 : pos;
    for (size_type i = start + 1; i-- > 0; ) {
        bool found = false;
        for (size_type j = 0; j < n; ++j) {
            if (_data[i] == s[j]) { found = true; break; }
        }
        if (!found) return i;
    }
    return npos;
}

template<>
basic_string<char>::size_type basic_string<char>::find_last_not_of(const char* s,
                                                                     size_type pos) const {
    return find_last_not_of(s, pos, s ? (size_type)::strlen(s) : 0);
}

template<>
basic_string<char>::size_type basic_string<char>::find_last_not_of(char c, size_type pos) const {
    if (!_data || _size == 0) return npos;
    size_type start = (pos >= _size) ? _size - 1 : pos;
    for (size_type i = start + 1; i-- > 0; ) {
        if (_data[i] != c) return i;
    }
    return npos;
}

// ---------------------------------------------------------------------------
// Substr / compare
// ---------------------------------------------------------------------------

template<>
basic_string<char> basic_string<char>::substr(size_type pos, size_type n) const {
    if (pos > _size) throw out_of_range("basic_string::substr");
    size_type count = (n == npos || pos + n > _size) ? _size - pos : n;
    return basic_string<char>(_data + pos, count);
}

template<>
int basic_string<char>::compare(const basic_string<char>& str) const {
    size_type n = _size < str._size ? _size : str._size;
    int r = ::memcmp(_data, str._data, n);
    if (r != 0) return r;
    if (_size < str._size) return -1;
    if (_size > str._size) return  1;
    return 0;
}

template<>
int basic_string<char>::compare(size_type pos1, size_type n1,
                                  const basic_string<char>& str) const {
    return substr(pos1, n1).compare(str);
}

template<>
int basic_string<char>::compare(size_type pos1, size_type n1,
                                  const basic_string<char>& str,
                                  size_type pos2, size_type n2) const {
    return substr(pos1, n1).compare(str.substr(pos2, n2));
}

template<>
int basic_string<char>::compare(const char* s) const {
    if (!s) return _size > 0 ? 1 : 0;
    size_type n = (size_type)::strlen(s);
    size_type m = _size < n ? _size : n;
    int r = ::memcmp(_data, s, m);
    if (r != 0) return r;
    if (_size < n) return -1;
    if (_size > n) return  1;
    return 0;
}

template<>
int basic_string<char>::compare(size_type pos1, size_type n1,
                                  const char* s, size_type n2) const {
    if (n2 == npos) n2 = s ? (size_type)::strlen(s) : 0;
    return substr(pos1, n1).compare(basic_string<char>(s, n2));
}

// ---------------------------------------------------------------------------
// Non-member operators (char specialisation)
// ---------------------------------------------------------------------------

template<>
basic_string<char> operator+(const basic_string<char>& lhs, const basic_string<char>& rhs) {
    basic_string<char> result(lhs);
    result.append(rhs);
    return result;
}

template<>
basic_string<char> operator+(const char* lhs, const basic_string<char>& rhs) {
    basic_string<char> result(lhs);
    result.append(rhs);
    return result;
}

template<>
basic_string<char> operator+(char lhs, const basic_string<char>& rhs) {
    basic_string<char> result(1, lhs);
    result.append(rhs);
    return result;
}

template<>
basic_string<char> operator+(const basic_string<char>& lhs, const char* rhs) {
    basic_string<char> result(lhs);
    result.append(rhs);
    return result;
}

template<>
basic_string<char> operator+(const basic_string<char>& lhs, char rhs) {
    basic_string<char> result(lhs);
    result.push_back(rhs);
    return result;
}

template<>
bool operator==(const basic_string<char>& lhs, const basic_string<char>& rhs) {
    return lhs.compare(rhs) == 0;
}

template<>
bool operator==(const char* lhs, const basic_string<char>& rhs) {
    return rhs.compare(lhs) == 0;
}

template<>
bool operator==(const basic_string<char>& lhs, const char* rhs) {
    return lhs.compare(rhs) == 0;
}

template<>
bool operator!=(const basic_string<char>& lhs, const basic_string<char>& rhs) {
    return lhs.compare(rhs) != 0;
}

template<>
bool operator!=(const char* lhs, const basic_string<char>& rhs) {
    return !(lhs == rhs);
}

template<>
bool operator!=(const basic_string<char>& lhs, const char* rhs) {
    return lhs.compare(rhs) != 0;
}

template<>
bool operator<(const basic_string<char>& lhs, const basic_string<char>& rhs) {
    return lhs.compare(rhs) < 0;
}

template<>
bool operator<(const basic_string<char>& lhs, const char* rhs) {
    return lhs.compare(rhs) < 0;
}

template<>
bool operator<(const char* lhs, const basic_string<char>& rhs) {
    return rhs.compare(lhs) > 0;
}

template<>
bool operator>(const basic_string<char>& lhs, const basic_string<char>& rhs) {
    return lhs.compare(rhs) > 0;
}

template<>
bool operator>(const basic_string<char>& lhs, const char* rhs) {
    return lhs.compare(rhs) > 0;
}

template<>
bool operator>(const char* lhs, const basic_string<char>& rhs) {
    return rhs.compare(lhs) < 0;
}

template<>
bool operator<=(const basic_string<char>& lhs, const basic_string<char>& rhs) {
    return lhs.compare(rhs) <= 0;
}

template<>
bool operator<=(const basic_string<char>& lhs, const char* rhs) {
    return lhs.compare(rhs) <= 0;
}

template<>
bool operator<=(const char* lhs, const basic_string<char>& rhs) {
    return rhs.compare(lhs) >= 0;
}

template<>
bool operator>=(const basic_string<char>& lhs, const basic_string<char>& rhs) {
    return lhs.compare(rhs) >= 0;
}

template<>
bool operator>=(const basic_string<char>& lhs, const char* rhs) {
    return lhs.compare(rhs) >= 0;
}

template<>
bool operator>=(const char* lhs, const basic_string<char>& rhs) {
    return rhs.compare(lhs) <= 0;
}

} // namespace std
