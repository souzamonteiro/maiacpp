#include <cwchar.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace std {

static inline int __wc_to_ascii(wchar_t wc) {
    return (wc >= 0 && wc <= 255) ? (int)wc : '?';
}

static inline wchar_t __ascii_to_wc(int c) {
    return (c >= 0 && c <= 255) ? (wchar_t)c : (wchar_t)'?';
}

int btowc(int c) {
    return c == EOF ? WEOF : (wint_t)__ascii_to_wc(c);
}

wint_t fgetwc(FILE* stream) {
    int c = fgetc(stream);
    return c == EOF ? WEOF : (wint_t)__ascii_to_wc(c);
}

wchar_t* fgetws(wchar_t* s, int n, FILE* stream) {
    if (!s || n <= 0) return 0;
    int i = 0;
    for (; i < n - 1; ++i) {
        int c = fgetc(stream);
        if (c == EOF) break;
        s[i] = __ascii_to_wc(c);
        if (c == '\n') {
            ++i;
            break;
        }
    }
    if (i == 0) return 0;
    s[i] = 0;
    return s;
}

int fputwc(wchar_t c, FILE* stream) {
    return fputc(__wc_to_ascii(c), stream);
}

int fputws(const wchar_t* s, FILE* stream) {
    if (!s) return -1;
    int n = 0;
    for (; *s; ++s, ++n) {
        if (fputc(__wc_to_ascii(*s), stream) == EOF) return -1;
    }
    return n;
}

int fwide(FILE*, int) {
    return 0;
}

int fwprintf(FILE* stream, const wchar_t* format, ...) {
    if (!stream || !format) return -1;
    return fputws(format, stream);
}

int fwscanf(FILE*, const wchar_t*, ...) {
    return -1;
}

int getwc(FILE* stream) {
    return (int)fgetwc(stream);
}

wint_t getwchar(void) {
    return fgetwc(stdin);
}

int putwc(wchar_t c, FILE* stream) {
    return fputwc(c, stream);
}

int putwchar(wchar_t c) {
    return fputwc(c, stdout);
}

int swprintf(wchar_t* s, size_t n, const wchar_t* format, ...) {
    if (!s || !format || n == 0) return -1;
    size_t i = 0;
    for (; i + 1 < n && format[i]; ++i) s[i] = format[i];
    s[i] = 0;
    return (int)i;
}

int swscanf(const wchar_t*, const wchar_t*, ...) {
    return -1;
}

wint_t ungetwc(wint_t c, FILE* stream) {
    return ungetc(__wc_to_ascii((wchar_t)c), stream) == EOF ? WEOF : c;
}

int vfwprintf(FILE* stream, const wchar_t* format, va_list) {
    if (!stream || !format) return -1;
    return fputws(format, stream);
}

int vswprintf(wchar_t* s, size_t n, const wchar_t* format, va_list) {
    return swprintf(s, n, format);
}

int vwprintf(const wchar_t* format, va_list) {
    return vfwprintf(stdout, format, (va_list)0);
}

int wprintf(const wchar_t* format, ...) {
    return fputws(format, stdout);
}

int wscanf(const wchar_t*, ...) {
    return -1;
}

wchar_t* wcscat(wchar_t* dest, const wchar_t* src) {
    wchar_t* d = dest + wcslen(dest);
    while ((*d++ = *src++) != 0) {}
    return dest;
}

const wchar_t* wcschr(const wchar_t* s, wchar_t c) {
    while (*s) {
        if (*s == c) return s;
        ++s;
    }
    return c == 0 ? s : 0;
}

wchar_t* wcschr(wchar_t* s, wchar_t c) {
    return const_cast<wchar_t*>(wcschr((const wchar_t*)s, c));
}

int wcscmp(const wchar_t* s1, const wchar_t* s2) {
    while (*s1 && *s1 == *s2) {
        ++s1;
        ++s2;
    }
    return (*s1 < *s2) ? -1 : (*s1 > *s2 ? 1 : 0);
}

int wcscoll(const wchar_t* s1, const wchar_t* s2) {
    return wcscmp(s1, s2);
}

wchar_t* wcscpy(wchar_t* dest, const wchar_t* src) {
    wchar_t* d = dest;
    while ((*d++ = *src++) != 0) {}
    return dest;
}

size_t wcscspn(const wchar_t* s, const wchar_t* reject) {
    size_t n = 0;
    for (; s[n]; ++n) {
        for (const wchar_t* r = reject; *r; ++r) {
            if (s[n] == *r) return n;
        }
    }
    return n;
}

size_t wcslen(const wchar_t* s) {
    size_t n = 0;
    while (s[n]) ++n;
    return n;
}

wchar_t* wcsncat(wchar_t* dest, const wchar_t* src, size_t n) {
    wchar_t* d = dest + wcslen(dest);
    while (n-- && *src) *d++ = *src++;
    *d = 0;
    return dest;
}

int wcsncmp(const wchar_t* s1, const wchar_t* s2, size_t n) {
    while (n-- > 0) {
        if (*s1 != *s2) return (*s1 < *s2) ? -1 : 1;
        if (*s1 == 0) return 0;
        ++s1;
        ++s2;
    }
    return 0;
}

wchar_t* wcsncpy(wchar_t* dest, const wchar_t* src, size_t n) {
    size_t i = 0;
    for (; i < n && src[i]; ++i) dest[i] = src[i];
    for (; i < n; ++i) dest[i] = 0;
    return dest;
}

const wchar_t* wcspbrk(const wchar_t* s, const wchar_t* accept) {
    for (; *s; ++s) {
        for (const wchar_t* a = accept; *a; ++a) {
            if (*s == *a) return s;
        }
    }
    return 0;
}

