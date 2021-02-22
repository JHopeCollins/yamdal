
# pragma once

# include "type_traits.h"
# include "index.h"

# include <concepts>
# include <type_traits>

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
 * The indexable_r concept specifies that a type A is callable with a single argument of type index<ndim,grid> (with unspecified ndim and grid) using the function template std::invoke, and has a return type that is convertible to R
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
 * view concepts with fully specified index.
 *
 *    - A view is an indexable type that is either copy or move constructible (or both).
 *    - Semantically, a view is intended to be analagous to std::ranges::view, so it should be copy/move constructible in constant time ie independent of the number of elements.
 *
 *    - Views will generally be either a non-owning view over some data in memory, or a generator that computes data on demand. They will normally only be the size of a couple of pointers or less.
 *    - The most common use for a view (over just an indexable) is to be captured by a lazy adaptor such as those returned from yam::transform.
 *
 * ===============================================================
 */

/*
 * The view_with concept specifies that a type A is indexable with index<ndim,grid>, and is either copy or move constructible (or both).
 */
   template<typename  A,
            ndim_t ndim,
            grid_t grid= primal>
   concept view_with =
      indexable_with<A,ndim,grid>
   && (   std::copy_constructible<A>
       || std::move_constructible<A> );

/*
 * The view_with_r concept specifies that a type A is indexable with index<ndim,grid>, is either copy or move constructible (or both), and has a return type that is convertible to R
 */
   template<typename  A,
            typename  R,
            ndim_t ndim,
            grid_t grid= primal>
   concept view_with_r =
      indexable_with_r<A,R,ndim,grid>
   && (   std::copy_constructible<A>
       || std::move_constructible<A> );

/*
 * Specialisations for specified dimension
 */
// 1D
   template<typename  A,
            grid_t grid>
   concept view1_with =
      view_with<A,1,grid>;

   template<typename  A,
            typename  R,
            grid_t grid>
   concept view1_with_r =
      view_with_r<A,R,1,grid>;

// 2D
   template<typename  A,
            grid_t grid>
   concept view2_with =
      view_with<A,2,grid>;

   template<typename  A,
            typename  R,
            grid_t grid>
   concept view2_with_r =
      view_with_r<A,R,2,grid>;

// 3D
   template<typename  A,
            grid_t grid>
   concept view3_with =
      view_with<A,3,grid>;

   template<typename  A,
            typename  R,
            grid_t grid>
   concept view3_with_r =
      view_with_r<A,R,3,grid>;

/*
 * Specialisations for specified gridtype
 */
// primal grid
   template<typename  A,
            ndim_t ndim>
   concept primal_view_with =
      view_with<A,ndim,primal>;

   template<typename  A,
            typename  R,
            ndim_t ndim>
   concept primal_view_with_r =
      view_with_r<A,R,ndim,primal>;

// dual grid
   template<typename  A,
            ndim_t ndim>
   concept dual_view_with =
      view_with<A,ndim,dual>;

   template<typename  A,
            typename  R,
            ndim_t ndim>
   concept dual_view_with_r =
      view_with_r<A,R,ndim,dual>;

/*
 * ===============================================================
 *
 * view concepts with multiple accepted indices
 *
 * ===============================================================
 */

/*
 * The view concept specifies that a type A is indexable (with unspecified ndim and grid), and either copy or move constructible (or both).
 */
   template<typename A>
   concept view =
      view_with<A,1,primal>
   || view_with<A,2,primal>
   || view_with<A,3,primal>
   || view_with<A,1,dual>
   || view_with<A,2,dual>
   || view_with<A,3,dual>;

/*
 * The view_r concept specifies that a type A is indexable (with unspecified ndim and grid), either copy or move constructible (or both), and has a return type that is convertible to R
 */
   template<typename A,
            typename R>
   concept view_r =
      view_with_r<A,R,1,primal>
   || view_with_r<A,R,2,primal>
   || view_with_r<A,R,3,primal>
   || view_with_r<A,R,1,dual>
   || view_with_r<A,R,2,dual>
   || view_with_r<A,R,3,dual>;

/*
 * Specialisations for specified dimension
 */
// 1D
   template<typename A>
   concept view1 =
      view_with<A,1,primal>
   || view_with<A,1,dual>;

   template<typename A,
            typename R>
   concept view1_r =
      view_with_r<A,R,1,primal>
   || view_with_r<A,R,1,dual>;

// 2D
   template<typename A>
   concept view2 =
      view_with<A,2,primal>
   || view_with<A,2,dual>;

   template<typename A,
            typename R>
   concept view2_r =
      view_with_r<A,R,2,primal>
   || view_with_r<A,R,2,dual>;

