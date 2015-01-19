#ifndef PJL_CALL_TRAITS_H
#define PJL_CALL_TRAITS_H

// standard
#include <type_traits>

namespace PJL {

///////////////////////////////////////////////////////////////////////////////

/**
 * Determines whether the given type \c T is efficiently passed by value.
 */
template<typename T>
struct is_efficiently_passed_by_value {
  static bool const value =
        std::is_arithmetic<T>::value
    ||  std::is_enum<T>::value
    ||  std::is_pointer<T>::value
    ||  std::is_reference<T>::value
    ||  std::is_member_function_pointer<T>::value;
};

/**
 * Useful traits when declaring functions.
 * This class is similar to boost::call_traits.
 *
 * @tparam T A type that is used for a function formal argument.
 */
template<typename T,bool = is_efficiently_passed_by_value<T>::value>
struct call_traits {
  /**
   * A type that is guaranteed to be the most efficient to use as a formal
   * argument.
   */
  typedef T const param_type;
};

/**
 * Partial specialization for when \c T is not efficiently passed by value.
 */
template<typename T>
struct call_traits<T,false> {
  typedef T const& param_type;
};

///////////////////////////////////////////////////////////////////////////////

} // namespace PJL
#endif /* PJL_CALL_TRAITS_H */
/* vim:set et sw=2 ts=2: */
