
# pragma once

# include "serial/algorithm.h"

# ifdef _OPENMP
# include "openmp/algorithm.h"
# endif

# include "views.h"
# include "concepts.h"
# include "index.h"
# include "execution.h"

# include "external/mdspan.h"

# include <utility>
# include <functional>
# include <concepts>
# include <type_traits>

# include <cassert>

namespace yam
{
/*
 * ===============================================================
 *
 * yam::assign
 * Assign values in range [begin_index,begin_index+extents) from one indexable to another indexable
 *
 * ===============================================================
 */

/*
 * Convenience overloads -----------------------------------------
 */

/*
 * if no execution policy is specified, use serial
 */
   template<indexable Destination,
            indexable Source,
            ptrdiff_t... Exts>
      requires same_grid_as<Destination,
                            Source>
            && std::assignable_from<element_type_of_t<Destination>,
                                    element_type_of_t<Source>>
            && (sizeof...(Exts)==ndim_of_v<Source>)
   constexpr void assign( const index_type_of_t<Source> begin_index,
                          const stx::extents<Exts...>          exts,
                                Destination&&           destination,
                          const Source&                      source )
  {
      assign( execution::seq,
              begin_index, exts,
              std::forward<Destination>(destination),
              source );

      return;
  }

/*
 * if `assign` is passed an rvalue `destination` (for example a temporary modifying-view), this overload makes `destination` an lvalue to pass to assign implementation
 *    need requires clause after function parameters so `destination` is in-scope for `decltype(std::forward<Destination>(destination))`
 */
   template<indexable Destination,
            indexable Source,
            ptrdiff_t... Exts>
   constexpr void assign( const execution_policy auto        policy,
                          const index_type_of_t<Source> begin_index,
                          const stx::extents<Exts...>          exts,
                                Destination&&           destination,
                          const Source&                      source )
      requires same_grid_as<Destination,
                            Source>
            && std::assignable_from<element_type_of_t<Destination>,
                                    element_type_of_t<Source>>
            && (std::is_rvalue_reference_v<decltype(std::forward<Destination>(destination))>)
            && (sizeof...(Exts)==ndim_of_v<Source>)
  {
      if( std::is_constant_evaluated() )
     {
         assert( is_constexpr_policy_v<decltype(policy)> );
     }

      assign( policy,
              begin_index, exts,
              destination,
              source );

      return;
  }

/*
 * ===============================================================
 *
 * yam::fill
 * assign all elements in range [begin_index,begin_index+extents) to `value`
 *
 * ===============================================================
 */

   template<indexable Destination,
            ptrdiff_t...     Exts,
            typename    ValueType>
      requires (sizeof...(Exts)==ndim_of_v<Destination>)
            && std::assignable_from<element_type_of_t<Destination>,
                                    ValueType>
   constexpr void fill( const execution_policy auto             policy,
                        const index_type_of_t<Destination> begin_index,
                        const stx::extents<Exts...>            extents,
                              Destination&&                destination,
                        const ValueType&                         value )
  {
      if( std::is_constant_evaluated() )
     {
         assert( is_constexpr_policy_v<decltype(policy)> );
     }

      using index_type = index_type_of_t<Destination>;

      assign( policy,
              begin_index, extents,
              std::forward<Destination>(destination),
              [&value]( index_type ){ return value; } );
      return;
  }

// if no policy is given, use serial
   template<indexable Destination,
            ptrdiff_t...     Exts,
            typename    ValueType>
      requires (sizeof...(Exts)==ndim_of_v<Destination>)
            && std::assignable_from<element_type_of_t<Destination>,
                                    ValueType>
   constexpr void fill( const index_type_of_t<Destination> begin_index,
                        const stx::extents<Exts...>            extents,
                              Destination&&                destination,
                              ValueType&&                        value )
  {
      fill( execution::seq,
            begin_index, extents,
            std::forward<Destination>(destination),
            std::forward<ValueType>(value) );
      return;
  }

/*
 * ===============================================================
 *
 * yam::generate
 * assign all elements in range [begin_index,begin_index+extents) to successive calls to generator()
 *
 * ===============================================================
 */

