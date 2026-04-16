// ISO/IEC 14882:1998(E) - 20.3 Function objects

#ifndef _FUNCTIONAL_
#define _FUNCTIONAL_

#include <cstddef>

namespace std {

template <class Arg, class Result> struct unary_function {
    typedef Arg argument_type;
    typedef Result result_type;
};

template <class Arg1, class Arg2, class Result> struct binary_function {
    typedef Arg1 first_argument_type;
    typedef Arg2 second_argument_type;
    typedef Result result_type;
};

template <class T> struct plus : binary_function<T,T,T> {
    T operator()(const T& x, const T& y) const;
};

template <class T> struct minus : binary_function<T,T,T> {
    T operator()(const T& x, const T& y) const;
};

template <class T> struct multiplies : binary_function<T,T,T> {
    T operator()(const T& x, const T& y) const;
};

template <class T> struct divides : binary_function<T,T,T> {
    T operator()(const T& x, const T& y) const;
};

template <class T> struct modulus : binary_function<T,T,T> {
    T operator()(const T& x, const T& y) const;
};

template <class T> struct negate : unary_function<T,T> {
    T operator()(const T& x) const;
};

template <class T> struct equal_to : binary_function<T,T,bool> {
    bool operator()(const T& x, const T& y) const;
};

template <class T> struct not_equal_to : binary_function<T,T,bool> {
    bool operator()(const T& x, const T& y) const;
};

template <class T> struct greater : binary_function<T,T,bool> {
    bool operator()(const T& x, const T& y) const;
};

template <class T> struct less : binary_function<T,T,bool> {
    bool operator()(const T& x, const T& y) const;
};

template <class T> struct greater_equal : binary_function<T,T,bool> {
    bool operator()(const T& x, const T& y) const;
};

template <class T> struct less_equal : binary_function<T,T,bool> {
    bool operator()(const T& x, const T& y) const;
};

template <class T> struct logical_and : binary_function<T,T,bool> {
    bool operator()(const T& x, const T& y) const;
};

template <class T> struct logical_or : binary_function<T,T,bool> {
    bool operator()(const T& x, const T& y) const;
};

template <class T> struct logical_not : unary_function<T,bool> {
    bool operator()(const T& x) const;
};

template <class Predicate>
class unary_negate : public unary_function<typename Predicate::argument_type, bool> {
public:
    explicit unary_negate(const Predicate& pred);
    bool operator()(const typename Predicate::argument_type& x) const;
};

template <class Predicate>
unary_negate<Predicate> not1(const Predicate& pred);

template <class Predicate>
class binary_negate
    : public binary_function<typename Predicate::first_argument_type,
                             typename Predicate::second_argument_type, bool> {
public:
    explicit binary_negate(const Predicate& pred);
    bool operator()(const typename Predicate::first_argument_type& x,
                    const typename Predicate::second_argument_type& y) const;
};

template <class Predicate>
binary_negate<Predicate> not2(const Predicate& pred);

template <class Operation>
class binder1st : public unary_function<typename Operation::second_argument_type,
                                        typename Operation::result_type> {
protected:
    Operation op;
    typename Operation::first_argument_type value;
public:
    binder1st(const Operation& x, const typename Operation::first_argument_type& y);
    typename Operation::result_type
    operator()(const typename Operation::second_argument_type& x) const;
};

template <class Operation, class T>
binder1st<Operation> bind1st(const Operation& op, const T& x);

template <class Operation>
class binder2nd : public unary_function<typename Operation::first_argument_type,
                                        typename Operation::result_type> {
protected:
    Operation op;
    typename Operation::second_argument_type value;
public:
    binder2nd(const Operation& x, const typename Operation::second_argument_type& y);
    typename Operation::result_type
    operator()(const typename Operation::first_argument_type& x) const;
};

template <class Operation, class T>
binder2nd<Operation> bind2nd(const Operation& op, const T& x);

template <class Arg, class Result>
class pointer_to_unary_function : public unary_function<Arg, Result> {
public:
    explicit pointer_to_unary_function(Result (*f)(Arg));
    Result operator()(Arg x) const;
};

template <class Arg, class Result>
pointer_to_unary_function<Arg, Result> ptr_fun(Result (*f)(Arg));

template <class Arg1, class Arg2, class Result>
class pointer_to_binary_function : public binary_function<Arg1, Arg2, Result> {
public:
    explicit pointer_to_binary_function(Result (*f)(Arg1, Arg2));
    Result operator()(Arg1 x, Arg2 y) const;
};

template <class Arg1, class Arg2, class Result>
pointer_to_binary_function<Arg1, Arg2, Result>
ptr_fun(Result (*f)(Arg1, Arg2));

template <class S, class T> class mem_fun_t : public unary_function<T*, S> {
public:
    explicit mem_fun_t(S (T::*p)());
    S operator()(T* p) const;
};

template <class S, class T, class A> class mem_fun1_t : public binary_function<T*, A, S> {
public:
    explicit mem_fun1_t(S (T::*p)(A));
    S operator()(T* p, A x) const;
};

template<class S, class T> mem_fun_t<S,T> mem_fun(S (T::*f)());
template<class S, class T, class A> mem_fun1_t<S,T,A> mem_fun(S (T::*f)(A));

template <class S, class T> class mem_fun_ref_t : public unary_function<T, S> {
public:
    explicit mem_fun_ref_t(S (T::*p)());
    S operator()(T& p) const;
};

template <class S, class T, class A> class mem_fun1_ref_t : public binary_function<T, A, S> {
public:
    explicit mem_fun1_ref_t(S (T::*p)(A));
    S operator()(T& p, A x) const;
};

template<class S, class T> mem_fun_ref_t<S,T> mem_fun_ref(S (T::*f)());
template<class S, class T, class A> mem_fun1_ref_t<S,T,A> mem_fun_ref(S (T::*f)(A));

template <class S, class T> class const_mem_fun_t : public unary_function<T*, S> {
public:
    explicit const_mem_fun_t(S (T::*p)() const);
    S operator()(const T* p) const;
};

template <class S, class T, class A> class const_mem_fun1_t : public binary_function<T*, A, S> {
public:
    explicit const_mem_fun1_t(S (T::*p)(A) const);
    S operator()(const T* p, A x) const;
};

template<class S, class T> const_mem_fun_t<S,T> mem_fun(S (T::*f)() const);
template<class S, class T, class A> const_mem_fun1_t<S,T,A> mem_fun(S (T::*f)(A) const);

template <class S, class T> class const_mem_fun_ref_t : public unary_function<T, S> {
public:
    explicit const_mem_fun_ref_t(S (T::*p)() const);
    S operator()(const T& p) const;
};

template <class S, class T, class A> class const_mem_fun1_ref_t : public binary_function<T, A, S> {
public:
    explicit const_mem_fun1_ref_t(S (T::*p)(A) const);
    S operator()(const T& p, A x) const;
};

template<class S, class T> const_mem_fun_ref_t<S,T> mem_fun_ref(S (T::*f)() const);
template<class S, class T, class A> const_mem_fun1_ref_t<S,T,A> mem_fun_ref(S (T::*f)(A) const);

} // namespace std

#endif