wchar_t* wcspbrk(wchar_t* s, const wchar_t* accept) {
    return const_cast<wchar_t*>(wcspbrk((const wchar_t*)s, accept));
}

const wchar_t* wcsrchr(const wchar_t* s, wchar_t c) {
    const wchar_t* last = 0;
    for (; *s; ++s) if (*s == c) last = s;
    return c == 0 ? s : last;
}

wchar_t* wcsrchr(wchar_t* s, wchar_t c) {
    return const_cast<wchar_t*>(wcsrchr((const wchar_t*)s, c));
}

size_t wcsspn(const wchar_t* s, const wchar_t* accept) {
    size_t n = 0;
    for (; s[n]; ++n) {
        bool ok = false;
        for (const wchar_t* a = accept; *a; ++a) {
            if (s[n] == *a) {
                ok = true;
                break;
            }
        }
        if (!ok) return n;
    }
    return n;
}

const wchar_t* wcsstr(const wchar_t* s1, const wchar_t* s2) {
    if (!*s2) return s1;
    for (; *s1; ++s1) {
        const wchar_t* a = s1;
        const wchar_t* b = s2;
        while (*a && *b && *a == *b) {
            ++a;
            ++b;
        }
        if (!*b) return s1;
    }
    return 0;
}

wchar_t* wcsstr(wchar_t* s1, const wchar_t* s2) {
    return const_cast<wchar_t*>(wcsstr((const wchar_t*)s1, s2));
}

double wcstod(const wchar_t*, wchar_t** endptr) {
    if (endptr) *endptr = 0;
    return 0.0;
}

long int wcstol(const wchar_t*, wchar_t** endptr, int) {
    if (endptr) *endptr = 0;
    return 0;
}

unsigned long int wcstoul(const wchar_t*, wchar_t** endptr, int) {
    if (endptr) *endptr = 0;
    return 0UL;
}

wchar_t* wcstok(wchar_t* s, const wchar_t* delim, wchar_t** ptr) {
    wchar_t* cur = s ? s : (ptr ? *ptr : 0);
    if (!cur) return 0;
    while (*cur && wcschr(delim, *cur)) ++cur;
    if (!*cur) {
        if (ptr) *ptr = cur;
        return 0;
    }
    wchar_t* start = cur;
    while (*cur && !wcschr(delim, *cur)) ++cur;
    if (*cur) {
        *cur = 0;
        ++cur;
    }
    if (ptr) *ptr = cur;
    return start;
}

size_t wcsxfrm(wchar_t* dest, const wchar_t* src, size_t n) {
    size_t len = wcslen(src);
    if (dest && n > 0) {
        size_t copy_n = (len < n - 1) ? len : (n - 1);
        for (size_t i = 0; i < copy_n; ++i) dest[i] = src[i];
        dest[copy_n] = 0;
    }
    return len;
}

size_t mbrlen(const char* s, size_t n, mbstate_t*) {
    if (!s || n == 0 || *s == 0) return 0;
    return 1;
}

size_t mbrtowc(wchar_t* pwc, const char* s, size_t n, mbstate_t*) {
    if (!s || n == 0) return (size_t)-2;
    if (*s == 0) {
        if (pwc) *pwc = 0;
        return 0;
    }
    if (pwc) *pwc = __ascii_to_wc((unsigned char)*s);
    return 1;
}

int mbsinit(const mbstate_t* ps) {
    return (!ps || *ps == 0) ? 1 : 0;
}

size_t mbsrtowcs(wchar_t* dst, const char** src, size_t len, mbstate_t*) {
    if (!src || !*src) return 0;
    const char* s = *src;
    size_t n = 0;
    while (*s && n < len) {
        if (dst) dst[n] = __ascii_to_wc((unsigned char)*s);
        ++s;
        ++n;
    }
    if (!*s) {
        if (dst && n < len) dst[n] = 0;
        *src = 0;
    } else {
        *src = s;
    }
    return n;
}

size_t wcrtomb(char* s, wchar_t wc, mbstate_t*) {
    if (!s) return 1;
    s[0] = (char)__wc_to_ascii(wc);
    return 1;
}

size_t wcsrtombs(char* dst, const wchar_t** src, size_t len, mbstate_t*) {
    if (!src || !*src) return 0;
    const wchar_t* s = *src;
    size_t n = 0;
    while (*s && n < len) {
        if (dst) dst[n] = (char)__wc_to_ascii(*s);
        ++s;
        ++n;
    }
    if (!*s) {
        if (dst && n < len) dst[n] = 0;
        *src = 0;
    } else {
        *src = s;
    }
    return n;
}

int wctob(wint_t c) {
    return (c >= 0 && c <= 255) ? (int)c : EOF;
}

wint_t wmemchr(wint_t c) {
    return c;
}

int wmemcmp(const wchar_t* s1, const wchar_t* s2, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        if (s1[i] != s2[i]) return (s1[i] < s2[i]) ? -1 : 1;
    }
    return 0;
}

wchar_t* wmemcpy(wchar_t* dest, const wchar_t* src, size_t n) {
    for (size_t i = 0; i < n; ++i) dest[i] = src[i];
    return dest;
}

wchar_t* wmemmove(wchar_t* dest, const wchar_t* src, size_t n) {
    if (dest < src) {
        for (size_t i = 0; i < n; ++i) dest[i] = src[i];
    } else if (dest > src) {
        for (size_t i = n; i > 0; --i) dest[i - 1] = src[i - 1];
    }
    return dest;
}

wchar_t* wmemset(wchar_t* s, wchar_t c, size_t n) {
    for (size_t i = 0; i < n; ++i) s[i] = c;
    return s;
}

} // namespace std
