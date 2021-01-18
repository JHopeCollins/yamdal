
# pragma once

namespace cpp
{
/*
 * Provides the member typedef type that names T (i.e., the identity transformation).
 *    The behavior of a program that adds specializations for type_identity is undefined.
 */
   template<typename T>
   struct type_identity
  {
      using type = T;
  };

// helper type
   template<typename T>
   using type_identity_t = typename type_identity<T>::type;
}
