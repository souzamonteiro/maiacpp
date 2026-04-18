// ISO/IEC 14882:1998(E) - 25 Algorithms library

#ifndef _ALGORITHM_
#define _ALGORITHM_

#include <iterator>
#include <utility>
#include <functional>
#include <cstdlib>

namespace std {

// 25.1 Non-modifying sequence operations
template<class InputIterator, class Function>
Function for_each(InputIterator first, InputIterator last, Function f);

template<class InputIterator, class T>
InputIterator find(InputIterator first, InputIterator last, const T& value);

template<class InputIterator, class Predicate>
InputIterator find_if(InputIterator first, InputIterator last, Predicate pred);

template<class ForwardIterator1, class ForwardIterator2>
ForwardIterator1 find_end(ForwardIterator1 first1, ForwardIterator1 last1,
                          ForwardIterator2 first2, ForwardIterator2 last2);

template<class ForwardIterator1, class ForwardIterator2, class BinaryPredicate>
ForwardIterator1 find_end(ForwardIterator1 first1, ForwardIterator1 last1,
                          ForwardIterator2 first2, ForwardIterator2 last2,
                          BinaryPredicate pred);

template<class ForwardIterator1, class ForwardIterator2>
ForwardIterator1 find_first_of(ForwardIterator1 first1, ForwardIterator1 last1,
                               ForwardIterator2 first2, ForwardIterator2 last2);

template<class ForwardIterator1, class ForwardIterator2, class BinaryPredicate>
ForwardIterator1 find_first_of(ForwardIterator1 first1, ForwardIterator1 last1,
                               ForwardIterator2 first2, ForwardIterator2 last2,
                               BinaryPredicate pred);

template<class ForwardIterator>
ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last);

template<class ForwardIterator, class BinaryPredicate>
ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last,
                              BinaryPredicate pred);

template<class InputIterator, class T>
typename iterator_traits<InputIterator>::difference_type
count(InputIterator first, InputIterator last, const T& value);

template<class InputIterator, class Predicate>
typename iterator_traits<InputIterator>::difference_type
count_if(InputIterator first, InputIterator last, Predicate pred);

template<class InputIterator1, class InputIterator2>
pair<InputIterator1, InputIterator2>
mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2);

template<class InputIterator1, class InputIterator2, class BinaryPredicate>
pair<InputIterator1, InputIterator2>
mismatch(InputIterator1 first1, InputIterator1 last1,
         InputIterator2 first2, BinaryPredicate pred);

template<class InputIterator1, class InputIterator2>
bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2);

template<class InputIterator1, class InputIterator2, class BinaryPredicate>
bool equal(InputIterator1 first1, InputIterator1 last1,
           InputIterator2 first2, BinaryPredicate pred);

template<class ForwardIterator1, class ForwardIterator2>
ForwardIterator1 search(ForwardIterator1 first1, ForwardIterator1 last1,
                        ForwardIterator2 first2, ForwardIterator2 last2);

template<class ForwardIterator1, class ForwardIterator2, class BinaryPredicate>
ForwardIterator1 search(ForwardIterator1 first1, ForwardIterator1 last1,
                        ForwardIterator2 first2, ForwardIterator2 last2,
                        BinaryPredicate pred);

template<class ForwardIterator, class Size, class T>
ForwardIterator search_n(ForwardIterator first, ForwardIterator last,
                         Size count, const T& value);

template<class ForwardIterator, class Size, class T, class BinaryPredicate>
ForwardIterator search_n(ForwardIterator first, ForwardIterator last,
                         Size count, const T& value, BinaryPredicate pred);

// 25.2 Mutating sequence operations
template<class InputIterator, class OutputIterator>
OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result);

template<class BidirectionalIterator1, class BidirectionalIterator2>
BidirectionalIterator2 copy_backward(BidirectionalIterator1 first,
                                     BidirectionalIterator1 last,
                                     BidirectionalIterator2 result);

template<class T> void swap(T& a, T& b);

template<class ForwardIterator1, class ForwardIterator2>
ForwardIterator2 swap_ranges(ForwardIterator1 first1, ForwardIterator1 last1,
                             ForwardIterator2 first2);

template<class ForwardIterator1, class ForwardIterator2>
void iter_swap(ForwardIterator1 a, ForwardIterator2 b);

template<class InputIterator, class OutputIterator, class UnaryOperation>
OutputIterator transform(InputIterator first, InputIterator last,
                         OutputIterator result, UnaryOperation op);

template<class InputIterator1, class InputIterator2, class OutputIterator,
         class BinaryOperation>
OutputIterator transform(InputIterator1 first1, InputIterator1 last1,
                         InputIterator2 first2, OutputIterator result,
                         BinaryOperation binary_op);

template<class ForwardIterator, class T>
void replace(ForwardIterator first, ForwardIterator last,
             const T& old_value, const T& new_value);

template<class ForwardIterator, class Predicate, class T>
void replace_if(ForwardIterator first, ForwardIterator last,
                Predicate pred, const T& new_value);

template<class InputIterator, class OutputIterator, class T>
OutputIterator replace_copy(InputIterator first, InputIterator last,
                            OutputIterator result,
                            const T& old_value, const T& new_value);

template<class Iterator, class OutputIterator, class Predicate, class T>
OutputIterator replace_copy_if(Iterator first, Iterator last,
                               OutputIterator result,
                               Predicate pred, const T& new_value);

template<class ForwardIterator, class T>
void fill(ForwardIterator first, ForwardIterator last, const T& value);

template<class OutputIterator, class Size, class T>
void fill_n(OutputIterator first, Size n, const T& value);

template<class ForwardIterator, class Generator>
void generate(ForwardIterator first, ForwardIterator last, Generator gen);

template<class OutputIterator, class Size, class Generator>
void generate_n(OutputIterator first, Size n, Generator gen);

template<class ForwardIterator, class T>
ForwardIterator remove(ForwardIterator first, ForwardIterator last, const T& value);

template<class ForwardIterator, class Predicate>
ForwardIterator remove_if(ForwardIterator first, ForwardIterator last, Predicate pred);

template<class InputIterator, class OutputIterator, class T>
OutputIterator remove_copy(InputIterator first, InputIterator last,
                           OutputIterator result, const T& value);

template<class InputIterator, class OutputIterator, class Predicate>
OutputIterator remove_copy_if(InputIterator first, InputIterator last,
                              OutputIterator result, Predicate pred);

