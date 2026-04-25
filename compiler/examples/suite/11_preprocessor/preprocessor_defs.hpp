#ifndef SUITE_PREPROCESSOR_DEFS_HPP
#define SUITE_PREPROCESSOR_DEFS_HPP

#define PP_BASE 40
#define PP_OFFSET 2
#define PP_SUM (PP_BASE + PP_OFFSET)
#define PP_BOOL_FLAG 1
#define PP_GREETING "macro-hello"

#define PP_ADD(a, b) ((a) + (b))
#define PP_MUL(a, b) ((a) * (b))
#define PP_SQR(x) PP_MUL((x), (x))
#define PP_JOIN(a, b) a##b
#define PP_STR(x) #x

#define PP_DECLARE_AND_SET(type, name, value) \
    type name = (value)

#define PP_CHECK_EQ(label, expr, expected) \
    do { \
        if ((expr) == (expected)) printf("PASS %s\n", label); \
        else printf("FAIL %s\n", label); \
    } while (0)

#ifdef PP_BOOL_FLAG
#define PP_IFDEF_VALUE 7
#else
#define PP_IFDEF_VALUE -7
#endif

#ifndef PP_NOT_DEFINED_YET
#define PP_IFNDEF_VALUE 11
#else
#define PP_IFNDEF_VALUE -11
#endif

#if defined(PP_BOOL_FLAG)
#define PP_DEFINED_VALUE 13
#else
#define PP_DEFINED_VALUE -13
#endif

#undef PP_TEMP_VALUE
#define PP_TEMP_VALUE 21
#undef PP_TEMP_VALUE
#define PP_TEMP_VALUE 22

#endif