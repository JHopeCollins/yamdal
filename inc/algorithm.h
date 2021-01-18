
# pragma once

# include <yamdal.h>

# include <concepts>

namespace yam
{
/*
 * Assign values in range [min_index,max_index) from one 1D indexable to another 1D indexable
 */
   template<indexable1 SRC,
            indexable1 DST>
      requires have_same_grid_v<SRC,DST> &&
               std::assignable_from<element_type_of_t<DST>,
                                    element_type_of_t<SRC>>
   constexpr void assign( const index_type_of_t<SRC>& min_index,
                          const index_type_of_t<SRC>& max_index,
                          const SRC&  src,
                                DST&  dst )
  {
      for( idx_t i=min_index[0]; i<max_index[0]; ++i )
     {
         const index_type_of_t<SRC> idx{i};
         dst(idx) = src(idx);
     }
      return;
  }
}
