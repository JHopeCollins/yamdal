
# pragma once

# include <yamdal.h>
# include <type_traits.h>

# include <cpp20/concepts.h>

namespace yam
{
/*
 * ===============================================================
 *
 * Indexable concepts with fully specified index
 *
 * ===============================================================
 */

/*
 * The indexable_with concept specifies that a type A is callable with a single argument of type index<ndim,grid> using the function template std::invoke.
 */
   template<typename  A,
            ndim_t ndim,
            grid_t grid= primal>
   concept bool indexable_with =
      cpp::invocable<A,index<ndim,grid>>;

/*
 * The indexable_with_r concept specifies that a type A is callable with a single argument of type index<ndim,grid> using the function template std::invoke, and has a return type that is convertible to R.
 */
   template<typename  A,
            typename  R,
            ndim_t ndim,
            grid_t grid= primal>
   concept bool indexable_with_r =
      cpp::invocable<A,index<ndim,grid>>
   && cpp::convertible_to<std::invoke_result_t<A,index<ndim,grid>>,R>;

/*
 * Specialisations for specified dimension
 */
// 1D
   template<typename  A,
            grid_t grid>
   concept bool indexable1_with =
      indexable_with<A,1,grid>;

   template<typename  A,
            typename  R,
            grid_t grid>
   concept bool indexable1_with_r =
      indexable_with_r<A,R,1,grid>;

// 2D
   template<typename  A,
            grid_t grid>
   concept bool indexable2_with =
      indexable_with<A,2,grid>;

   template<typename  A,
            typename  R,
            grid_t grid>
   concept bool indexable2_with_r =
      indexable_with_r<A,R,2,grid>;

// 3D
   template<typename  A,
            grid_t grid>
   concept bool indexable3_with =
      indexable_with<A,3,grid>;

   template<typename  A,
            typename  R,
            grid_t grid>
   concept bool indexable3_with_r =
      indexable_with_r<A,R,3,grid>;

/*
 * Specialisations for specified gridtype
 */
// primal grid
   template<typename  A,
            ndim_t ndim>
   concept bool primal_indexable_with =
      indexable_with<A,ndim,primal>;

   template<typename  A,
            typename  R,
            ndim_t ndim>
   concept bool primal_indexable_with_r =
      indexable_with_r<A,R,ndim,primal>;

// dual grid
   template<typename  A,
            ndim_t ndim>
   concept bool dual_indexable_with =
      indexable_with<A,ndim,dual>;

   template<typename  A,
            typename  R,
            ndim_t ndim>
   concept bool dual_indexable_with_r =
      indexable_with_r<A,R,ndim,dual>;

/*
 * ===============================================================
 *
 * Indexable concepts with multiple accepted indices
 *
 * ===============================================================
 */

/*
 * The indexable concept specifies that a type A is callable with a single argument of type index<ndim,grid> (with unspecified ndim and grid) using the function template std::invoke.
 */
   template<typename A>
   concept bool indexable =
      indexable_with<A,1,primal>
   || indexable_with<A,2,primal>
   || indexable_with<A,3,primal>
   || indexable_with<A,1,dual>
   || indexable_with<A,2,dual>
   || indexable_with<A,3,dual>;

/*
 * The indexable concept specifies that a type A is callable with a single argument of type index<ndim,grid> (with unspecified ndim and grid) using the function template std::invoke, and has a return type that is convertible to R
 */
   template<typename A,
            typename R>
   concept bool indexable_r =
      indexable_with_r<A,R,1,primal>
   || indexable_with_r<A,R,2,primal>
   || indexable_with_r<A,R,3,primal>
   || indexable_with_r<A,R,1,dual>
   || indexable_with_r<A,R,2,dual>
   || indexable_with_r<A,R,3,dual>;

/*
 * Specialisations for specified dimension
 */
// 1D
   template<typename A>
   concept bool indexable1 =
      indexable_with<A,1,primal>
   || indexable_with<A,1,dual>;

   template<typename A,
            typename R>
   concept bool indexable1_r =
      indexable_with_r<A,R,1,primal>
   || indexable_with_r<A,R,1,dual>;

// 2D
   template<typename A>
   concept bool indexable2 =
      indexable_with<A,2,primal>
   || indexable_with<A,2,dual>;

   template<typename A,
            typename R>
   concept bool indexable2_r =
      indexable_with_r<A,R,2,primal>
   || indexable_with_r<A,R,2,dual>;

// 3D
   template<typename A>
   concept bool indexable3 =
      indexable_with<A,3,primal>
   || indexable_with<A,3,dual>;

   template<typename A,
            typename R>
   concept bool indexable3_r =
      indexable_with_r<A,R,3,primal>
   || indexable_with_r<A,R,3,dual>;

/*
 * Specialisations for specified gridtype
 */
// primal grid
   template<typename A>
   concept bool primal_indexable =
      indexable_with<A,1,primal>
   || indexable_with<A,2,primal>
   || indexable_with<A,3,primal>;

   template<typename A,
            typename R>
   concept bool primal_indexable_r =
      indexable_with_r<A,R,1,primal>
   || indexable_with_r<A,R,2,primal>
   || indexable_with_r<A,R,3,primal>;

// dual grid
   template<typename A>
   concept bool dual_indexable =
      indexable_with<A,1,dual>
   || indexable_with<A,2,dual>
   || indexable_with<A,3,dual>;

   template<typename A,
            typename R>
   concept bool dual_indexable_r =
      indexable_with_r<A,R,1,dual>
   || indexable_with_r<A,R,2,dual>
   || indexable_with_r<A,R,3,dual>;

/*
 * Specialisations for specified number of dimensions and gridtype
 */
// primal grid
// 1D
   template<typename A>
   concept bool primal_indexable1 =
      indexable_with<A,1,primal>;

   template<typename A,
            typename R>
   concept bool primal_indexable1_r =
      indexable_with_r<A,R,1,primal>;

// 2D
   template<typename A>
   concept bool primal_indexable2 =
      indexable_with<A,2,primal>;

   template<typename A,
            typename R>
   concept bool primal_indexable2_r =
      indexable_with_r<A,R,2,primal>;

// 3D
   template<typename A>
   concept bool primal_indexable3 =
      indexable_with<A,3,primal>;

   template<typename A,
            typename R>
   concept bool primal_indexable3_r =
      indexable_with_r<A,R,3,primal>;

// dual grid
// 1D
   template<typename A>
   concept bool dual_indexable1 =
      indexable_with<A,1,dual>;

   template<typename A,
            typename R>
   concept bool dual_indexable1_r =
      indexable_with_r<A,R,1,dual>;

// 2D
   template<typename A>
   concept bool dual_indexable2 =
      indexable_with<A,2,dual>;

   template<typename A,
            typename R>
   concept bool dual_indexable2_r =
      indexable_with_r<A,R,2,dual>;

// 3D
   template<typename A>
   concept bool dual_indexable3 =
      indexable_with<A,3,dual>;

   template<typename A,
            typename R>
   concept bool dual_indexable3_r =
      indexable_with_r<A,R,3,dual>;

}