template<class ForwardIterator>
ForwardIterator unique(ForwardIterator first, ForwardIterator last);

template<class ForwardIterator, class BinaryPredicate>
ForwardIterator unique(ForwardIterator first, ForwardIterator last,
                       BinaryPredicate pred);

template<class InputIterator, class OutputIterator>
OutputIterator unique_copy(InputIterator first, InputIterator last,
                           OutputIterator result);

template<class InputIterator, class OutputIterator, class BinaryPredicate>
OutputIterator unique_copy(InputIterator first, InputIterator last,
                           OutputIterator result, BinaryPredicate pred);

template<class BidirectionalIterator>
void reverse(BidirectionalIterator first, BidirectionalIterator last);

template<class BidirectionalIterator, class OutputIterator>
OutputIterator reverse_copy(BidirectionalIterator first, BidirectionalIterator last,
                            OutputIterator result);

template<class ForwardIterator>
void rotate(ForwardIterator first, ForwardIterator middle, ForwardIterator last);

template<class ForwardIterator, class OutputIterator>
OutputIterator rotate_copy(ForwardIterator first, ForwardIterator middle,
                           ForwardIterator last, OutputIterator result);

template<class RandomAccessIterator>
void random_shuffle(RandomAccessIterator first, RandomAccessIterator last);

template<class RandomAccessIterator, class RandomNumberGenerator>
void random_shuffle(RandomAccessIterator first, RandomAccessIterator last,
                    RandomNumberGenerator& rand);

template<class BidirectionalIterator, class Predicate>
BidirectionalIterator partition(BidirectionalIterator first,
                                BidirectionalIterator last, Predicate pred);

template<class BidirectionalIterator, class Predicate>
BidirectionalIterator stable_partition(BidirectionalIterator first,
                                       BidirectionalIterator last, Predicate pred);

// 25.3 Sorting and related operations
template<class RandomAccessIterator>
void sort(RandomAccessIterator first, RandomAccessIterator last);

template<class RandomAccessIterator, class Compare>
void sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp);

template<class RandomAccessIterator>
void stable_sort(RandomAccessIterator first, RandomAccessIterator last);

template<class RandomAccessIterator, class Compare>
void stable_sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp);

template<class RandomAccessIterator>
void partial_sort(RandomAccessIterator first, RandomAccessIterator middle,
                  RandomAccessIterator last);

template<class RandomAccessIterator, class Compare>
void partial_sort(RandomAccessIterator first, RandomAccessIterator middle,
                  RandomAccessIterator last, Compare comp);

template<class InputIterator, class RandomAccessIterator>
RandomAccessIterator partial_sort_copy(InputIterator first, InputIterator last,
                                       RandomAccessIterator result_first,
                                       RandomAccessIterator result_last);

template<class InputIterator, class RandomAccessIterator, class Compare>
RandomAccessIterator partial_sort_copy(InputIterator first, InputIterator last,
                                       RandomAccessIterator result_first,
                                       RandomAccessIterator result_last,
                                       Compare comp);

template<class RandomAccessIterator>
void nth_element(RandomAccessIterator first, RandomAccessIterator nth,
                 RandomAccessIterator last);

template<class RandomAccessIterator, class Compare>
void nth_element(RandomAccessIterator first, RandomAccessIterator nth,
                 RandomAccessIterator last, Compare comp);

template<class ForwardIterator, class T>
ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last,
                            const T& value);

template<class ForwardIterator, class T, class Compare>
ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last,
                            const T& value, Compare comp);

template<class ForwardIterator, class T>
ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last,
                            const T& value);

template<class ForwardIterator, class T, class Compare>
ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last,
                            const T& value, Compare comp);

template<class ForwardIterator, class T>
pair<ForwardIterator, ForwardIterator>
equal_range(ForwardIterator first, ForwardIterator last, const T& value);

template<class ForwardIterator, class T, class Compare>
pair<ForwardIterator, ForwardIterator>
equal_range(ForwardIterator first, ForwardIterator last,
            const T& value, Compare comp);

template<class ForwardIterator, class T>
bool binary_search(ForwardIterator first, ForwardIterator last, const T& value);

template<class ForwardIterator, class T, class Compare>
bool binary_search(ForwardIterator first, ForwardIterator last,
                   const T& value, Compare comp);

template<class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator merge(InputIterator1 first1, InputIterator1 last1,
                     InputIterator2 first2, InputIterator2 last2,
                     OutputIterator result);

template<class InputIterator1, class InputIterator2, class OutputIterator,
         class Compare>
OutputIterator merge(InputIterator1 first1, InputIterator1 last1,
                     InputIterator2 first2, InputIterator2 last2,
                     OutputIterator result, Compare comp);

template<class BidirectionalIterator>
void inplace_merge(BidirectionalIterator first, BidirectionalIterator middle,
                   BidirectionalIterator last);

template<class BidirectionalIterator, class Compare>
void inplace_merge(BidirectionalIterator first, BidirectionalIterator middle,
                   BidirectionalIterator last, Compare comp);

template<class InputIterator1, class InputIterator2>
bool includes(InputIterator1 first1, InputIterator1 last1,
              InputIterator2 first2, InputIterator2 last2);

template<class InputIterator1, class InputIterator2, class Compare>
bool includes(InputIterator1 first1, InputIterator1 last1,
              InputIterator2 first2, InputIterator2 last2, Compare comp);

template<class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_union(InputIterator1 first1, InputIterator1 last1,
                         InputIterator2 first2, InputIterator2 last2,
                         OutputIterator result);

template<class InputIterator1, class InputIterator2, class OutputIterator,
         class Compare>
OutputIterator set_union(InputIterator1 first1, InputIterator1 last1,
                         InputIterator2 first2, InputIterator2 last2,
                         OutputIterator result, Compare comp);

template<class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_intersection(InputIterator1 first1, InputIterator1 last1,
                                InputIterator2 first2, InputIterator2 last2,
                                OutputIterator result);

template<class InputIterator1, class InputIterator2, class OutputIterator,
         class Compare>
OutputIterator set_intersection(InputIterator1 first1, InputIterator1 last1,
                                InputIterator2 first2, InputIterator2 last2,
                                OutputIterator result, Compare comp);

template<class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1,
                              InputIterator2 first2, InputIterator2 last2,
                              OutputIterator result);

