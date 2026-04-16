// ISO/IEC 14882:1998(E) - 18.2.1 Numeric limits

#ifndef _LIMITS_
#define _LIMITS_

#include <climits>
#include <cfloat>
#include <cwchar>

namespace std {

#define __MAIACPP_DECLARE_INTEGER_LIMITS(Type, MinValue, MaxValue, DigitsValue, Digits10Value, SignedValue, ModuloValue) \
template<> class numeric_limits<Type> { \
public: \
    static const bool is_specialized = true; \
    static Type min() throw() { return (MinValue); } \
    static Type max() throw() { return (MaxValue); } \
    static const int digits = (DigitsValue); \
    static const int digits10 = (Digits10Value); \
    static const bool is_signed = (SignedValue); \
    static const bool is_integer = true; \
    static const bool is_exact = true; \
    static const int radix = 2; \
    static Type epsilon() throw() { return 0; } \
    static Type round_error() throw() { return 0; } \
    static const int min_exponent = 0; \
    static const int min_exponent10 = 0; \
    static const int max_exponent = 0; \
    static const int max_exponent10 = 0; \
    static const bool has_infinity = false; \
    static const bool has_quiet_NaN = false; \
    static const bool has_signaling_NaN = false; \
    static const float_denorm_style has_denorm = denorm_absent; \
    static const bool has_denorm_loss = false; \
    static Type infinity() throw() { return 0; } \
    static Type quiet_NaN() throw() { return 0; } \
    static Type signaling_NaN() throw() { return 0; } \
    static Type denorm_min() throw() { return 0; } \
    static const bool is_iec559 = false; \
    static const bool is_bounded = true; \
    static const bool is_modulo = (ModuloValue); \
    static const bool traps = false; \
    static const bool tinyness_before = false; \
    static const float_round_style round_style = round_toward_zero; \
};

#define __MAIACPP_DECLARE_FLOAT_LIMITS(Type, MinValue, MaxValue, EpsilonValue, DigitsValue, Digits10Value, MantDigitsValue, MinExpValue, MinExp10Value, MaxExpValue, MaxExp10Value) \
template<> class numeric_limits<Type> { \
public: \
    static const bool is_specialized = true; \
    static Type min() throw() { return (MinValue); } \
    static Type max() throw() { return (MaxValue); } \
    static const int digits = (MantDigitsValue); \
    static const int digits10 = (Digits10Value); \
    static const bool is_signed = true; \
    static const bool is_integer = false; \
    static const bool is_exact = false; \
    static const int radix = FLT_RADIX; \
    static Type epsilon() throw() { return (EpsilonValue); } \
    static Type round_error() throw() { return static_cast<Type>(0.5); } \
    static const int min_exponent = (MinExpValue); \
    static const int min_exponent10 = (MinExp10Value); \
    static const int max_exponent = (MaxExpValue); \
    static const int max_exponent10 = (MaxExp10Value); \
    static const bool has_infinity = false; \
    static const bool has_quiet_NaN = false; \
    static const bool has_signaling_NaN = false; \
    static const float_denorm_style has_denorm = denorm_absent; \
    static const bool has_denorm_loss = false; \
    static Type infinity() throw() { return 0; } \
    static Type quiet_NaN() throw() { return 0; } \
    static Type signaling_NaN() throw() { return 0; } \
    static Type denorm_min() throw() { return 0; } \
    static const bool is_iec559 = false; \
    static const bool is_bounded = true; \
    static const bool is_modulo = false; \
    static const bool traps = false; \
    static const bool tinyness_before = false; \
    static const float_round_style round_style = round_to_nearest; \
};

template<class T> class numeric_limits;

enum float_round_style {
    round_indeterminate = -1,
    round_toward_zero = 0,
    round_to_nearest = 1,
    round_toward_infinity = 2,
    round_toward_neg_infinity = 3
};

enum float_denorm_style {
    denorm_indeterminate = -1,
    denorm_absent = 0,
    denorm_present = 1
};

template<> class numeric_limits<bool>;
template<> class numeric_limits<char>;
template<> class numeric_limits<signed char>;
template<> class numeric_limits<unsigned char>;
template<> class numeric_limits<wchar_t>;
template<> class numeric_limits<short>;
template<> class numeric_limits<int>;
template<> class numeric_limits<long>;
template<> class numeric_limits<unsigned short>;
template<> class numeric_limits<unsigned int>;
template<> class numeric_limits<unsigned long>;
template<> class numeric_limits<float>;
template<> class numeric_limits<double>;
template<> class numeric_limits<long double>;

template<class T> class numeric_limits {
public:
    static const bool is_specialized = false;
    static T min() throw();
    static T max() throw();
    static const int digits = 0;
    static const int digits10 = 0;
    static const bool is_signed = false;
    static const bool is_integer = false;
    static const bool is_exact = false;
    static const int radix = 0;
    static T epsilon() throw();
    static T round_error() throw();
    static const int min_exponent = 0;
    static const int min_exponent10 = 0;
    static const int max_exponent = 0;
    static const int max_exponent10 = 0;
    static const bool has_infinity = false;
    static const bool has_quiet_NaN = false;
    static const bool has_signaling_NaN = false;
    static const float_denorm_style has_denorm = denorm_absent;
    static const bool has_denorm_loss = false;
    static T infinity() throw();
    static T quiet_NaN() throw();
    static T signaling_NaN() throw();
    static T denorm_min() throw();
    static const bool is_iec559 = false;
    static const bool is_bounded = false;
    static const bool is_modulo = false;
    static const bool traps = false;
    static const bool tinyness_before = false;
    static const float_round_style round_style = round_toward_zero;
};

template<> class numeric_limits<bool> {
public:
    static const bool is_specialized = true;
    static bool min() throw() { return false; }
    static bool max() throw() { return true; }
    static const int digits = 1;
    static const int digits10 = 0;
    static const bool is_signed = false;
    static const bool is_integer = true;
    static const bool is_exact = true;
    static const int radix = 2;
    static bool epsilon() throw() { return false; }
    static bool round_error() throw() { return false; }
    static const int min_exponent = 0;
    static const int min_exponent10 = 0;
    static const int max_exponent = 0;
    static const int max_exponent10 = 0;
    static const bool has_infinity = false;
    static const bool has_quiet_NaN = false;
    static const bool has_signaling_NaN = false;
    static const float_denorm_style has_denorm = denorm_absent;
    static const bool has_denorm_loss = false;
    static bool infinity() throw() { return false; }
    static bool quiet_NaN() throw() { return false; }
    static bool signaling_NaN() throw() { return false; }
    static bool denorm_min() throw() { return false; }
    static const bool is_iec559 = false;
    static const bool is_bounded = true;
    static const bool is_modulo = false;
    static const bool traps = false;
    static const bool tinyness_before = false;
    static const float_round_style round_style = round_toward_zero;
};

__MAIACPP_DECLARE_INTEGER_LIMITS(char, CHAR_MIN, CHAR_MAX, 7, 2, true, false)
__MAIACPP_DECLARE_INTEGER_LIMITS(signed char, SCHAR_MIN, SCHAR_MAX, 7, 2, true, false)
__MAIACPP_DECLARE_INTEGER_LIMITS(unsigned char, 0, UCHAR_MAX, 8, 2, false, true)
__MAIACPP_DECLARE_INTEGER_LIMITS(wchar_t, WCHAR_MIN, WCHAR_MAX, 31, 9, true, false)
__MAIACPP_DECLARE_INTEGER_LIMITS(short, SHRT_MIN, SHRT_MAX, 15, 4, true, false)
__MAIACPP_DECLARE_INTEGER_LIMITS(int, INT_MIN, INT_MAX, 15, 4, true, false)
__MAIACPP_DECLARE_INTEGER_LIMITS(long, LONG_MIN, LONG_MAX, 31, 9, true, false)
__MAIACPP_DECLARE_INTEGER_LIMITS(unsigned short, 0, USHRT_MAX, 16, 4, false, true)
__MAIACPP_DECLARE_INTEGER_LIMITS(unsigned int, 0, UINT_MAX, 16, 4, false, true)
__MAIACPP_DECLARE_INTEGER_LIMITS(unsigned long, 0, ULONG_MAX, 32, 9, false, true)

__MAIACPP_DECLARE_FLOAT_LIMITS(float, FLT_MIN, FLT_MAX, FLT_EPSILON, FLT_DIG, FLT_DIG, FLT_MANT_DIG, FLT_MIN_EXP, FLT_MIN_10_EXP, FLT_MAX_EXP, FLT_MAX_10_EXP)
__MAIACPP_DECLARE_FLOAT_LIMITS(double, DBL_MIN, DBL_MAX, DBL_EPSILON, DBL_DIG, DBL_DIG, DBL_MANT_DIG, DBL_MIN_EXP, DBL_MIN_10_EXP, DBL_MAX_EXP, DBL_MAX_10_EXP)
__MAIACPP_DECLARE_FLOAT_LIMITS(long double, LDBL_MIN, LDBL_MAX, LDBL_EPSILON, LDBL_DIG, LDBL_DIG, LDBL_MANT_DIG, LDBL_MIN_EXP, LDBL_MIN_10_EXP, LDBL_MAX_EXP, LDBL_MAX_10_EXP)

#undef __MAIACPP_DECLARE_INTEGER_LIMITS
#undef __MAIACPP_DECLARE_FLOAT_LIMITS

} // namespace std

#endif