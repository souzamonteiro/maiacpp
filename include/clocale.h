// ISO/IEC 14882:1998(E) - 22.3 C Library Locales

#ifndef _CLOCALE_
#define _CLOCALE_

namespace std {

struct lconv {
    char* decimal_point;
    char* thousands_sep;
    char* grouping;
    char* int_curr_symbol;
    char* currency_symbol;
    char* mon_decimal_point;
    char* mon_thousands_sep;
    char* mon_grouping;
    char* positive_sign;
    char* negative_sign;
    char int_frac_digits;
    char frac_digits;
    char p_cs_precedes;
    char p_sep_by_space;
    char n_cs_precedes;
    char n_sep_by_space;
    char p_sign_posn;
    char n_sign_posn;
    // ... outros campos implementation-defined
};

char* setlocale(int category, const char* locale);
struct lconv* localeconv();

} // namespace std

#define LC_ALL      0 /* implementation-defined (WASM/MaiaC profile) */
#define LC_COLLATE  1 /* implementation-defined (WASM/MaiaC profile) */
#define LC_CTYPE    2 /* implementation-defined (WASM/MaiaC profile) */
#define LC_MONETARY 3 /* implementation-defined (WASM/MaiaC profile) */
#define LC_NUMERIC  4 /* implementation-defined (WASM/MaiaC profile) */
#define LC_TIME     5 /* implementation-defined (WASM/MaiaC profile) */
#define NULL        0 /* implementation-defined (WASM/MaiaC profile) */

#endif