template<class InputIterator1, class InputIterator2, class OutputIterator,
         class Compare>
OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1,
                              InputIterator2 first2, InputIterator2 last2,
                              OutputIterator result, Compare comp);

template<class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_symmetric_difference(InputIterator1 first1, InputIterator1 last1,
                                        InputIterator2 first2, InputIterator2 last2,
                                        OutputIterator result);

template<class InputIterator1, class InputIterator2, class OutputIterator,
         class Compare>
OutputIterator set_symmetric_difference(InputIterator1 first1, InputIterator1 last1,
                                        InputIterator2 first2, InputIterator2 last2,
                                        OutputIterator result, Compare comp);

template<class RandomAccessIterator>
void push_heap(RandomAccessIterator first, RandomAccessIterator last);

template<class RandomAccessIterator, class Compare>
void push_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp);

template<class RandomAccessIterator>
void pop_heap(RandomAccessIterator first, RandomAccessIterator last);

template<class RandomAccessIterator, class Compare>
void pop_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp);

template<class RandomAccessIterator>
void make_heap(RandomAccessIterator first, RandomAccessIterator last);

template<class RandomAccessIterator, class Compare>
void make_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp);

template<class RandomAccessIterator>
void sort_heap(RandomAccessIterator first, RandomAccessIterator last);

template<class RandomAccessIterator, class Compare>
void sort_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp);

template<class T> const T& min(const T& a, const T& b);
template<class T, class Compare> const T& min(const T& a, const T& b, Compare comp);
template<class T> const T& max(const T& a, const T& b);
template<class T, class Compare> const T& max(const T& a, const T& b, Compare comp);

template<class ForwardIterator>
ForwardIterator min_element(ForwardIterator first, ForwardIterator last);

template<class ForwardIterator, class Compare>
ForwardIterator min_element(ForwardIterator first, ForwardIterator last, Compare comp);

template<class ForwardIterator>
ForwardIterator max_element(ForwardIterator first, ForwardIterator last);

template<class ForwardIterator, class Compare>
ForwardIterator max_element(ForwardIterator first, ForwardIterator last, Compare comp);

template<class InputIterator1, class InputIterator2>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
                             InputIterator2 first2, InputIterator2 last2);

template<class InputIterator1, class InputIterator2, class Compare>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
                             InputIterator2 first2, InputIterator2 last2,
                             Compare comp);

template<class BidirectionalIterator>
bool next_permutation(BidirectionalIterator first, BidirectionalIterator last);

template<class BidirectionalIterator, class Compare>
bool next_permutation(BidirectionalIterator first, BidirectionalIterator last,
                      Compare comp);

template<class BidirectionalIterator>
bool prev_permutation(BidirectionalIterator first, BidirectionalIterator last);

template<class BidirectionalIterator, class Compare>
bool prev_permutation(BidirectionalIterator first, BidirectionalIterator last,
                      Compare comp);

namespace __maiacpp_algorithm_detail {

template<class Iterator, class Compare>
inline void __insertion_sort(Iterator first, Iterator last, Compare comp) {
    if (first == last) return;
    for (Iterator it = first + 1; it != last; ++it) {
        typename iterator_traits<Iterator>::value_type value = *it;
        Iterator pos = it;
        while (pos != first) {
            Iterator prev = pos - 1;
            if (!comp(value, *prev)) break;
            *pos = *prev;
            pos = prev;
        }
        *pos = value;
    }
}

template<class RandomAccessIterator, class Compare>
inline void __push_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
    if (last - first < 2) return;
    typename iterator_traits<RandomAccessIterator>::difference_type child = (last - first) - 1;
    while (child > 0) {
        typename iterator_traits<RandomAccessIterator>::difference_type parent = (child - 1) / 2;
        if (!comp(*(first + parent), *(first + child))) break;
        std::iter_swap(first + parent, first + child);
        child = parent;
    }
}

template<class RandomAccessIterator, class Compare>
inline void __adjust_heap(RandomAccessIterator first, RandomAccessIterator last,
                          typename iterator_traits<RandomAccessIterator>::difference_type parent,
                          Compare comp) {
    typename iterator_traits<RandomAccessIterator>::difference_type length = last - first;
    while (true) {
        typename iterator_traits<RandomAccessIterator>::difference_type left = parent * 2 + 1;
        typename iterator_traits<RandomAccessIterator>::difference_type right = left + 1;
        typename iterator_traits<RandomAccessIterator>::difference_type best = parent;
        if (left < length && comp(*(first + best), *(first + left))) best = left;
        if (right < length && comp(*(first + best), *(first + right))) best = right;
        if (best == parent) break;
        std::iter_swap(first + parent, first + best);
        parent = best;
    }
}

template<class T>
struct __identity_less {
    bool operator()(const T& a, const T& b) const { return a < b; }
};

} // namespace __maiacpp_algorithm_detail

template<class InputIterator, class Function>
inline Function for_each(InputIterator first, InputIterator last, Function f) {
    for (; first != last; ++first) f(*first);
    return f;
}

template<class InputIterator, class T>
inline InputIterator find(InputIterator first, InputIterator last, const T& value) {
    for (; first != last; ++first) if (*first == value) break;
    return first;
}

template<class InputIterator, class Predicate>
inline InputIterator find_if(InputIterator first, InputIterator last, Predicate pred) {
    for (; first != last; ++first) if (pred(*first)) break;
    return first;
}

template<class ForwardIterator1, class ForwardIterator2>
inline ForwardIterator1 find_end(ForwardIterator1 first1, ForwardIterator1 last1,
                                 ForwardIterator2 first2, ForwardIterator2 last2) {
    if (first2 == last2) return last1;
    ForwardIterator1 result = last1;
    for (; first1 != last1; ++first1) {
        ForwardIterator1 it1 = first1;
        ForwardIterator2 it2 = first2;
        while (it1 != last1 && it2 != last2 && *it1 == *it2) { ++it1; ++it2; }
        if (it2 == last2) result = first1;
    }
    return result;
}

template<class ForwardIterator1, class ForwardIterator2, class BinaryPredicate>
inline ForwardIterator1 find_end(ForwardIterator1 first1, ForwardIterator1 last1,
                                 ForwardIterator2 first2, ForwardIterator2 last2,
                                 BinaryPredicate pred) {
    if (first2 == last2) return last1;
    ForwardIterator1 result = last1;
    for (; first1 != last1; ++first1) {
        ForwardIterator1 it1 = first1;
        ForwardIterator2 it2 = first2;
        while (it1 != last1 && it2 != last2 && pred(*it1, *it2)) { ++it1; ++it2; }
        if (it2 == last2) result = first1;
    }
    return result;
}

