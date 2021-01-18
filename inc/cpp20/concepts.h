
# pragma once

# include <functional>
# include <type_traits>

namespace cpp
{
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
   template <class From, class To>
   concept convertible_to =
      std::is_convertible_v<From, To>
   && requires( std::add_rvalue_reference_t<From> (&f)() )
     {
         static_cast<To>(f());
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
   template< class F, class... Args >
   concept bool invocable =
      requires(F&& f, Args&&... args)
     {
         std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
     };

}
