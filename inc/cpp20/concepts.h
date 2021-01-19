
# pragma once

# include <functional>
# include <type_traits>

namespace cpp
{
/*
 * This file is for implentations of specific c++20 concepts required in yamdal, but which are not included in g++-8's standard library yet.
 * Unless otherwise stated, implementations come from cppreference.com
 */

/*
 * ----------------------------------------
 *
 * Core language concepts
 *
 * ----------------------------------------
 */

/*
 * The concept same_as<T, U> is satisfied if and only if T and U denote the same type.
 * std::same_as<T, U> subsumes std::same_as<U, T> and vice versa.
 */
   template<class T, class U>
   concept bool same_as =
      std::is_same_v<T,U>
   && std::is_same_v<U,T>;

/*
 * The concept convertible_to<From, To> specifies that an expression of the same type and value category as those of std::declval<From>() can be implicitly and explicitly converted to the type To, and the two forms of conversion are equivalent.
 */
   template<class From, class To>
   concept convertible_to =
      std::is_convertible_v<From, To>
   && requires( std::add_rvalue_reference_t<From> (&f)() )
     {
         static_cast<To>(f());
     };

/*
 * The concept integral<T> is satisfied if and only if T is an integral type
 */
   template<class T>
   concept integral = std::is_integral_v<T>;

/*
 * The concept signed_integral<T> is satisfied if and only if T is an integral type and std::is_signed_v<T> is true
 */
   template<class T>
   concept signed_integral = std::is_integral_v<T> && std::is_signed_v<T>;

/*
 * The concept unsigned_integral<T> is satisfied if and only if T is an integral type and std::is_signed_v<T> is false
 */
   template<class T>
   concept unsigned_integral = std::is_integral_v<T> && !std::is_signed_v<T>;

/*
 * The concept floating_point<T> is satisfied if and only if T is a floating-point type
 */
   template<class T>
   concept floating_point = std::is_floating_point_v<T>;

/*
 * The concept assignable_from<LHS, RHS> specifies that an expression of the type and value category specified by RHS can be assigned to an lvalue expression whose type is specified by LHS.
 *
 *    NOTE:
 *    This is an incomplete implementation because g++-8 doesn't have common_reference yet. If needed, could possibly use the gcc libstdc++? Need to check licensing first.
 */
   template<class LHS, class RHS>
   concept assignable_from =
      std::is_lvalue_reference_v<LHS>
// && std::common_reference_with<const std::remove_reference_t<LHS>&,
//                               const std::remove_reference_t<RHS>&>
   && requires(LHS lhs, RHS&& rhs)
     {
         { lhs = std::forward<RHS>(rhs) } -> cpp::same_as<LHS>;
     };

/*
 * ----------------------------------------
 *
 * Callable concepts
 *
 * ----------------------------------------
 */

/*
 * The invocable concept specifies that a callable type F can be called with a set of argument Args... using the function template std::invoke.
 */
   template<class F, class... Args>
   concept bool invocable =
      requires(F&& f, Args&&... args)
     {
         std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
     };

}
