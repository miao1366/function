// https://blog.csdn.net/csfreebird/article/details/44904121

#include <iostream>
#include <string>



template <bool B, typename T = void>
struct enable_if {};

template <typename T>
struct enable_if<true, T> { typedef T type; };

template <typename T, T v>
struct integral_constant {
  static const T value = v;
  typedef T value_type;
  typedef integral_constant<T, v> type;
};

template <typename T, T v> const T integral_constant<T, v>::value;

typedef integral_constant<bool, true> true_type;
typedef integral_constant<bool, false> false_type;

template <typename T> struct is_integral;

template <typename T> struct is_integral : false_type { };
template<> struct is_integral<bool> : true_type { };
template<> struct is_integral<char> : true_type { };
template<> struct is_integral<unsigned char> : true_type { };
template<> struct is_integral<signed char> : true_type { };


template <typename Op, typename T1, typename T2>
inline void call_op_returning_void(
    const Op& op, T1& v1, const T2& v2) {
    return op(v1, v2);
}

template <typename T, typename Op, typename Enabler = void>
struct DivideOnAddition {
    static void inplace_divide(T& /*obj*/, const Op&, int /*number*/) {
        // do nothing
    }
};

template <typename T, typename Op>
struct ProbablyAddtition {
    ProbablyAddtition(const Op& op) {
        T res(32);
        call_op_returning_void(op, res, T(64));
        _ok = (res == T(96));  // works for integral/floating point.
    }
    operator bool() const { return _ok; }
private:
    bool _ok;
};

template <typename T, typename Op>
struct DivideOnAddition<T, Op, typename enable_if<
                                   is_integral<T>::value>::type> {
    static void inplace_divide(T& obj, const Op& op, int number) {
        static ProbablyAddtition<T, Op> probably_add(op);
        if (probably_add) {
            obj = (T)round(obj / (double)number);
        }
    }
};

namespace is {
    template<bool v> struct is_true{static const bool value = v;};
    template<bool v> const bool is_true<v>::value;
    typedef is_true<true> True;
    typedef is_true<false> False;
    //template<typename T> struct is_intergtal;
    template<typename T> struct is_intergtal : False{};
    template<> struct is_intergtal<char> : True{};
    template<> struct is_intergtal<long> : True{};
}

  


int main() {

    std::cout<< "false_type-> value: " <<false_type::value<< false_type::type::value << std::endl;

}

