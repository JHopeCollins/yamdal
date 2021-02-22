
# pragma once

# include "index.h"

# include <array>

namespace yam
{
/*
 * the strides needed to flatten a multi-dimensional index to a 1D contiguous index
 *    used to map from an ndim index to a location in the underlying memory
 *    outermost index varies fastest (row-major / C-style indexing)
 *    no padding used
 */
   template<ndim_t NDIM,
            grid_t GRID= primal>
   struct stride
  {
      static constexpr ndim_t ndim = NDIM;
      static constexpr grid_t grid = GRID;

      using index_type = index<ndim,grid>;

      std::array<idx_t,ndim> strides;

      stride() = default;
      stride( const stride&  ) = default;
      stride(       stride&& ) = default;

   // construct strides from a grid index_range
      constexpr explicit stride( const index_range<ndim,grid>& irng )
     {
         strides[ndim-1]=1;
         ndim_t i=ndim-2;
         while( i!=0 )
        {
            strides[i] = strides[i+1]*irng.extent(i+1);
            --i;
        }
     }

      [[nodiscard]]
      constexpr idx_t& operator[]( const ndim_t i ) { return strides[i]; }

      [[nodiscard]]
      constexpr const idx_t& operator[]( const ndim_t i ) const { return strides[i]; }

      stride& operator=( const stride&  ) = default;
      stride& operator=(       stride&& ) = default;
  };

// convenience typedefs

   // dimensions
   template<grid_t grid= primal>
   using stride1 = stride<1,grid>;

   template<grid_t grid= primal>
   using stride2 = stride<2,grid>;

   template<grid_t grid= primal>
   using stride3 = stride<3,grid>;

   // grid type
   template<ndim_t ndim>
   using primal_stride = stride<ndim,primal>;

   template<ndim_t ndim>
   using dual_stride = stride<ndim,dual>;

   // dimensions and grid type
   using primal_stride1 = stride<1,primal>;
   using primal_stride2 = stride<2,primal>;
   using primal_stride3 = stride<3,primal>;

   using   dual_stride1 = stride<1,dual>;
   using   dual_stride2 = stride<2,dual>;
   using   dual_stride3 = stride<3,dual>;

}
