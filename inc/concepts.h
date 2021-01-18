
# pragma once

# include <yamdal.h>
# include <type_traits.h>

# include <cpp20/concepts.h>

namespace yam
{
/*
 * ---------------------------------------------------------------
 *
 * Fully specified indexable concepts
 *
 * ---------------------------------------------------------------
 */

/*
 * The indexable_with concept specifies that a callable type A can be called with a single argument of type index<ndim,grid> using the function template std::invoke, and has a non-void return type.
 */
   template<typename  A,
            ndim_t ndim,
            grid_t grid>
   concept bool indexable_with =
      cpp::invocable<A,index<ndim,grid>>;

/*
 * The indexable_with_r concept specifies that a callable type A can be called with a single argument of type index<ndim,grid> using the function template std::invoke, and has a return type that is convertible to R.
 */
   template<typename  A,
            typename  R,
            ndim_t ndim,
            grid_t grid>
   concept bool indexable_with_r =
      cpp::invocable<A,index<ndim,grid>>
   && cpp::convertible_to<std::invoke_result_t<A,index<ndim,grid>>,R>;

/*
 * Specialisation for specified dimension
 */
// 1D
   template<typename  A,
            grid_t grid>
   concept bool indexable1_with =
      indexable_with<A,1,grid>;

// 2D
   template<typename  A,
            grid_t grid>
   concept bool indexable2_with =
      indexable_with<A,2,grid>;

// 3D
   template<typename  A,
            grid_t grid>
   concept bool indexable3_with =
      indexable_with<A,3,grid>;

/*
 * Specialisation for specified gridtype
 */
// primal grid
   template<typename  A,
            ndim_t ndim>
   concept bool primal_indexable_with =
      indexable_with<A,ndim,primal>;

// dual grid
   template<typename  A,
            ndim_t ndim>
   concept bool dual_indexable_with =
      indexable_with<A,ndim,dual>;

}
