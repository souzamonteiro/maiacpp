// ISO/IEC 14882:1998(E) - 18.3, 18.7, 20.4.6, 25.4, 26.5

#ifndef _CSTDLIB_
#define _CSTDLIB_

namespace std {

typedef /* implementation-defined */ div_t;
typedef /* implementation-defined */ ldiv_t;
typedef /* implementation-defined */ size_t;

// String conversion
double atof(const char* nptr);
int atoi(const char* nptr);
long int atol(const char* nptr);
double strtod(const char* nptr, char** endptr);
long int strtol(const char* nptr, char** endptr, int base);
unsigned long int strtoul(const char* nptr, char** endptr, int base);

// Pseudo-random sequence
int rand(void);
void srand(unsigned int seed);

// Memory management
void* calloc(size_t nmemb, size_t size);
void* malloc(size_t size);
void* realloc(void* ptr, size_t size);
void free(void* ptr);

// Environment
void abort(void);
int atexit(void (*func)(void));
void exit(int status);
char* getenv(const char* name);
int system(const char* string);

// Searching and sorting
void* bsearch(const void* key, const void* base, size_t nmemb, size_t size,
              int (*compar)(const void*, const void*));
void qsort(void* base, size_t nmemb, size_t size,
           int (*compar)(const void*, const void*));

// Integer arithmetic
int abs(int j);
long int labs(long int j);
div_t div(int numer, int denom);
ldiv_t ldiv(long int numer, long int denom);

// Multibyte/wide character conversion
int mblen(const char* s, size_t n);
int mbtowc(wchar_t* pwc, const char* s, size_t n);
int wctomb(char* s, wchar_t wchar);
size_t mbstowcs(wchar_t* pwcs, const char* s, size_t n);
size_t wcstombs(char* s, const wchar_t* pwcs, size_t n);

} // namespace std

#define EXIT_FAILURE /* implementation-defined */
#define EXIT_SUCCESS /* implementation-defined */
#define MB_CUR_MAX   /* implementation-defined */
#define NULL         /* implementation-defined */
#define RAND_MAX     /* implementation-defined */

#endif