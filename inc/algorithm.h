
# pragma once

# include <views.h>
# include <execution.h>
# include <concepts.h>
# include <yamdal.h>

# include <functional>
# include <concepts>

namespace yam
{
/*
 * ===============================================================
 *
 * yam::assign
 * Assign values in range [begin_index,end_index) from one indexable to another indexable
 *
 * ===============================================================
 */

/*
 * Serial execution --------------------------------------------------
 */

/*
 * 1D grid
 */
   template<indexable1 Destination,
            indexable1 Source>
      requires same_grid_as<Destination,
                            Source>
            && std::assignable_from<element_type_of_t<Destination>,
                                    element_type_of_t<Source>>
   constexpr void assign(       execution::serial_policy,
                          const index_type_of_t<Source> begin_index,
                          const index_type_of_t<Source>   end_index,
                                Destination&            destination,
                          const Source&                      source )
  {
//    if( is_empty_range( begin_index, end_index ) ){ return; }

      for( idx_t i=begin_index[0]; i<end_index[0]; ++i )
     {
         const index_type_of_t<Source> idx{i};
         destination(idx) = source(idx);
     }
      return;
  }

/*
 * 2D grid
 */
   template<indexable2 Destination,
            indexable2 Source>
      requires same_grid_as<Destination,
                            Source>
            && std::assignable_from<element_type_of_t<Destination>,
                                    element_type_of_t<Source>>
   constexpr void assign(       execution::serial_policy,
                          const index_type_of_t<Source> begin_index,
                          const index_type_of_t<Source>   end_index,
                                Destination&            destination,
                          const Source&                      source )
  {
//    if( is_empty_range( begin_index, end_index ) ){ return; }

      for( idx_t i=begin_index[0]; i<end_index[0]; ++i )
     {
         for( idx_t j=begin_index[1]; j<end_index[1]; ++j )
        {
            const index_type_of_t<Source> idx{i,j};
            destination(idx) = source(idx);
        }
     }
      return;
  }

/*
 * 3D grid
 */
   template<indexable3 Destination,
            indexable3 Source>
      requires same_grid_as<Destination,
                            Source>
            && std::assignable_from<element_type_of_t<Destination>,
                                    element_type_of_t<Source>>
   constexpr void assign(       execution::serial_policy,
                          const index_type_of_t<Source> begin_index,
                          const index_type_of_t<Source>   end_index,
                                Destination&            destination,
                          const Source&                      source )
  {
//    if( is_empty_range( begin_index, end_index ) ){ return; }

      for( idx_t i=begin_index[0]; i<end_index[0]; ++i )
     {
         for( idx_t j=begin_index[1]; j<end_index[1]; ++j )
        {
            for( idx_t k=begin_index[2]; k<end_index[2]; ++k )
           {
               const index_type_of_t<Source> idx{i,j,k};
               destination(idx) = source(idx);
           }
        }
     }
      return;
  }


/*
 * OpenMP execution --------------------------------------------------
 */

# ifdef _OPENMP
   template<indexable Destination,
            indexable Source>
      requires same_grid_as<Destination,
                            Source>
            && std::assignable_from<element_type_of_t<Destination>,
                                    element_type_of_t<Source>>
   constexpr void assign(       execution::openmp_policy,
                          const index_type_of_t<Source> begin_index,
                          const index_type_of_t<Source>   end_index,
                                Destination&            destination,
                          const Source&                      source )
  {
//    if( is_empty_range( begin_index, end_index ) ){ return; }

      using index_type = index_type_of_t<Source>;
      constexpr ndim_t ndim = ndim_of_v<Source>;

   # pragma omp parallel for
      for( idx_t i=begin_index[0]; i<end_index[0]; ++i )
     {
      // create block of only one i index, and all j,k,... etc indices
         index_type block_begin{begin_index};
         index_type block_end{end_index};

         block_begin[0]=i;
         block_end[0]=i+1;

      // each thread processes their own block sequentially
         assign( execution::seq,
                 block_begin, block_end,
                 destination, source );
     }

      return;
  }
# endif

/*
 * Convenience overloads -----------------------------------------
 */

/*
 * if no execution policy is specified, use serial
 */
   template<indexable Destination,
            indexable Source>
      requires same_grid_as<Destination,
                            Source>
            && std::assignable_from<element_type_of_t<Destination>,
                                    element_type_of_t<Source>>
   constexpr void assign( const index_type_of_t<Source> begin_index,
                          const index_type_of_t<Source>   end_index,
                                Destination&            destination,
                          const Source&                      source )
  {
      assign( execution::seq,
              begin_index, end_index,
              destination, source );
      return;
  }


/*
 * ===============================================================
 *
 * yam::transform
 *    yam::transform applies the given function TransformFunc to a variadic pack of indexables
 *    Two overload types are available
 *       1) returns an indexable lambda that lazily evaluates the transform when called with an index
 *             - note this overload copies the indexable arguments to the tranform
 *             - result can later be assigned to an index range using yam::assign
 *       2) eagerly evaluate the transformation and assign result into given indexable
 *             - evaluates only over a given index range [begin_index,end_index)
 *             - can be executed according to a yam::execution_policy
 *
 * ===============================================================
 */

/*
 * lazy transform
 */
   template<typename    TransformFunc,
            indexable         Source0,
            indexable...      Sources>
      requires same_grid_as<Source0,
                            Sources...>
            && transformation<TransformFunc,
                              element_type_of_t<Source0>,
                              element_type_of_t<Sources>...>
   [[nodiscard]]
   constexpr auto transform( TransformFunc&& transform_func,
                             Source0&&              source0,
                             Sources&&...           sources ) -> indexable auto
  {
      using index_type = common_index_type_t<Source0,Sources...>;

   // capture by value in case returned lambda outlives original sources/transform_func.
   // forwarding allows move construction if sources/transform_func are rvalues.

      return [ tfunc{std::forward<TransformFunc>(transform_func)},
               src0{std::forward<Source0>(source0)},
               ...srcs{std::forward<Sources>(sources)} ]
            ( const index_type idx )
           {
               return std::invoke( tfunc, src0(idx), srcs(idx)... );
           };
  }

/*
 * eager transform
 */
   template<typename    TransformFunc,
            indexable     Destination,
            indexable...      Sources>
      requires same_grid_as<Destination,
                            Sources...>
            && transformation_r<TransformFunc,
                                element_type_of_t<Destination>,
                                element_type_of_t<Sources>...>
   constexpr void transform( const execution_policy auto             policy,
                             const index_type_of_t<Destination> begin_index,
                             const index_type_of_t<Destination>   end_index,
                                   Destination&                 destination,
                                   TransformFunc&&           transform_func,
                                   Sources&&...                     sources )
  {
   // need to create a view (window) of each source to avoid copying entire array into lazy transform adaptor
      assign( policy,
              begin_index, end_index,
              destination,
              transform( std::forward<TransformFunc>(transform_func),
                         window(std::forward<Sources>(sources))... ) );
  }

/*
 * Convenience overload for eager serial execution
 */
   template<typename    TransformFunc,
            indexable     Destination,
            indexable...      Sources>
      requires same_grid_as<Destination,
                            Sources...>
            && transformation_r<TransformFunc,
                                element_type_of_t<Destination>,
                                element_type_of_t<Sources>...>
   constexpr void transform( const index_type_of_t<Destination> begin_index,
                             const index_type_of_t<Destination>   end_index,
                                   Destination&                 destination,
                                   TransformFunc&&           transform_func,
                                   Sources&&...                     sources )
  {
      transform( execution::seq,
                 begin_index, end_index,
                 destination,
                 std::forward<TransformFunc>(transform_func),
                 std::forward<Sources>(sources)... );
  }

/*
 * ===============================================================
 *
 * yam::reduce
 *    Reduces the range [begin_index,end_index), in unspecified order, along with the initial value init over reduction_func
 *       Performs a left-fold over the range in an (unspecified) order - ie requires reduce_func(std::move(init),source(index))
 *    Currently only serial execution available
 *
 * ===============================================================
 */

