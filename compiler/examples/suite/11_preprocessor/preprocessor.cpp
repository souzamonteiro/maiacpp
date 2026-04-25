// 11_preprocessor — Exercises a realistic C++98 preprocessing surface.
// This test is intentionally macro-heavy so that missing macro expansion is
// visible in the generated C output and lowering diagnostics.

#include <stdio.h>
#include <string.h>
#include "preprocessor_defs.hpp"

int main() {
    PP_DECLARE_AND_SET(int, joined_value, PP_SUM);
    PP_DECLARE_AND_SET(int, nested_value, PP_ADD(PP_IFDEF_VALUE, PP_IFNDEF_VALUE));
    PP_DECLARE_AND_SET(int, squared_value, PP_SQR(3));
    PP_DECLARE_AND_SET(int, PP_JOIN(local_, var), PP_TEMP_VALUE);

    PP_CHECK_EQ("object_like_sum", joined_value, 42);
    PP_CHECK_EQ("function_like_add", nested_value, 18);
    PP_CHECK_EQ("nested_macro_mul", squared_value, 9);
    PP_CHECK_EQ("token_paste", local_var, 22);
    PP_CHECK_EQ("defined_if", PP_DEFINED_VALUE, 13);

    if (strcmp(PP_STR(PP_GREETING), "PP_GREETING") == 0) {
        printf("PASS stringification_raw\n");
    } else {
        printf("FAIL stringification_raw\n");
    }

    if (strcmp(PP_GREETING, "macro-hello") == 0) {
        printf("PASS object_like_string\n");
    } else {
        printf("FAIL object_like_string\n");
    }

    printf("ALL PASS\n");
    return 0;
}