template<class ForwardIterator1, class ForwardIterator2>
inline ForwardIterator1 find_first_of(ForwardIterator1 first1, ForwardIterator1 last1,
                                      ForwardIterator2 first2, ForwardIterator2 last2) {
    for (; first1 != last1; ++first1)
        for (ForwardIterator2 it = first2; it != last2; ++it)
            if (*first1 == *it) return first1;
    return last1;
}

template<class ForwardIterator1, class ForwardIterator2, class BinaryPredicate>
inline ForwardIterator1 find_first_of(ForwardIterator1 first1, ForwardIterator1 last1,
                                      ForwardIterator2 first2, ForwardIterator2 last2,
                                      BinaryPredicate pred) {
    for (; first1 != last1; ++first1)
        for (ForwardIterator2 it = first2; it != last2; ++it)
            if (pred(*first1, *it)) return first1;
    return last1;
}

template<class ForwardIterator>
inline ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last) {
    if (first == last) return last;
    ForwardIterator next = first;
    for (++next; next != last; ++first, ++next)
        if (*first == *next) return first;
    return last;
}

template<class ForwardIterator, class BinaryPredicate>
inline ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last, BinaryPredicate pred) {
    if (first == last) return last;
    ForwardIterator next = first;
    for (++next; next != last; ++first, ++next)
        if (pred(*first, *next)) return first;
    return last;
}

template<class InputIterator, class T>
inline typename iterator_traits<InputIterator>::difference_type
count(InputIterator first, InputIterator last, const T& value) {
    typename iterator_traits<InputIterator>::difference_type n = 0;
    for (; first != last; ++first) if (*first == value) ++n;
    return n;
}

template<class InputIterator, class Predicate>
inline typename iterator_traits<InputIterator>::difference_type
count_if(InputIterator first, InputIterator last, Predicate pred) {
    typename iterator_traits<InputIterator>::difference_type n = 0;
    for (; first != last; ++first) if (pred(*first)) ++n;
    return n;
}

template<class InputIterator1, class InputIterator2>
inline pair<InputIterator1, InputIterator2>
mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) {
    while (first1 != last1 && *first1 == *first2) { ++first1; ++first2; }
    return pair<InputIterator1, InputIterator2>(first1, first2);
}

template<class InputIterator1, class InputIterator2, class BinaryPredicate>
inline pair<InputIterator1, InputIterator2>
mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, BinaryPredicate pred) {
    while (first1 != last1 && pred(*first1, *first2)) { ++first1; ++first2; }
    return pair<InputIterator1, InputIterator2>(first1, first2);
}

template<class InputIterator1, class InputIterator2>
inline bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) {
    for (; first1 != last1; ++first1, ++first2) if (!(*first1 == *first2)) return false;
    return true;
}

template<class InputIterator1, class InputIterator2, class BinaryPredicate>
inline bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, BinaryPredicate pred) {
    for (; first1 != last1; ++first1, ++first2) if (!pred(*first1, *first2)) return false;
    return true;
}

template<class ForwardIterator1, class ForwardIterator2>
inline ForwardIterator1 search(ForwardIterator1 first1, ForwardIterator1 last1,
                               ForwardIterator2 first2, ForwardIterator2 last2) {
    if (first2 == last2) return first1;
    for (; first1 != last1; ++first1) {
        ForwardIterator1 it1 = first1;
        ForwardIterator2 it2 = first2;
        while (it1 != last1 && it2 != last2 && *it1 == *it2) { ++it1; ++it2; }
        if (it2 == last2) return first1;
        if (it1 == last1) return last1;
    }
    return last1;
}

template<class ForwardIterator1, class ForwardIterator2, class BinaryPredicate>
inline ForwardIterator1 search(ForwardIterator1 first1, ForwardIterator1 last1,
                               ForwardIterator2 first2, ForwardIterator2 last2,
                               BinaryPredicate pred) {
    if (first2 == last2) return first1;
    for (; first1 != last1; ++first1) {
        ForwardIterator1 it1 = first1;
        ForwardIterator2 it2 = first2;
        while (it1 != last1 && it2 != last2 && pred(*it1, *it2)) { ++it1; ++it2; }
        if (it2 == last2) return first1;
        if (it1 == last1) return last1;
    }
    return last1;
}

template<class ForwardIterator, class Size, class T>
inline ForwardIterator search_n(ForwardIterator first, ForwardIterator last, Size count, const T& value) {
    if (count <= 0) return first;
    while (first != last) {
        if (*first == value) {
            ForwardIterator it = first;
            Size n = 0;
            while (it != last && n < count && *it == value) { ++it; ++n; }
            if (n == count) return first;
            if (it == last) return last;
            first = it;
        } else {
            ++first;
        }
    }
    return last;
}

template<class ForwardIterator, class Size, class T, class BinaryPredicate>
inline ForwardIterator search_n(ForwardIterator first, ForwardIterator last, Size count, const T& value, BinaryPredicate pred) {
    if (count <= 0) return first;
    while (first != last) {
        if (pred(*first, value)) {
            ForwardIterator it = first;
            Size n = 0;
            while (it != last && n < count && pred(*it, value)) { ++it; ++n; }
            if (n == count) return first;
            if (it == last) return last;
            first = it;
        } else {
            ++first;
        }
    }
    return last;
}

template<class InputIterator, class OutputIterator>
inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result) {
    for (; first != last; ++first, ++result) *result = *first;
    return result;
}

template<class BidirectionalIterator1, class BidirectionalIterator2>
inline BidirectionalIterator2 copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result) {
    while (first != last) { --last; --result; *result = *last; }
    return result;
}

template<class T>
inline void swap(T& a, T& b) { T tmp = a; a = b; b = tmp; }

template<class ForwardIterator1, class ForwardIterator2>
inline ForwardIterator2 swap_ranges(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2) {
    for (; first1 != last1; ++first1, ++first2) std::iter_swap(first1, first2);
    return first2;
}

template<class ForwardIterator1, class ForwardIterator2>
inline void iter_swap(ForwardIterator1 a, ForwardIterator2 b) { std::swap(*a, *b); }

