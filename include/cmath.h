// ISO/IEC 14882:1998(E) - 26.5 C Library

#ifndef _CMATH_
#define _CMATH_

namespace std {

double abs(double x);
double acos(double x);
double asin(double x);
double atan(double x);
double atan2(double y, double x);
double ceil(double x);
double cos(double x);
double cosh(double x);
double exp(double x);
double fabs(double x);
double floor(double x);
double fmod(double x, double y);
double frexp(double x, int* exp);
double ldexp(double x, int exp);
double log(double x);
double log10(double x);
double modf(double x, double* intpart);
double pow(double x, double y);
double pow(double x, int y);
double sin(double x);
double sinh(double x);
double sqrt(double x);
double tan(double x);
double tanh(double x);

float abs(float x);
float acos(float x);
float asin(float x);
float atan(float x);
float atan2(float y, float x);
float ceil(float x);
float cos(float x);
float cosh(float x);
float exp(float x);
float fabs(float x);
float floor(float x);
float fmod(float x, float y);
float frexp(float x, int* exp);
float ldexp(float x, int exp);
float log(float x);
float log10(float x);
float modf(float x, float* intpart);
float pow(float x, float y);
float pow(float x, int y);
float sin(float x);
float sinh(float x);
float sqrt(float x);
float tan(float x);
float tanh(float x);

long double abs(long double x);
long double acos(long double x);
long double asin(long double x);
long double atan(long double x);
long double atan2(long double y, long double x);
long double ceil(long double x);
long double cos(long double x);
long double cosh(long double x);
long double exp(long double x);
long double fabs(long double x);
long double floor(long double x);
long double fmod(long double x, long double y);
long double frexp(long double x, int* exp);
long double ldexp(long double x, int exp);
long double log(long double x);
long double log10(long double x);
long double modf(long double x, long double* intpart);
long double pow(long double x, long double y);
long double pow(long double x, int y);
long double sin(long double x);
long double sinh(long double x);
long double sqrt(long double x);
long double tan(long double x);
long double tanh(long double x);

} // namespace std

#define HUGE_VAL /* implementation-defined */

#endif