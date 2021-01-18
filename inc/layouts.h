
# pragma once

# include <yamdal.h>

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
            grid_t   GT= primal>
   struct stride
  {
      static constexpr ndim_t ndim = NDIM;
      static constexpr grid_t grid = GT;

      using index_type = index<ndim,grid>;
      using shape_type = shape<ndim,grid>;

      std::array<idx_t,ndim> strides;

      stride() = default;
      stride( const stride&  ) = default;
      stride(       stride&& ) = default;

   // construct strides from a grid shape
      constexpr explicit stride( const shape_type& sh )
     {
         strides[ndim-1]=1;
         for( ndim_t i=ndim-2; i>=0; --i )
        {
            strides[i] = strides[i+1]*sh[i+1];
        }
     }

      constexpr const idx_t& operator[]( const ndim_t i ) const { return strides[i]; }

      stride& operator=( const stride&  ) = default;
      stride& operator=(       stride&& ) = default;
  };

// convenience typedefs

   // dimensions
   template<grid_t GT>
   using stride1 = stride<1,GT>;

   template<grid_t GT>
   using stride2 = stride<2,GT>;

   template<grid_t GT>
   using stride3 = stride<3,GT>;

   // grid type
   template<ndim_t NDIM>
   using primal_stride = stride<NDIM,primal>;

   template<ndim_t NDIM>
   using dual_stride = stride<NDIM,dual>;

   // dimensions and grid type
   using primal_stride1 = stride<1,primal>;
   using primal_stride2 = stride<2,primal>;
   using primal_stride3 = stride<3,primal>;

   using   dual_stride1 = stride<1,dual>;
   using   dual_stride2 = stride<2,dual>;
   using   dual_stride3 = stride<3,dual>;

}
