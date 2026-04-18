#include <cwctype.h>
#include <cctype>
#include <cstring>

namespace std {

static inline int __wc_to_ascii(wint_t wc) {
    return (wc >= 0 && wc <= 255) ? (int)wc : '?';
}

int iswalnum(wint_t wc) { return ::isalnum(__wc_to_ascii(wc)); }
int iswalpha(wint_t wc) { return ::isalpha(__wc_to_ascii(wc)); }
int iswcntrl(wint_t wc) { return ::iscntrl(__wc_to_ascii(wc)); }
int iswdigit(wint_t wc) { return ::isdigit(__wc_to_ascii(wc)); }
int iswgraph(wint_t wc) { return ::isgraph(__wc_to_ascii(wc)); }
int iswlower(wint_t wc) { return ::islower(__wc_to_ascii(wc)); }
int iswprint(wint_t wc) { return ::isprint(__wc_to_ascii(wc)); }
int iswpunct(wint_t wc) { return ::ispunct(__wc_to_ascii(wc)); }
int iswspace(wint_t wc) { return ::isspace(__wc_to_ascii(wc)); }
int iswupper(wint_t wc) { return ::isupper(__wc_to_ascii(wc)); }
int iswxdigit(wint_t wc) { return ::isxdigit(__wc_to_ascii(wc)); }

wctype_t wctype(const char* property) {
    if (!property) return 0;
    if (std::strcmp(property, "alnum") == 0) return 1;
    if (std::strcmp(property, "alpha") == 0) return 2;
    if (std::strcmp(property, "cntrl") == 0) return 3;
    if (std::strcmp(property, "digit") == 0) return 4;
    if (std::strcmp(property, "graph") == 0) return 5;
    if (std::strcmp(property, "lower") == 0) return 6;
    if (std::strcmp(property, "print") == 0) return 7;
    if (std::strcmp(property, "punct") == 0) return 8;
    if (std::strcmp(property, "space") == 0) return 9;
    if (std::strcmp(property, "upper") == 0) return 10;
    if (std::strcmp(property, "xdigit") == 0) return 11;
    return 0;
}

int iswctype(wint_t wc, wctype_t desc) {
    switch (desc) {
        case 1: return iswalnum(wc);
        case 2: return iswalpha(wc);
        case 3: return iswcntrl(wc);
        case 4: return iswdigit(wc);
        case 5: return iswgraph(wc);
        case 6: return iswlower(wc);
        case 7: return iswprint(wc);
        case 8: return iswpunct(wc);
        case 9: return iswspace(wc);
        case 10: return iswupper(wc);
        case 11: return iswxdigit(wc);
        default: return 0;
    }
}

wint_t towlower(wint_t wc) { return (wint_t)::tolower(__wc_to_ascii(wc)); }
wint_t towupper(wint_t wc) { return (wint_t)::toupper(__wc_to_ascii(wc)); }

wctrans_t wctrans(const char* property) {
    if (!property) return 0;
    if (std::strcmp(property, "tolower") == 0) return 1;
    if (std::strcmp(property, "toupper") == 0) return 2;
    return 0;
}

wint_t towctrans(wint_t wc, wctrans_t desc) {
    if (desc == 1) return towlower(wc);
    if (desc == 2) return towupper(wc);
    return wc;
}

} // namespace std
