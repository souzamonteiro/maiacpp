// ISO/IEC 14882:1998(E) - 21.4 Null-terminated sequence utilities

#ifndef _CWCTYPE_
#define _CWCTYPE_

#include <cwchar>

namespace std {

typedef int wctrans_t; /* implementation-defined (provisional until wide-char library is implemented) */
typedef int wctype_t;  /* implementation-defined (provisional until wide-char library is implemented) */

int iswalnum(wint_t wc);
int iswalpha(wint_t wc);
int iswcntrl(wint_t wc);
int iswdigit(wint_t wc);
int iswgraph(wint_t wc);
int iswlower(wint_t wc);
int iswprint(wint_t wc);
int iswpunct(wint_t wc);
int iswspace(wint_t wc);
int iswupper(wint_t wc);
int iswxdigit(wint_t wc);
int iswctype(wint_t wc, wctype_t desc);
wctype_t wctype(const char* property);

wint_t towlower(wint_t wc);
wint_t towupper(wint_t wc);
wint_t towctrans(wint_t wc, wctrans_t desc);
wctrans_t wctrans(const char* property);

} // namespace std

#define WEOF (-1) /* implementation-defined (WASM/MaiaC profile) */

#endif