template<class InputIterator, class OutputIterator, class UnaryOperation>
inline OutputIterator transform(InputIterator first, InputIterator last, OutputIterator result, UnaryOperation op) {
    for (; first != last; ++first, ++result) *result = op(*first);
    return result;
}

template<class InputIterator1, class InputIterator2, class OutputIterator, class BinaryOperation>
inline OutputIterator transform(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, OutputIterator result, BinaryOperation binary_op) {
    for (; first1 != last1; ++first1, ++first2, ++result) *result = binary_op(*first1, *first2);
    return result;
}

template<class ForwardIterator, class T>
inline void replace(ForwardIterator first, ForwardIterator last, const T& old_value, const T& new_value) {
    for (; first != last; ++first) if (*first == old_value) *first = new_value;
}

template<class ForwardIterator, class Predicate, class T>
inline void replace_if(ForwardIterator first, ForwardIterator last, Predicate pred, const T& new_value) {
    for (; first != last; ++first) if (pred(*first)) *first = new_value;
}

template<class InputIterator, class OutputIterator, class T>
inline OutputIterator replace_copy(InputIterator first, InputIterator last, OutputIterator result, const T& old_value, const T& new_value) {
    for (; first != last; ++first, ++result) *result = (*first == old_value ? new_value : *first);
    return result;
}

template<class Iterator, class OutputIterator, class Predicate, class T>
inline OutputIterator replace_copy_if(Iterator first, Iterator last, OutputIterator result, Predicate pred, const T& new_value) {
    for (; first != last; ++first, ++result) *result = (pred(*first) ? new_value : *first);
    return result;
}

template<class ForwardIterator, class T>
inline void fill(ForwardIterator first, ForwardIterator last, const T& value) {
    for (; first != last; ++first) *first = value;
}

template<class OutputIterator, class Size, class T>
inline void fill_n(OutputIterator first, Size n, const T& value) {
    for (; n > 0; --n, ++first) *first = value;
}

template<class ForwardIterator, class Generator>
inline void generate(ForwardIterator first, ForwardIterator last, Generator gen) {
    for (; first != last; ++first) *first = gen();
}

template<class OutputIterator, class Size, class Generator>
inline void generate_n(OutputIterator first, Size n, Generator gen) {
    for (; n > 0; --n, ++first) *first = gen();
}

template<class ForwardIterator, class T>
inline ForwardIterator remove(ForwardIterator first, ForwardIterator last, const T& value) {
    first = std::find(first, last, value);
    if (first == last) return last;
    ForwardIterator result = first;
    ++first;
    for (; first != last; ++first) if (!(*first == value)) { *result = *first; ++result; }
    return result;
}

template<class ForwardIterator, class Predicate>
inline ForwardIterator remove_if(ForwardIterator first, ForwardIterator last, Predicate pred) {
    while (first != last && !pred(*first)) ++first;
    if (first == last) return last;
    ForwardIterator result = first;
    ++first;
    for (; first != last; ++first) if (!pred(*first)) { *result = *first; ++result; }
    return result;
}

template<class InputIterator, class OutputIterator, class T>
inline OutputIterator remove_copy(InputIterator first, InputIterator last, OutputIterator result, const T& value) {
    for (; first != last; ++first) if (!(*first == value)) { *result = *first; ++result; }
    return result;
}

template<class InputIterator, class OutputIterator, class Predicate>
inline OutputIterator remove_copy_if(InputIterator first, InputIterator last, OutputIterator result, Predicate pred) {
    for (; first != last; ++first) if (!pred(*first)) { *result = *first; ++result; }
    return result;
}

template<class ForwardIterator>
inline ForwardIterator unique(ForwardIterator first, ForwardIterator last) {
    if (first == last) return last;
    ForwardIterator result = first;
    while (++first != last) if (!(*result == *first)) *(++result) = *first;
    return ++result;
}

template<class ForwardIterator, class BinaryPredicate>
inline ForwardIterator unique(ForwardIterator first, ForwardIterator last, BinaryPredicate pred) {
    if (first == last) return last;
    ForwardIterator result = first;
    while (++first != last) if (!pred(*result, *first)) *(++result) = *first;
    return ++result;
}

template<class InputIterator, class OutputIterator>
inline OutputIterator unique_copy(InputIterator first, InputIterator last, OutputIterator result) {
    if (first == last) return result;
    typename iterator_traits<InputIterator>::value_type value = *first;
    *result = value; ++result; ++first;
    for (; first != last; ++first) if (!(value == *first)) { value = *first; *result = value; ++result; }
    return result;
}

template<class InputIterator, class OutputIterator, class BinaryPredicate>
inline OutputIterator unique_copy(InputIterator first, InputIterator last, OutputIterator result, BinaryPredicate pred) {
    if (first == last) return result;
    typename iterator_traits<InputIterator>::value_type value = *first;
    *result = value; ++result; ++first;
    for (; first != last; ++first) if (!pred(value, *first)) { value = *first; *result = value; ++result; }
    return result;
}

template<class BidirectionalIterator>
inline void reverse(BidirectionalIterator first, BidirectionalIterator last) {
    while (first != last) {
        --last;
        if (first == last) break;
        std::iter_swap(first, last);
        ++first;
    }
}

template<class BidirectionalIterator, class OutputIterator>
inline OutputIterator reverse_copy(BidirectionalIterator first, BidirectionalIterator last, OutputIterator result) {
    while (first != last) { --last; *result = *last; ++result; }
    return result;
}

template<class ForwardIterator>
inline void rotate(ForwardIterator first, ForwardIterator middle, ForwardIterator last) {
    if (first == middle || middle == last) return;
    ForwardIterator first2 = middle;
    do {
        std::iter_swap(first, first2);
        ++first;
        ++first2;
        if (first == middle) middle = first2;
    } while (first2 != last);
    ForwardIterator new_middle = first;
    first2 = middle;
    while (first2 != last) {
        std::iter_swap(first, first2);
        ++first;
        ++first2;
        if (first == middle) middle = first2;
        else if (first2 == last) first2 = middle;
    }
    (void)new_middle;
}

template<class ForwardIterator, class OutputIterator>
inline OutputIterator rotate_copy(ForwardIterator first, ForwardIterator middle, ForwardIterator last, OutputIterator result) {
    result = std::copy(middle, last, result);
    return std::copy(first, middle, result);
}

