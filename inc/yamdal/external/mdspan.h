
# pragma once

# include <experimental/mdspan>

namespace stx = std::experimental;

namespace yam
{
/*
 * calculation of number of elements
 */
// number of elements in a grid of a given shape
   template<ptrdiff_t... Exts>
   [[nodiscard]]
   constexpr size_t num_elems( const stx::extents<Exts...> exts )
  {
      return [&]<auto... Idxs>( std::index_sequence<Idxs...> ) -> size_t
     {
         return size_t((std::max<ptrdiff_t>(0,exts.extent(Idxs))*...));
     }(std::make_index_sequence<sizeof...(Exts)>());
  }

   template<ptrdiff_t... Exts>
   [[nodiscard]]
   constexpr bool is_empty_range( const stx::extents<Exts...> exts )
  {
      return num_elems(exts)==0;
  }
}

