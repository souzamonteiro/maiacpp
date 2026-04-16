// ISO/IEC 14882:1998(E) - 20.4.6, 21.4

#ifndef _CSTRING_
#define _CSTRING_

namespace std {

// Copying
void* memcpy(void* dest, const void* src, size_t n);
void* memmove(void* dest, const void* src, size_t n);
char* strcpy(char* dest, const char* src);
char* strncpy(char* dest, const char* src, size_t n);

// Concatenation
char* strcat(char* dest, const char* src);
char* strncat(char* dest, const char* src, size_t n);

// Comparison
int memcmp(const void* s1, const void* s2, size_t n);
int strcmp(const char* s1, const char* s2);
int strcoll(const char* s1, const char* s2);
int strncmp(const char* s1, const char* s2, size_t n);
size_t strxfrm(char* dest, const char* src, size_t n);

// Searching
const void* memchr(const void* s, int c, size_t n);
void* memchr(void* s, int c, size_t n);
const char* strchr(const char* s, int c);
char* strchr(char* s, int c);
size_t strcspn(const char* s, const char* reject);
const char* strpbrk(const char* s, const char* accept);
char* strpbrk(char* s, const char* accept);
const char* strrchr(const char* s, int c);
char* strrchr(char* s, int c);
size_t strspn(const char* s, const char* accept);
const char* strstr(const char* s1, const char* s2);
char* strstr(char* s1, const char* s2);
char* strtok(char* s, const char* delim);

// Miscellaneous
void* memset(void* s, int c, size_t n);
char* strerror(int errnum);
size_t strlen(const char* s);

} // namespace std

#define NULL 0 /* implementation-defined (WASM/MaiaC profile) */

#endif