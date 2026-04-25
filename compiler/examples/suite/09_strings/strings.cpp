// 09_strings — Exercises C-string operations via <string.h>
#include <stdio.h>
#include <string.h>

static int char_count(const char* s, char c) {
    int n = 0;
    while (*s) { if (*s == c) ++n; ++s; }
    return n;
}

int main() {
    if (strlen("") == 0) printf("PASS strlen_empty\n");
    if (strlen("hello") == 5) printf("PASS strlen_5\n");
    
    if (strcmp("abc", "abc") == 0) printf("PASS strcmp_eq\n");
    if (strcmp("abc", "abd") < 0) printf("PASS strcmp_lt\n");
    
    char buf[32];
    strcpy(buf, "hello");
    if (buf[0] == 'h' && buf[4] == 'o') printf("PASS strcpy\n");
    
    if (strcmp("mississippi", "mississippi") == 0 && 
        char_count("mississippi", 's') == 4) printf("PASS char_count\n");
    
    char s1[64];
    strcpy(s1, "foo");
    strcat(s1, "bar");
    if (strcmp(s1, "foobar") == 0) printf("PASS strcat\n");
    
    if (strstr("the quick brown fox", "quick") != 0) printf("PASS strstr\n");
    
    printf("ALL PASS\n");
    return 0;
}