// 3D
   template<typename A>
   concept view3 =
      view_with<A,3,primal>
   || view_with<A,3,dual>;

   template<typename A,
            typename R>
   concept view3_r =
      view_with_r<A,R,3,primal>
   || view_with_r<A,R,3,dual>;

/*
 * Specialisations for specified gridtype
 */
// primal grid
   template<typename A>
   concept primal_view =
      view_with<A,1,primal>
   || view_with<A,2,primal>
   || view_with<A,3,primal>;

   template<typename A,
            typename R>
   concept primal_view_r =
      view_with_r<A,R,1,primal>
   || view_with_r<A,R,2,primal>
   || view_with_r<A,R,3,primal>;

// dual grid
   template<typename A>
   concept dual_view =
      view_with<A,1,dual>
   || view_with<A,2,dual>
   || view_with<A,3,dual>;

   template<typename A,
            typename R>
   concept dual_view_r =
      view_with_r<A,R,1,dual>
   || view_with_r<A,R,2,dual>
   || view_with_r<A,R,3,dual>;

/*
 * Specialisations for specified number of dimensions and gridtype
 */
// primal grid
// 1D
   template<typename A>
   concept primal_view1 =
      view_with<A,1,primal>;

   template<typename A,
            typename R>
   concept primal_view1_r =
      view_with_r<A,R,1,primal>;

// 2D
   template<typename A>
   concept primal_view2 =
      view_with<A,2,primal>;

   template<typename A,
            typename R>
   concept primal_view2_r =
      view_with_r<A,R,2,primal>;

// 3D
   template<typename A>
   concept primal_view3 =
      view_with<A,3,primal>;

   template<typename A,
            typename R>
   concept primal_view3_r =
      view_with_r<A,R,3,primal>;

// dual grid
// 1D
   template<typename A>
   concept dual_view1 =
      view_with<A,1,dual>;

   template<typename A,
            typename R>
   concept dual_view1_r =
      view_with_r<A,R,1,dual>;

// 2D
   template<typename A>
   concept dual_view2 =
      view_with<A,2,dual>;

   template<typename A,
            typename R>
   concept dual_view2_r =
      view_with_r<A,R,2,dual>;

// 3D
   template<typename A>
   concept dual_view3 =
      view_with<A,3,dual>;

   template<typename A,
            typename R>
   concept dual_view3_r =
      view_with_r<A,R,3,dual>;

/*
 * ===============================================================
 *
 * Compatability concepts for different types with similar/same grids
 *
 * ===============================================================
 */

/*
 * do multiple indexables have the same grid? ie same gridtype and dimension
 */
   template<typename    A,
            typename... As>
   concept same_grid_as =
      indexable<A> && (indexable<As>&&...)
   && (std::same_as<index_type_of_t<A>,
                    index_type_of_t<As>>&&...);

/*
 * ===============================================================
 *
 * Compatability concepts for callables and indexables to be used with higher order functions
 *
 * ===============================================================
 */

/*
 * Set of arguments for a (lazy) transform algorithm (destination range not yet specified)
 *    The function TransformFunc can be called with arguments Arguments and the return type is not void
 */
   template<typename   TransformFunc,
            typename...    Arguments>
   concept transformation =
      std::invocable<TransformFunc,
                     Arguments...>
   && !std::is_void_v<std::invoke_result_t<TransformFunc,
                                           Arguments...>>;

/*
 * Set of arguments for a transform algorithm
 *    The function TransformFunc can be called with arguments Arguments and the return type is assignable to type Destination
 */
   template<typename   TransformFunc,
            typename     Destination,
            typename...    Arguments>
   concept transformation_r =
      std::invocable<TransformFunc,
                     Arguments...>
   && std::assignable_from<Destination,
                           std::invoke_result_t<TransformFunc,
                                                Arguments...>>;

/*
 * Set of arguments for a reduce algorithm
 */
   template<typename ReduceFunc,
            typename ReduceType,
            typename SourceType>
   concept reduction =
      transformation_r<ReduceFunc,
                       ReduceType&,
                       ReduceType&&,
                       const SourceType&>;

/*
 * Set of arguments for transform_reduce algorithm
 */
   template<typename TransformFunc,
            typename    ReduceFunc,
            typename    ReduceType,
            typename       Source0,
            typename...    Sources>
   concept transformation_reduction =
      transformation<TransformFunc,
                     Source0,
                     Sources...>
   && reduction<ReduceFunc,
                ReduceType,
                std::invoke_result_t<TransformFunc,
                                     Source0,
                                     Sources...>>;
}
