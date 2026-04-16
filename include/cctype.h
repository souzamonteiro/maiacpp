// ISO/IEC 14882:1998(E) - 21.4 Null-terminated sequence utilities

#ifndef _CCTYPE_
#define _CCTYPE_

namespace std {

int isalnum(int c);
int isalpha(int c);
int iscntrl(int c);
int isdigit(int c);
int isgraph(int c);
int islower(int c);
int isprint(int c);
int ispunct(int c);
int isspace(int c);
int isupper(int c);
int isxdigit(int c);
int tolower(int c);
int toupper(int c);

} // namespace std

#endif