   template<indexable Destination,
            ptrdiff_t...     Exts,
            typename    Generator>
      requires (sizeof...(Exts)==ndim_of_v<Destination>)
            && std::invocable<Generator>
            && std::assignable_from<element_type_of_t<Destination>,
                                    std::invoke_result_t<Generator>>
   constexpr void generate( const execution_policy auto             policy,
                            const index_type_of_t<Destination> begin_index,
                            const stx::extents<Exts...>            extents,
                                  Destination&&                destination,
                                  Generator                      generator )
  {
      if( std::is_constant_evaluated() )
     {
         assert( is_constexpr_policy_v<decltype(policy)> );
     }

      using index_type = index_type_of_t<Destination>;

      assign( policy,
              begin_index, extents,
              std::forward<Destination>(destination),
              [&generator]( index_type ){ return generator(); } );
      return;
  }

// if no policy is given, use serial
   template<indexable Destination,
            ptrdiff_t...     Exts,
            typename    Generator>
      requires (sizeof...(Exts)==ndim_of_v<Destination>)
            && std::invocable<Generator>
            && std::assignable_from<element_type_of_t<Destination>,
                                    std::invoke_result_t<Generator>>
   constexpr void generate( const index_type_of_t<Destination> begin_index,
                            const stx::extents<Exts...>            extents,
                                  Destination&&                destination,
                                  Generator&&                    generator )
  {
      generate( execution::seq,
                begin_index, extents,
                std::forward<Destination>(destination),
                std::forward<Generator>(generator) );
      return;
  }

/*
 * ===============================================================
 *
 * yam::transform
 *    yam::transform applies the given function TransformFunc to a variadic pack of indexables
 *    Two overload types are available
 *       1) returns an indexable lambda that lazily evaluates the transform when called with an index
 *             - note this overload copies the indexable `source` arguments to the transform
 *             - result can later be assigned to an index range using yam::assign
 *       2) eagerly evaluate the transformation and assign result into given indexable
 *             - evaluates only over a given index range [begin_index,begin_index+exts)
 *             - can be executed according to a yam::execution_policy
 *
 * ===============================================================
 */

/*
 * lazy transform
 */
   template<typename TransformFunc,
            view           Source0,
            view...        Sources>
      requires same_grid_as<Source0,
                            Sources...>
            && transformation<TransformFunc,
                              element_type_of_t<Source0>,
                              element_type_of_t<Sources>...>
   [[nodiscard]]
   constexpr view auto transform( TransformFunc&& transform_func,
                                  Source0&&              source0,
                                  Sources&&...           sources )
  {
      using index_type = common_index_type_t<Source0,Sources...>;

   // capture by value in case returned lambda outlives original sources/transform_func.
   // forwarding allows move construction if sources/transform_func are rvalues.

      return [ tfunc{std::forward<TransformFunc>(transform_func)},
               src0{std::forward<Source0>(source0)},
               ...srcs{std::forward<Sources>(sources)} ]
            ( index_type idx )
           {
               return std::invoke( tfunc,
                                   src0(idx),
                                   srcs(idx)... );
           };
  }

/*
 * eager transform
 */
   template<typename    TransformFunc,
            indexable     Destination,
            indexable...      Sources,
            ptrdiff_t...         Exts>
      requires same_grid_as<Destination,
                            Sources...>
            && transformation_r<TransformFunc,
                                element_type_of_t<Destination>,
                                element_type_of_t<Sources>...>
            && (sizeof...(Exts)==ndim_of_v<Destination>)
   constexpr void transform( const execution_policy auto             policy,
                             const index_type_of_t<Destination> begin_index,
                             const stx::extents<Exts...>               exts,
                                   Destination&&                destination,
                                   TransformFunc&&           transform_func,
                             const Sources&...                      sources )
  {
      if( std::is_constant_evaluated() )
     {
         assert( is_constexpr_policy_v<decltype(policy)> );
     }

   // need to create a view (window) of each source to avoid copying entire array into lazy transform adaptor
      assign( policy,
              begin_index, exts,
              std::forward<Destination>(destination),
              transform( std::forward<TransformFunc>(transform_func),
                         window(sources)... ) );

      return;
  }

/*
 * Convenience overload for eager serial execution
 */
   template<typename    TransformFunc,
            indexable     Destination,
            indexable...      Sources,
            ptrdiff_t...         Exts>
      requires same_grid_as<Destination,
                            Sources...>
            && transformation_r<TransformFunc,
                                element_type_of_t<Destination>,
                                element_type_of_t<Sources>...>
            && (sizeof...(Exts)==ndim_of_v<Destination>)
   constexpr void transform( const index_type_of_t<Destination> begin_index,
                             const stx::extents<Exts...>               exts,
                                   Destination&&                destination,
                                   TransformFunc&&           transform_func,
                                   Sources&&...                     sources )
  {
      transform( execution::seq,
                 begin_index, exts,
                 std::forward<Destination>(destination),
                 std::forward<TransformFunc>(transform_func),
                 std::forward<Sources>(sources)... );

      return;
  }

/*
 * ===============================================================
 *
 * yam::reduce
 *    Reduces the range [begin_index,begin_index+extents), in unspecified order, along with the initial value init over reduction_func
 *       Performs a left-fold over the range in an (unspecified) order - ie requires reduce_func(std::move(init),source(index))
 *    Currently only serial execution available
 *
 * ===============================================================
 */

/*
 * Convenience overload assumes serial evaluation
 */
   template<typename ReduceFunc,
            typename ReduceType,
            indexable    Source,
            ptrdiff_t...   Exts>
      requires reduction<ReduceFunc,
                         ReduceType,
                         element_type_of_t<Source>>
            && (sizeof...(Exts)==ndim_of_v<Source>)
   [[nodiscard]]
   constexpr ReduceType reduce( const index_type_of_t<Source> begin_index,
                                const stx::extents<Exts...>          exts,
                                      ReduceFunc&&            reduce_func,
                                      ReduceType&&                   init,
                                      Source&&                     source )
  {
      return reduce( execution::seq,
                     begin_index, exts,
                     std::forward<ReduceFunc>(reduce_func),
                     std::forward<ReduceType>(init),
                     std::forward<Source>(source) );
  }

/*
 * ===============================================================
 *
 * yam::transform_reduce
 *    Applies transform_func to each set of elements at the same index in the range [begin_index, begin_index+extents) from the indexable Sources and reduces the results (possibly permuted and aggregated in unspecified manner) along with the initial value init over reduce_func
 *
 * ===============================================================
 */

