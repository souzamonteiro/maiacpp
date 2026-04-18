// ISO/IEC 14882:1998(E) - 26.4 Generalized numeric operations

#ifndef _NUMERIC_
#define _NUMERIC_

#include <functional>

namespace std {

template <class InputIterator, class T>
T accumulate(InputIterator first, InputIterator last, T init);

template <class InputIterator, class T, class BinaryOperation>
T accumulate(InputIterator first, InputIterator last, T init, BinaryOperation binary_op);

template <class InputIterator1, class InputIterator2, class T>
T inner_product(InputIterator1 first1, InputIterator1 last1,
                InputIterator2 first2, T init);

template <class InputIterator1, class InputIterator2, class T,
          class BinaryOperation1, class BinaryOperation2>
T inner_product(InputIterator1 first1, InputIterator1 last1,
                InputIterator2 first2, T init,
                BinaryOperation1 binary_op1, BinaryOperation2 binary_op2);

template <class InputIterator, class OutputIterator>
OutputIterator partial_sum(InputIterator first, InputIterator last,
                           OutputIterator result);

template <class InputIterator, class OutputIterator, class BinaryOperation>
OutputIterator partial_sum(InputIterator first, InputIterator last,
                           OutputIterator result, BinaryOperation binary_op);

template <class InputIterator, class OutputIterator>
OutputIterator adjacent_difference(InputIterator first, InputIterator last,
                                   OutputIterator result);

template <class InputIterator, class OutputIterator, class BinaryOperation>
OutputIterator adjacent_difference(InputIterator first, InputIterator last,
                                   OutputIterator result, BinaryOperation binary_op);

template <class InputIterator, class T>
inline T accumulate(InputIterator first, InputIterator last, T init) {
    for (; first != last; ++first) init = init + *first;
    return init;
}

template <class InputIterator, class T, class BinaryOperation>
inline T accumulate(InputIterator first, InputIterator last, T init, BinaryOperation binary_op) {
    for (; first != last; ++first) init = binary_op(init, *first);
    return init;
}

template <class InputIterator1, class InputIterator2, class T>
inline T inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T init) {
    for (; first1 != last1; ++first1, ++first2) init = init + (*first1 * *first2);
    return init;
}

template <class InputIterator1, class InputIterator2, class T, class BinaryOperation1, class BinaryOperation2>
inline T inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T init,
                       BinaryOperation1 binary_op1, BinaryOperation2 binary_op2) {
    for (; first1 != last1; ++first1, ++first2) init = binary_op1(init, binary_op2(*first1, *first2));
    return init;
}

template <class InputIterator, class OutputIterator>
inline OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator result) {
    if (first == last) return result;
    typename iterator_traits<InputIterator>::value_type sum = *first;
    *result = sum;
    ++first; ++result;
    for (; first != last; ++first, ++result) {
        sum = sum + *first;
        *result = sum;
    }
    return result;
}

template <class InputIterator, class OutputIterator, class BinaryOperation>
inline OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator result, BinaryOperation binary_op) {
    if (first == last) return result;
    typename iterator_traits<InputIterator>::value_type sum = *first;
    *result = sum;
    ++first; ++result;
    for (; first != last; ++first, ++result) {
        sum = binary_op(sum, *first);
        *result = sum;
    }
    return result;
}

template <class InputIterator, class OutputIterator>
inline OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result) {
    if (first == last) return result;
    typename iterator_traits<InputIterator>::value_type prev = *first;
    *result = prev;
    ++first; ++result;
    for (; first != last; ++first, ++result) {
        typename iterator_traits<InputIterator>::value_type value = *first;
        *result = value - prev;
        prev = value;
    }
    return result;
}

template <class InputIterator, class OutputIterator, class BinaryOperation>
inline OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result, BinaryOperation binary_op) {
    if (first == last) return result;
    typename iterator_traits<InputIterator>::value_type prev = *first;
    *result = prev;
    ++first; ++result;
    for (; first != last; ++first, ++result) {
        typename iterator_traits<InputIterator>::value_type value = *first;
        *result = binary_op(value, prev);
        prev = value;
    }
    return result;
}

} // namespace std

#endif