   template<typename ReduceFunc,
            typename ReduceType,
            indexable    Source>
      requires reduction<ReduceFunc,
                         ReduceType,
                         element_type_of_t<Source>>
   [[nodiscard]]
   constexpr ReduceType reduce(       execution::serial_policy,
                                const index_type_of_t<Source> begin_index,
                                const index_type_of_t<Source>   end_index,
                                      ReduceFunc&&            reduce_func,
                                      ReduceType                     init,
                                      Source&&                     source )
  {
      using index_type   =   index_type_of_t<Source>;
      using element_type = element_type_of_t<Source>;

   // always return reference to current reduction-value (init)
      const auto init_wrapper =
         [ &init ]
         ( const index_type ) -> ReduceType&
        {
            return init;
        };

   // return reduction of ( init , element )
      const auto reducer =
         [ &init, rfunc{std::forward<ReduceFunc>(reduce_func)} ]
         ( const element_type& elem )
        {
            return std::invoke( rfunc,
                                std::move(init), elem );
        };

   // at each element, executes: init=reduce_func(init,element)
      transform( execution::seq,
                 begin_index, end_index,
                 std::move(init_wrapper),
                 std::move(reducer),
                 std::forward<Source>(source) );

      return init;
  }

/*
 * Convenience overload assumes serial evaluation
 */
   template<typename ReduceFunc,
            typename ReduceType,
            indexable    Source>
      requires reduction<ReduceFunc,
                         ReduceType,
                         element_type_of_t<Source>>
   [[nodiscard]]
   constexpr ReduceType reduce( const index_type_of_t<Source> begin_index,
                                const index_type_of_t<Source>   end_index,
                                      ReduceFunc&&            reduce_func,
                                      ReduceType&&                   init,
                                      Source&&                     source )
  {
      return reduce( execution::seq,
                     begin_index, end_index,
                     std::forward<ReduceFunc>(reduce_func),
                     std::forward<ReduceType>(init),
                     std::forward<Source>(source) );
  }

/*
 * ===============================================================
 *
 * yam::transform_reduce
 *    Applies transform_func to each set of elements at the same index in the range [begin_index, end_index) from the indexable Sources and reduces the results (possibly permuted and aggregated in unspecified manner) along with the initial value init over reduce_func
 *
 * ===============================================================
 */