template<class RandomAccessIterator>
inline void random_shuffle(RandomAccessIterator first, RandomAccessIterator last) {
    if (first == last) return;
    for (RandomAccessIterator it = first + 1; it != last; ++it) {
        typename iterator_traits<RandomAccessIterator>::difference_type n = (it - first) + 1;
        typename iterator_traits<RandomAccessIterator>::difference_type j = std::rand() % n;
        std::iter_swap(it, first + j);
    }
}

template<class RandomAccessIterator, class RandomNumberGenerator>
inline void random_shuffle(RandomAccessIterator first, RandomAccessIterator last, RandomNumberGenerator& rand) {
    if (first == last) return;
    for (RandomAccessIterator it = first + 1; it != last; ++it) {
        typename iterator_traits<RandomAccessIterator>::difference_type n = (it - first) + 1;
        std::iter_swap(it, first + (rand(n)));
    }
}

template<class BidirectionalIterator, class Predicate>
inline BidirectionalIterator partition(BidirectionalIterator first, BidirectionalIterator last, Predicate pred) {
    while (true) {
        while (first != last && pred(*first)) ++first;
        if (first == last) return first;
        do {
            --last;
            if (first == last) return first;
        } while (!pred(*last));
        std::iter_swap(first, last);
        ++first;
    }
}

template<class BidirectionalIterator, class Predicate>
inline BidirectionalIterator stable_partition(BidirectionalIterator first, BidirectionalIterator last, Predicate pred) {
    BidirectionalIterator result = first;
    while (result != last && pred(*result)) ++result;
    if (result == last) return last;
    for (BidirectionalIterator it = result; it != last; ++it) {
        if (pred(*it)) {
            typename iterator_traits<BidirectionalIterator>::value_type value = *it;
            BidirectionalIterator pos = it;
            while (pos != result) {
                BidirectionalIterator prev = pos;
                --prev;
                *pos = *prev;
                pos = prev;
            }
            *result = value;
            ++result;
        }
    }
    return result;
}

template<class RandomAccessIterator>
inline void sort(RandomAccessIterator first, RandomAccessIterator last) {
    typedef typename iterator_traits<RandomAccessIterator>::value_type value_type;
    __maiacpp_algorithm_detail::__insertion_sort(first, last, __maiacpp_algorithm_detail::__identity_less<value_type>());
}

template<class RandomAccessIterator, class Compare>
inline void sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
    __maiacpp_algorithm_detail::__insertion_sort(first, last, comp);
}

template<class RandomAccessIterator>
inline void stable_sort(RandomAccessIterator first, RandomAccessIterator last) { std::sort(first, last); }

template<class RandomAccessIterator, class Compare>
inline void stable_sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp) { std::sort(first, last, comp); }

template<class RandomAccessIterator>
inline void partial_sort(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last) {
    std::sort(first, last);
    (void)middle;
}

template<class RandomAccessIterator, class Compare>
inline void partial_sort(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last, Compare comp) {
    std::sort(first, last, comp);
    (void)middle;
}

template<class InputIterator, class RandomAccessIterator>
inline RandomAccessIterator partial_sort_copy(InputIterator first, InputIterator last, RandomAccessIterator result_first, RandomAccessIterator result_last) {
    RandomAccessIterator out = result_first;
    for (; first != last && out != result_last; ++first, ++out) *out = *first;
    std::sort(result_first, out);
    for (; first != last; ++first) {
        if (*first < *(out - 1)) {
            *(out - 1) = *first;
            std::sort(result_first, out);
        }
    }
    return out;
}

template<class InputIterator, class RandomAccessIterator, class Compare>
inline RandomAccessIterator partial_sort_copy(InputIterator first, InputIterator last, RandomAccessIterator result_first, RandomAccessIterator result_last, Compare comp) {
    RandomAccessIterator out = result_first;
    for (; first != last && out != result_last; ++first, ++out) *out = *first;
    std::sort(result_first, out, comp);
    for (; first != last; ++first) {
        if (comp(*first, *(out - 1))) {
            *(out - 1) = *first;
            std::sort(result_first, out, comp);
        }
    }
    return out;
}

template<class RandomAccessIterator>
inline void nth_element(RandomAccessIterator first, RandomAccessIterator nth, RandomAccessIterator last) {
    std::sort(first, last);
    (void)nth;
}

template<class RandomAccessIterator, class Compare>
inline void nth_element(RandomAccessIterator first, RandomAccessIterator nth, RandomAccessIterator last, Compare comp) {
    std::sort(first, last, comp);
    (void)nth;
}

template<class ForwardIterator, class T>
inline ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const T& value) {
    typename iterator_traits<ForwardIterator>::difference_type len = std::distance(first, last);
    while (len > 0) {
        typename iterator_traits<ForwardIterator>::difference_type half = len / 2;
        ForwardIterator mid = first;
        std::advance(mid, half);
        if (*mid < value) { first = mid; ++first; len -= half + 1; }
        else len = half;
    }
    return first;
}

template<class ForwardIterator, class T, class Compare>
inline ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const T& value, Compare comp) {
    typename iterator_traits<ForwardIterator>::difference_type len = std::distance(first, last);
    while (len > 0) {
        typename iterator_traits<ForwardIterator>::difference_type half = len / 2;
        ForwardIterator mid = first;
        std::advance(mid, half);
        if (comp(*mid, value)) { first = mid; ++first; len -= half + 1; }
        else len = half;
    }
    return first;
}

template<class ForwardIterator, class T>
inline ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last, const T& value) {
    typename iterator_traits<ForwardIterator>::difference_type len = std::distance(first, last);
    while (len > 0) {
        typename iterator_traits<ForwardIterator>::difference_type half = len / 2;
        ForwardIterator mid = first;
        std::advance(mid, half);
        if (!(value < *mid)) { first = mid; ++first; len -= half + 1; }
        else len = half;
    }
    return first;
}

template<class ForwardIterator, class T, class Compare>
inline ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last, const T& value, Compare comp) {
    typename iterator_traits<ForwardIterator>::difference_type len = std::distance(first, last);
    while (len > 0) {
        typename iterator_traits<ForwardIterator>::difference_type half = len / 2;
        ForwardIterator mid = first;
        std::advance(mid, half);
        if (!comp(value, *mid)) { first = mid; ++first; len -= half + 1; }
        else len = half;
    }
    return first;
}

