
# pragma once

// # ifdef _OPENMP
//    # include <omp.h>
// # endif
// 
namespace yam
{
/*
 * ===============================================================
 *
 * namespace holding classes and functions used to determined how algorithms are executed (parallelised).
 *
 * ===============================================================
 */

   namespace execution
  {
   /*
    * unique tag types indicating how algorithms should be parallelised
    */
      struct serial_policy {};
      inline constexpr serial_policy seq;

# ifdef _OPENMP
      struct openmp_policy {};
      inline constexpr openmp_policy openmp;
# endif

// # ifdef _OPENACC
//       struct openacc_policy {}
//       inline constexpr openacc_policy openacc;
// # endif
  }

/*
 * Test if a type is an execution policy tag type
 */
// type trait
   template<typename T>
   struct is_execution_policy
      : std::false_type {};

   template<>
   struct is_execution_policy<execution::serial_policy>
      : std::true_type {};

# ifdef _OPENMP
   template<>
   struct is_execution_policy<execution::openmp_policy>
      : std::true_type {};
# endif

// # ifdef _OPENACC
//    template<>
//    struct is_execution_policy<execution::openacc_policy>
//       : std::true_type {};
// # endif

// helper variable template
   template<typename T>
   inline constexpr bool is_execution_policy_v =
      is_execution_policy<std::remove_cvref_t<T>>::value;

// concept
   template<typename T>
   concept execution_policy =
      is_execution_policy_v<T>;

/*
 * Check if policy is suitable for constexpr execution
 */

// type trait
   template<execution_policy policy>
   struct is_constexpr_policy
      : std::false_type {};

   template<>
   struct is_constexpr_policy<execution::serial_policy>
      : std::true_type {};

// helper variable template
   template<typename T>
   inline constexpr bool is_constexpr_policy_v =
      is_constexpr_policy<std::remove_cvref_t<T>>::value;
}
