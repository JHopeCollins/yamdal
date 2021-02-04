
# pragma once

# include <yamdal.h>
# include <type_traits.h>

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
   concept indexable_with =
      std::invocable<A,index<ndim,grid>>;

/*
 * The indexable_with_r concept specifies that a type A is callable with a single argument of type index<ndim,grid> using the function template std::invoke, and has a return type that is convertible to R.
 */
   template<typename  A,
            typename  R,
            ndim_t ndim,
            grid_t grid= primal>
   concept indexable_with_r =
      std::invocable<A,index<ndim,grid>>
   && std::convertible_to<std::invoke_result_t<A,index<ndim,grid>>,R>;

/*
 * Specialisations for specified dimension
 */
// 1D
   template<typename  A,
            grid_t grid>
   concept indexable1_with =
      indexable_with<A,1,grid>;

   template<typename  A,
            typename  R,
            grid_t grid>
   concept indexable1_with_r =
      indexable_with_r<A,R,1,grid>;

// 2D
   template<typename  A,
            grid_t grid>
   concept indexable2_with =
      indexable_with<A,2,grid>;

   template<typename  A,
            typename  R,
            grid_t grid>
   concept indexable2_with_r =
      indexable_with_r<A,R,2,grid>;

// 3D
   template<typename  A,
            grid_t grid>
   concept indexable3_with =
      indexable_with<A,3,grid>;

   template<typename  A,
            typename  R,
            grid_t grid>
   concept indexable3_with_r =
      indexable_with_r<A,R,3,grid>;

/*
 * Specialisations for specified gridtype
 */
// primal grid
   template<typename  A,
            ndim_t ndim>
   concept primal_indexable_with =
      indexable_with<A,ndim,primal>;

   template<typename  A,
            typename  R,
            ndim_t ndim>
   concept primal_indexable_with_r =
      indexable_with_r<A,R,ndim,primal>;

// dual grid
   template<typename  A,
            ndim_t ndim>
   concept dual_indexable_with =
      indexable_with<A,ndim,dual>;

   template<typename  A,
            typename  R,
            ndim_t ndim>
   concept dual_indexable_with_r =
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
   concept indexable =
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
   concept indexable_r =
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
   concept indexable1 =
      indexable_with<A,1,primal>
   || indexable_with<A,1,dual>;

   template<typename A,
            typename R>
   concept indexable1_r =
      indexable_with_r<A,R,1,primal>
   || indexable_with_r<A,R,1,dual>;

// 2D
   template<typename A>
   concept indexable2 =
      indexable_with<A,2,primal>
   || indexable_with<A,2,dual>;

   template<typename A,
            typename R>
   concept indexable2_r =
      indexable_with_r<A,R,2,primal>
   || indexable_with_r<A,R,2,dual>;

// 3D
   template<typename A>
   concept indexable3 =
      indexable_with<A,3,primal>
   || indexable_with<A,3,dual>;

   template<typename A,
            typename R>
   concept indexable3_r =
      indexable_with_r<A,R,3,primal>
   || indexable_with_r<A,R,3,dual>;

/*
 * Specialisations for specified gridtype
 */
// primal grid
   template<typename A>
   concept primal_indexable =
      indexable_with<A,1,primal>
   || indexable_with<A,2,primal>
   || indexable_with<A,3,primal>;

   template<typename A,
            typename R>
   concept primal_indexable_r =
      indexable_with_r<A,R,1,primal>
   || indexable_with_r<A,R,2,primal>
   || indexable_with_r<A,R,3,primal>;

// dual grid
   template<typename A>
   concept dual_indexable =
      indexable_with<A,1,dual>
   || indexable_with<A,2,dual>
   || indexable_with<A,3,dual>;

   template<typename A,
            typename R>
   concept dual_indexable_r =
      indexable_with_r<A,R,1,dual>
   || indexable_with_r<A,R,2,dual>
   || indexable_with_r<A,R,3,dual>;

/*
 * Specialisations for specified number of dimensions and gridtype
 */
// primal grid
// 1D
   template<typename A>
   concept primal_indexable1 =
      indexable_with<A,1,primal>;

   template<typename A,
            typename R>
   concept primal_indexable1_r =
      indexable_with_r<A,R,1,primal>;

// 2D
   template<typename A>
   concept primal_indexable2 =
      indexable_with<A,2,primal>;

   template<typename A,
            typename R>
   concept primal_indexable2_r =
      indexable_with_r<A,R,2,primal>;

// 3D
   template<typename A>
   concept primal_indexable3 =
      indexable_with<A,3,primal>;

   template<typename A,
            typename R>
   concept primal_indexable3_r =
      indexable_with_r<A,R,3,primal>;

// dual grid
// 1D
   template<typename A>
   concept dual_indexable1 =
      indexable_with<A,1,dual>;

   template<typename A,
            typename R>
   concept dual_indexable1_r =
      indexable_with_r<A,R,1,dual>;

// 2D
   template<typename A>
   concept dual_indexable2 =
      indexable_with<A,2,dual>;

   template<typename A,
            typename R>
   concept dual_indexable2_r =
      indexable_with_r<A,R,2,dual>;

// 3D
   template<typename A>
   concept dual_indexable3 =
      indexable_with<A,3,dual>;

   template<typename A,
            typename R>
   concept dual_indexable3_r =
      indexable_with_r<A,R,3,dual>;

/*
 * ===============================================================
 *
 * Compatability concepts for different types with similar/same grids
 *
 * ===============================================================
 */

/*
 * do multiple indexables have compatible grids? ie same gridtype and dimension
 */
   template<typename    A,
            typename... As>
   concept compatible_grids =
      indexable<A> && (indexable<As>&&...)
   && (std::same_as<index_type_of_t<A>,
                    index_type_of_t<As>>&&...);

/*
 * ===============================================================
 *
 * Compatability concepts for callables over indexables
 *
 * ===============================================================
 */

/*
 * the function F can be called with arguments As and the return type is not void
 */
   template<typename     F,
            typename... As>
   concept transformation =
      std::invocable<F,As...>
   && !std::is_void_v<std::invoke_result_t<F,As...>>;

/*
 * the function F can be called with arguments As and the return type is assignable to type R
 */
   template<typename     R,
            typename     F,
            typename... As>
   concept transformation_r =
      std::invocable<F,As...>
   && std::assignable_from<R,std::invoke_result_t<F,As...>>;

}
