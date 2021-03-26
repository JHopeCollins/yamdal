
# pragma once

# include "../concepts.h"
# include "../index.h"
# include "../execution.h"

# include "../external/mdspan.h"

# include <concepts>
# include <type_traits>
# include <functional>
# include <utility>

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
 * 1D grid
 */
   template<indexable1 Destination,
            indexable1 Source,
            ptrdiff_t... Exts>
      requires same_grid_as<Destination,
                            Source>
            && std::assignable_from<element_type_of_t<Destination>,
                                    element_type_of_t<Source>>
            && (sizeof...(Exts)==1)
   constexpr void assign(       execution::serial_policy,
                          const index_type_of_t<Source> begin_index,
                          const stx::extents<Exts...>          exts,
                                Destination&            destination,
                          const Source&                      source )
  {
      const auto i0 = begin_index[0];

      for( idx_t i=i0; i<i0+exts.extent(0); ++i )
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
            indexable2 Source,
            ptrdiff_t... Exts>
      requires same_grid_as<Destination,
                            Source>
            && std::assignable_from<element_type_of_t<Destination>,
                                    element_type_of_t<Source>>
            && (sizeof...(Exts)==2)
   constexpr void assign(       execution::serial_policy,
                          const index_type_of_t<Source> begin_index,
                          const stx::extents<Exts...>          exts,
                                Destination&            destination,
                          const Source&                      source )
  {
      const auto i0 = begin_index[0];
      const auto j0 = begin_index[1];

      for( idx_t i=i0; i<i0+exts.extent(0); ++i )
     {
         for( idx_t j=j0; j<j0+exts.extent(1); ++j )
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
            indexable3 Source,
            ptrdiff_t... Exts>
      requires same_grid_as<Destination,
                            Source>
            && std::assignable_from<element_type_of_t<Destination>,
                                    element_type_of_t<Source>>
            && (sizeof...(Exts)==3)
   constexpr void assign(       execution::serial_policy,
                          const index_type_of_t<Source> begin_index,
                          const stx::extents<Exts...>          exts,
                                Destination&            destination,
                          const Source&                      source )
  {
      const auto i0 = begin_index[0];
      const auto j0 = begin_index[1];
      const auto k0 = begin_index[2];

      for( idx_t i=i0; i<i0+exts.extent(0); ++i )
     {
         for( idx_t j=j0; j<j0+exts.extent(1); ++j )
        {
            for( idx_t k=k0; k<k0+exts.extent(2); ++k )
           {
               const index_type_of_t<Source> idx{i,j,k};
               destination(idx) = source(idx);
           }
        }
     }

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

   template<typename ReduceFunc,
            typename ReduceType,
            indexable    Source,
            ptrdiff_t...   Exts>
      requires reduction<ReduceFunc,
                         ReduceType,
                         element_type_of_t<Source>>
            && (sizeof...(Exts)==ndim_of_v<Source>)
   [[nodiscard]]
   constexpr ReduceType reduce(       execution::serial_policy,
                                const index_type_of_t<Source> begin_index,
                                const stx::extents<Exts...>          exts,
                                      ReduceFunc              reduce_func,
                                      ReduceType                     init,
                                const Source&                      source )
  {
      using index_type   =   index_type_of_t<Source>;
      using element_type = element_type_of_t<Source>;

   // always return reference to current reduction-value (init)
      auto init_wrapper =
         [ &init ]
         ( index_type ) -> ReduceType&
        {
            return init;
        };

   // return reduction of ( init , element )
      auto reducer =
         [ &init, &reduce_func ]
         ( const element_type& elem )
        {
            return std::invoke( reduce_func,
                                std::move(init), elem );
        };

   // at each element, executes: init=reduce_func(init,element)
      transform( execution::seq,
                 begin_index, exts,
                 init_wrapper,
                 reducer,
                 source );

      return init;
  }
}