template<class ForwardIterator, class T>
inline pair<ForwardIterator, ForwardIterator>
equal_range(ForwardIterator first, ForwardIterator last, const T& value) {
    return pair<ForwardIterator, ForwardIterator>(std::lower_bound(first, last, value), std::upper_bound(first, last, value));
}

template<class ForwardIterator, class T, class Compare>
inline pair<ForwardIterator, ForwardIterator>
equal_range(ForwardIterator first, ForwardIterator last, const T& value, Compare comp) {
    return pair<ForwardIterator, ForwardIterator>(std::lower_bound(first, last, value, comp), std::upper_bound(first, last, value, comp));
}

template<class ForwardIterator, class T>
inline bool binary_search(ForwardIterator first, ForwardIterator last, const T& value) {
    ForwardIterator it = std::lower_bound(first, last, value);
    return it != last && !(value < *it);
}

template<class ForwardIterator, class T, class Compare>
inline bool binary_search(ForwardIterator first, ForwardIterator last, const T& value, Compare comp) {
    ForwardIterator it = std::lower_bound(first, last, value, comp);
    return it != last && !comp(value, *it);
}

template<class InputIterator1, class InputIterator2, class OutputIterator>
inline OutputIterator merge(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
    while (first1 != last1 && first2 != last2) {
        if (*first2 < *first1) *result++ = *first2++;
        else *result++ = *first1++;
    }
    result = std::copy(first1, last1, result);
    return std::copy(first2, last2, result);
}

template<class InputIterator1, class InputIterator2, class OutputIterator, class Compare>
inline OutputIterator merge(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result, Compare comp) {
    while (first1 != last1 && first2 != last2) {
        if (comp(*first2, *first1)) *result++ = *first2++;
        else *result++ = *first1++;
    }
    result = std::copy(first1, last1, result);
    return std::copy(first2, last2, result);
}

template<class BidirectionalIterator>
inline void inplace_merge(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last) {
    if (first == middle || middle == last) return;
    for (BidirectionalIterator it = middle; it != last; ++it) {
        typename iterator_traits<BidirectionalIterator>::value_type value = *it;
        BidirectionalIterator pos = it;
        BidirectionalIterator prev = pos;
        while (pos != first) {
            prev = pos;
            --prev;
            if (!(value < *prev)) break;
            *pos = *prev;
            pos = prev;
        }
        *pos = value;
    }
}

template<class BidirectionalIterator, class Compare>
inline void inplace_merge(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last, Compare comp) {
    if (first == middle || middle == last) return;
    for (BidirectionalIterator it = middle; it != last; ++it) {
        typename iterator_traits<BidirectionalIterator>::value_type value = *it;
        BidirectionalIterator pos = it;
        while (pos != first) {
            BidirectionalIterator prev = pos;
            --prev;
            if (!comp(value, *prev)) break;
            *pos = *prev;
            pos = prev;
        }
        *pos = value;
    }
}

template<class InputIterator1, class InputIterator2>
inline bool includes(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2) {
    while (first2 != last2) {
        if (first1 == last1 || *first2 < *first1) return false;
        if (!(*first1 < *first2)) ++first2;
        ++first1;
    }
    return true;
}

template<class InputIterator1, class InputIterator2, class Compare>
inline bool includes(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, Compare comp) {
    while (first2 != last2) {
        if (first1 == last1 || comp(*first2, *first1)) return false;
        if (!comp(*first1, *first2)) ++first2;
        ++first1;
    }
    return true;
}

template<class InputIterator1, class InputIterator2, class OutputIterator>
inline OutputIterator set_union(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
    while (first1 != last1 && first2 != last2) {
        if (*first1 < *first2) *result++ = *first1++;
        else if (*first2 < *first1) *result++ = *first2++;
        else { *result++ = *first1++; ++first2; }
    }
    result = std::copy(first1, last1, result);
    return std::copy(first2, last2, result);
}

template<class InputIterator1, class InputIterator2, class OutputIterator, class Compare>
inline OutputIterator set_union(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result, Compare comp) {
    while (first1 != last1 && first2 != last2) {
        if (comp(*first1, *first2)) *result++ = *first1++;
        else if (comp(*first2, *first1)) *result++ = *first2++;
        else { *result++ = *first1++; ++first2; }
    }
    result = std::copy(first1, last1, result);
    return std::copy(first2, last2, result);
}

template<class InputIterator1, class InputIterator2, class OutputIterator>
inline OutputIterator set_intersection(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
    while (first1 != last1 && first2 != last2) {
        if (*first1 < *first2) ++first1;
        else if (*first2 < *first1) ++first2;
        else { *result++ = *first1++; ++first2; }
    }
    return result;
}

template<class InputIterator1, class InputIterator2, class OutputIterator, class Compare>
inline OutputIterator set_intersection(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result, Compare comp) {
    while (first1 != last1 && first2 != last2) {
        if (comp(*first1, *first2)) ++first1;
        else if (comp(*first2, *first1)) ++first2;
        else { *result++ = *first1++; ++first2; }
    }
    return result;
}

template<class InputIterator1, class InputIterator2, class OutputIterator>
inline OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
    while (first1 != last1) {
        if (first2 == last2) return std::copy(first1, last1, result);
        if (*first1 < *first2) *result++ = *first1++;
        else if (*first2 < *first1) ++first2;
        else { ++first1; ++first2; }
    }
    return result;
}

template<class InputIterator1, class InputIterator2, class OutputIterator, class Compare>
inline OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result, Compare comp) {
    while (first1 != last1) {
        if (first2 == last2) return std::copy(first1, last1, result);
        if (comp(*first1, *first2)) *result++ = *first1++;
        else if (comp(*first2, *first1)) ++first2;
        else { ++first1; ++first2; }
    }
    return result;
}

template<class InputIterator1, class InputIterator2, class OutputIterator>
inline OutputIterator set_symmetric_difference(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
    while (first1 != last1 && first2 != last2) {
        if (*first1 < *first2) *result++ = *first1++;
        else if (*first2 < *first1) *result++ = *first2++;
        else { ++first1; ++first2; }
    }
    result = std::copy(first1, last1, result);
    return std::copy(first2, last2, result);
}