   template<typename    TransformFunc,
            typename       ReduceFunc,
            typename       ReduceType,
            indexable         Source0,
            indexable...      Sources,
            ptrdiff_t...         Exts>
      requires same_grid_as<Source0,
                            Sources...>
            && transformation_reduction<TransformFunc,
                                        ReduceFunc,
                                        ReduceType,
                                        element_type_of_t<Source0>,
                                        element_type_of_t<Sources>...>
            && (sizeof...(Exts)==ndim_of_v<Source0>)
   [[nodiscard]]
   constexpr ReduceType transform_reduce( const execution_policy auto         policy,
                                          const index_type_of_t<Source0> begin_index,
                                          const stx::extents<Exts...>           exts,
                                                TransformFunc&&       transform_func,
                                                ReduceFunc&&             reduce_func,
                                                ReduceType&&                    init,
                                          const Source0&                     source0,
                                          const Sources&...                  sources )
  {
      if( std::is_constant_evaluated() )
     {
         assert( is_constexpr_policy_v<decltype(policy)> );
     }

      return reduce( policy,
                     begin_index, exts,
                     std::forward<ReduceFunc>(reduce_func),
                     std::forward<ReduceType>(init),
                     transform( std::forward<TransformFunc>(transform_func),
                                window(source0),
                                window(sources)... ) );
  }

/*
 * Convenience overload assumes serial evaluation
 */
   template<typename    TransformFunc,
            typename       ReduceFunc,
            typename       ReduceType,
            indexable         Source0,
            indexable...      Sources,
            ptrdiff_t...         Exts>
      requires same_grid_as<Source0,
                            Sources...>
            && transformation_reduction<TransformFunc,
                                        ReduceFunc,
                                        ReduceType,
                                        element_type_of_t<Source0>,
                                        element_type_of_t<Sources>...>
            && (sizeof...(Exts)==ndim_of_v<Source0>)
   [[nodiscard]]
   constexpr ReduceType transform_reduce( const index_type_of_t<Source0> begin_index,
                                          const stx::extents<Exts...>           exts,
                                                TransformFunc&&       transform_func,
                                                ReduceFunc&&             reduce_func,
                                                ReduceType&&                    init,
                                                Source0&&                    source0,
                                                Sources&&...                 sources )
  {
      return transform_reduce( execution::seq,
                               begin_index, exts,
                               std::forward<TransformFunc>(transform_func),
                               std::forward<ReduceFunc>(reduce_func),
                               std::forward<ReduceType>(init),
                               std::forward<Source0>(source0),
                               std::forward<Sources>(sources)... );
  }
}
