// ISO/IEC 14882:1998(E) - 21.4 Null-terminated sequence utilities

#ifndef _CWCHAR_
#define _CWCHAR_

#include <cstddef>
#include <cstdarg>
#include <cstdio>

namespace std {

typedef int mbstate_t; /* implementation-defined (provisional until wide-char library is implemented) */
typedef int wint_t;    /* implementation-defined (WASM/MaiaC profile) */

// Wide character input/output
int btowc(int c);
wint_t fgetwc(FILE* stream);
wchar_t* fgetws(wchar_t* s, int n, FILE* stream);
int fputwc(wchar_t c, FILE* stream);
int fputws(const wchar_t* s, FILE* stream);
int fwide(FILE* stream, int mode);
int fwprintf(FILE* stream, const wchar_t* format, ...);
int fwscanf(FILE* stream, const wchar_t* format, ...);
int getwc(FILE* stream);
wint_t getwchar(void);
int putwc(wchar_t c, FILE* stream);
int putwchar(wchar_t c);
int swprintf(wchar_t* s, size_t n, const wchar_t* format, ...);
int swscanf(const wchar_t* s, const wchar_t* format, ...);
wint_t ungetwc(wint_t c, FILE* stream);
int vfwprintf(FILE* stream, const wchar_t* format, va_list arg);
int vswprintf(wchar_t* s, size_t n, const wchar_t* format, va_list arg);
int vwprintf(const wchar_t* format, va_list arg);
int wprintf(const wchar_t* format, ...);
int wscanf(const wchar_t* format, ...);

// Wide string manipulation
wchar_t* wcscat(wchar_t* dest, const wchar_t* src);
const wchar_t* wcschr(const wchar_t* s, wchar_t c);
wchar_t* wcschr(wchar_t* s, wchar_t c);
int wcscmp(const wchar_t* s1, const wchar_t* s2);
int wcscoll(const wchar_t* s1, const wchar_t* s2);
wchar_t* wcscpy(wchar_t* dest, const wchar_t* src);
size_t wcscspn(const wchar_t* s, const wchar_t* reject);
size_t wcslen(const wchar_t* s);
wchar_t* wcsncat(wchar_t* dest, const wchar_t* src, size_t n);
int wcsncmp(const wchar_t* s1, const wchar_t* s2, size_t n);
wchar_t* wcsncpy(wchar_t* dest, const wchar_t* src, size_t n);
const wchar_t* wcspbrk(const wchar_t* s, const wchar_t* accept);
wchar_t* wcspbrk(wchar_t* s, const wchar_t* accept);
const wchar_t* wcsrchr(const wchar_t* s, wchar_t c);
wchar_t* wcsrchr(wchar_t* s, wchar_t c);
size_t wcsspn(const wchar_t* s, const wchar_t* accept);
const wchar_t* wcsstr(const wchar_t* s1, const wchar_t* s2);
wchar_t* wcsstr(wchar_t* s1, const wchar_t* s2);
double wcstod(const wchar_t* nptr, wchar_t** endptr);
long int wcstol(const wchar_t* nptr, wchar_t** endptr, int base);
unsigned long int wcstoul(const wchar_t* nptr, wchar_t** endptr, int base);
wchar_t* wcstok(wchar_t* s, const wchar_t* delim, wchar_t** ptr);
size_t wcsxfrm(wchar_t* dest, const wchar_t* src, size_t n);

// Multibyte/wide conversion
size_t mbrlen(const char* s, size_t n, mbstate_t* ps);
size_t mbrtowc(wchar_t* pwc, const char* s, size_t n, mbstate_t* ps);
int mbsinit(const mbstate_t* ps);
size_t mbsrtowcs(wchar_t* dst, const char** src, size_t len, mbstate_t* ps);
size_t wcrtomb(char* s, wchar_t wc, mbstate_t* ps);
size_t wcsrtombs(char* dst, const wchar_t** src, size_t len, mbstate_t* ps);
int wctob(wint_t c);

// Wide memory manipulation
wint_t wmemchr(wint_t c);
int wmemcmp(const wchar_t* s1, const wchar_t* s2, size_t n);
wchar_t* wmemcpy(wchar_t* dest, const wchar_t* src, size_t n);
wchar_t* wmemmove(wchar_t* dest, const wchar_t* src, size_t n);
wchar_t* wmemset(wchar_t* s, wchar_t c, size_t n);

} // namespace std

#define NULL      0           /* implementation-defined (WASM/MaiaC profile) */
#define WCHAR_MAX 2147483647  /* implementation-defined (provisional until wchar model is finalized) */
#define WCHAR_MIN (-2147483647 - 1) /* implementation-defined (provisional until wchar model is finalized) */
#define WEOF      (-1)        /* implementation-defined (WASM/MaiaC profile) */

#endif