template<class InputIterator1, class InputIterator2, class OutputIterator, class Compare>
inline OutputIterator set_symmetric_difference(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result, Compare comp) {
    while (first1 != last1 && first2 != last2) {
        if (comp(*first1, *first2)) *result++ = *first1++;
        else if (comp(*first2, *first1)) *result++ = *first2++;
        else { ++first1; ++first2; }
    }
    result = std::copy(first1, last1, result);
    return std::copy(first2, last2, result);
}

template<class RandomAccessIterator>
inline void push_heap(RandomAccessIterator first, RandomAccessIterator last) {
    typedef typename iterator_traits<RandomAccessIterator>::value_type value_type;
    __maiacpp_algorithm_detail::__push_heap(first, last, __maiacpp_algorithm_detail::__identity_less<value_type>());
}

template<class RandomAccessIterator, class Compare>
inline void push_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
    __maiacpp_algorithm_detail::__push_heap(first, last, comp);
}

template<class RandomAccessIterator>
inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last) {
    typedef typename iterator_traits<RandomAccessIterator>::value_type value_type;
    if (last - first < 2) return;
    std::iter_swap(first, last - 1);
    __maiacpp_algorithm_detail::__adjust_heap(first, last - 1, 0, __maiacpp_algorithm_detail::__identity_less<value_type>());
}

template<class RandomAccessIterator, class Compare>
inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
    if (last - first < 2) return;
    std::iter_swap(first, last - 1);
    __maiacpp_algorithm_detail::__adjust_heap(first, last - 1, 0, comp);
}

template<class RandomAccessIterator>
inline void make_heap(RandomAccessIterator first, RandomAccessIterator last) {
    typedef typename iterator_traits<RandomAccessIterator>::value_type value_type;
    for (typename iterator_traits<RandomAccessIterator>::difference_type parent = (last - first) / 2; parent > 0; ) {
        --parent;
        __maiacpp_algorithm_detail::__adjust_heap(first, last, parent, __maiacpp_algorithm_detail::__identity_less<value_type>());
    }
}

template<class RandomAccessIterator, class Compare>
inline void make_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
    for (typename iterator_traits<RandomAccessIterator>::difference_type parent = (last - first) / 2; parent > 0; ) {
        --parent;
        __maiacpp_algorithm_detail::__adjust_heap(first, last, parent, comp);
    }
}

template<class RandomAccessIterator>
inline void sort_heap(RandomAccessIterator first, RandomAccessIterator last) {
    while (last - first > 1) std::pop_heap(first, last--);
}

template<class RandomAccessIterator, class Compare>
inline void sort_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
    while (last - first > 1) std::pop_heap(first, last--, comp);
}

template<class T>
inline const T& min(const T& a, const T& b) { return b < a ? b : a; }
template<class T, class Compare>
inline const T& min(const T& a, const T& b, Compare comp) { return comp(b, a) ? b : a; }
template<class T>
inline const T& max(const T& a, const T& b) { return a < b ? b : a; }
template<class T, class Compare>
inline const T& max(const T& a, const T& b, Compare comp) { return comp(a, b) ? b : a; }

template<class ForwardIterator>
inline ForwardIterator min_element(ForwardIterator first, ForwardIterator last) {
    if (first == last) return last;
    ForwardIterator best = first;
    while (++first != last) if (*first < *best) best = first;
    return best;
}

template<class ForwardIterator, class Compare>
inline ForwardIterator min_element(ForwardIterator first, ForwardIterator last, Compare comp) {
    if (first == last) return last;
    ForwardIterator best = first;
    while (++first != last) if (comp(*first, *best)) best = first;
    return best;
}

template<class ForwardIterator>
inline ForwardIterator max_element(ForwardIterator first, ForwardIterator last) {
    if (first == last) return last;
    ForwardIterator best = first;
    while (++first != last) if (*best < *first) best = first;
    return best;
}

template<class ForwardIterator, class Compare>
inline ForwardIterator max_element(ForwardIterator first, ForwardIterator last, Compare comp) {
    if (first == last) return last;
    ForwardIterator best = first;
    while (++first != last) if (comp(*best, *first)) best = first;
    return best;
}

template<class InputIterator1, class InputIterator2>
inline bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2) {
    while (first1 != last1 && first2 != last2) {
        if (*first1 < *first2) return true;
        if (*first2 < *first1) return false;
        ++first1; ++first2;
    }
    return first1 == last1 && first2 != last2;
}

template<class InputIterator1, class InputIterator2, class Compare>
inline bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, Compare comp) {
    while (first1 != last1 && first2 != last2) {
        if (comp(*first1, *first2)) return true;
        if (comp(*first2, *first1)) return false;
        ++first1; ++first2;
    }
    return first1 == last1 && first2 != last2;
}

template<class BidirectionalIterator>
inline bool next_permutation(BidirectionalIterator first, BidirectionalIterator last) {
    typedef typename iterator_traits<BidirectionalIterator>::value_type value_type;
    return std::next_permutation(first, last, __maiacpp_algorithm_detail::__identity_less<value_type>());
}

template<class BidirectionalIterator, class Compare>
inline bool next_permutation(BidirectionalIterator first, BidirectionalIterator last, Compare comp) {
    if (first == last) return false;
    BidirectionalIterator i = last;
    if (first == --i) return false;
    while (true) {
        BidirectionalIterator ii = i;
        if (comp(*--i, *ii)) {
            BidirectionalIterator j = last;
            while (!comp(*i, *--j)) {}
            std::iter_swap(i, j);
            std::reverse(ii, last);
            return true;
        }
        if (i == first) {
            std::reverse(first, last);
            return false;
        }
    }
}

template<class BidirectionalIterator>
inline bool prev_permutation(BidirectionalIterator first, BidirectionalIterator last) {
    typedef typename iterator_traits<BidirectionalIterator>::value_type value_type;
    return std::prev_permutation(first, last, __maiacpp_algorithm_detail::__identity_less<value_type>());
}

template<class BidirectionalIterator, class Compare>
inline bool prev_permutation(BidirectionalIterator first, BidirectionalIterator last, Compare comp) {
    if (first == last) return false;
    BidirectionalIterator i = last;
    if (first == --i) return false;
    while (true) {
        BidirectionalIterator ii = i;
        if (comp(*ii, *--i)) {
            BidirectionalIterator j = last;
            while (!comp(*--j, *i)) {}
            std::iter_swap(i, j);
            std::reverse(ii, last);
            return true;
        }
        if (i == first) {
            std::reverse(first, last);
            return false;
        }
    }
}

} // namespace std

#endif