   template<typename    TransformFunc,
            typename       ReduceFunc,
            typename       ReduceType,
            indexable         Source0,
            indexable...      Sources>
      requires same_grid_as<Source0,
                            Sources...>
            && transformation_reduction<TransformFunc,
                                        ReduceFunc,
                                        ReduceType,
                                        element_type_of_t<Source0>,
                                        element_type_of_t<Sources>...>
   [[nodiscard]]
   constexpr ReduceType transform_reduce(       execution_policy auto         policy,
                                          const index_type_of_t<Source0> begin_index,
                                          const index_type_of_t<Source0>   end_index,
                                                TransformFunc&&       transform_func,
                                                ReduceFunc&&             reduce_func,
                                                ReduceType&&                    init,
                                                Source0&&                    source0,
                                                Sources&&...                 sources )
  {
      return reduce( policy,
                     begin_index, end_index,
                     std::forward<ReduceFunc>(reduce_func),
                     std::forward<ReduceType>(init),
                     transform( std::forward<TransformFunc>(transform_func),
                                window(std::forward<Source0>(source0)),
                                window(std::forward<Sources>(sources))... ) );
  }

/*
 * Convenience overload assumes serial evaluation
 */
   template<typename    TransformFunc,
            typename       ReduceFunc,
            typename       ReduceType,
            indexable         Source0,
            indexable...      Sources>
      requires same_grid_as<Source0,
                            Sources...>
            && transformation_reduction<TransformFunc,
                                        ReduceFunc,
                                        ReduceType,
                                        element_type_of_t<Source0>,
                                        element_type_of_t<Sources>...>
   [[nodiscard]]
   constexpr ReduceType transform_reduce( const index_type_of_t<Source0> begin_index,
                                          const index_type_of_t<Source0>   end_index,
                                                TransformFunc&&       transform_func,
                                                ReduceFunc&&             reduce_func,
                                                ReduceType&&                    init,
                                                Source0&&                    source0,
                                                Sources&&...                 sources )
  {
      return transform_reduce( execution::seq,
                               begin_index, end_index,
                               std::forward<TransformFunc>(transform_func),
                               std::forward<ReduceFunc>(reduce_func),
                               std::forward<ReduceType>(init),
                               std::forward<Source0>(source0),
                               std::forward<Sources>(sources)... );
  }
}
