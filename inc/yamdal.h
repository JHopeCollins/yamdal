
# pragma once

# include <array>

namespace yam
{
/*
 * integer types used for number of dimensions and for indexing
 */
   using ndim_t = unsigned short int;
   using idx_t = size_t;

/*
 * Indicates type of the grid that an indexable is defined over - primal or dual
 *    Primal is the standard grid (should be used as default unless both primal and dual grids are needed).
 *    Dual grid is used for cell-based grids with primal node-based grids
 */
   enum struct grid_t { primal, dual };
   inline constexpr grid_t primal = grid_t::primal;
   inline constexpr grid_t   dual = grid_t::dual;

/*
 * declarations
 */
   template<ndim_t NDIM, grid_t GRID> struct index;
   template<ndim_t NDIM, grid_t GRID> struct shape;

/*
 * ===============================================================
 *
 * yam::index
 * type definition and associated functions and type aliases
 *
 * ===============================================================
 */

/*
 * a multi-dimensional index into a grid of ndim dimensions and grid_type type
 */
   template<ndim_t NDIM,
            grid_t GRID= primal>
   struct index
  {
      static constexpr ndim_t ndim = NDIM;
      static constexpr grid_t grid = GRID;

      using shape_type = shape<ndim,grid>;

      std::array<idx_t,ndim> idxs;

      [[nodiscard]]
      constexpr const idx_t& operator[]( const ndim_t i ) const { return idxs[i]; }
  };

/*
 * index comparisons
 *    Note that the less-than operator i0<i1 is intended to test whether the the set of indices [i0,i1) is not empty. This means that it does not provide an ordering over indices of dimension greater than 1, so cannot be used in sorting algorithms.
 *    To see this, consider two 2D indices:
 *       index2 i0{0,1};
 *       index2 i1{1,0};
 *    Neither index is smaller than the other, and they are not equal to each other. Hence they cannot be ordered (totally or partially).
 *       assert( !(i0<i1) );  // passes
 *       assert( !(i1<i0) );  // passes
 *       assert( !(i1==i0) ); // passes
 */

   template<ndim_t ndim,
            grid_t grid>
   [[nodiscard]]
   constexpr bool operator==( const index<ndim,grid>& lhs,
                              const index<ndim,grid>& rhs )
  {
      bool eq=true;
      for( ndim_t i=0; i<ndim; ++i )
     {
         eq = eq and (lhs[i]==rhs[i]);
     }
      return eq;
  }

   template<ndim_t ndim,
            grid_t grid>
   [[nodiscard]]
   constexpr bool operator!=( const index<ndim,grid>& lhs,
                              const index<ndim,grid>& rhs )
  {
      return !(lhs==rhs);
  }

   template<ndim_t ndim,
            grid_t grid>
   [[nodiscard]]
   constexpr bool operator<( const index<ndim,grid>& lhs,
                             const index<ndim,grid>& rhs )
  {
      bool lt=true;
      for( ndim_t i=0; i<ndim; ++i )
     {
         lt = lt and (lhs[i]<rhs[i]);
     }
      return lt;
  }

// convenience typedefs

   // dimensions
   template<grid_t grid= primal>
   using index1 = index<1,grid>;

   template<grid_t grid= primal>
   using index2 = index<2,grid>;

   template<grid_t grid= primal>
   using index3 = index<3,grid>;

   // grid type
   template<ndim_t ndim>
   using primal_index = index<ndim,primal>;

   template<ndim_t ndim>
   using dual_index = index<ndim,dual>;

   // dimensions and grid type
   using primal_index1 = index<1,primal>;
   using primal_index2 = index<2,primal>;
   using primal_index3 = index<3,primal>;

   using   dual_index1 = index<1,dual>;
   using   dual_index2 = index<2,dual>;
   using   dual_index3 = index<3,dual>;

/*
 * ===============================================================
 *
 * yam::shape
 * type definition and associated functions and type aliases
 *
 * ===============================================================
 */

/*
 * The shape of a multi-dimensional grid
 *    stores the extent of the grid in each dimension
 *    assumes each dimension starts at zero
 */
   template<ndim_t NDIM,
            grid_t GRID= primal>
   struct shape
  {
      static constexpr ndim_t ndim = NDIM;
      static constexpr grid_t grid = GRID;

      using index_type = index<ndim,grid>;

      std::array<idx_t,ndim> extents;

      [[nodiscard]]
      constexpr const idx_t& operator[]( const ndim_t i ) const { return extents[i]; }
  };

// comparisons

   template<ndim_t ndim,
            grid_t grid>
   [[nodiscard]]
   constexpr bool operator==( const shape<ndim,grid>& lhs,
                              const shape<ndim,grid>& rhs )
  {
      bool eq=true;
      for( ndim_t i=0; i<ndim; ++i )
     {
         eq = eq and (lhs[i]==rhs[i]);
     }
      return eq;
  }

   template<ndim_t ndim,
            grid_t   grid>
   [[nodiscard]]
   constexpr bool operator!=( const shape<ndim,grid>& lhs,
                              const shape<ndim,grid>& rhs )
  {
      return !(lhs==rhs);
  }

   template<ndim_t ndim,
            grid_t   grid>
   [[nodiscard]]
   constexpr bool operator<=( const shape<ndim,grid>& lhs,
                              const shape<ndim,grid>& rhs )
  {
      bool le=true;
      for( ndim_t i=0; i<ndim; ++i )
     {
         le = le and (lhs[i]<=rhs[i]);
     }
      return le;
  }

   template<ndim_t ndim,
            grid_t   grid>
   [[nodiscard]]
   constexpr bool operator<( const shape<ndim,grid>& lhs,
                             const shape<ndim,grid>& rhs )
  {
      return (lhs<=rhs) and (lhs!=rhs);
  }

// convenience typedefs

   // dimensions
   template<grid_t grid= primal>
   using shape1 = shape<1,grid>;

   template<grid_t grid= primal>
   using shape2 = shape<2,grid>;

   template<grid_t grid= primal>
   using shape3 = shape<3,grid>;

   // grid type
   template<ndim_t ndim>
   using primal_shape = shape<ndim,primal>;

   template<ndim_t ndim>
   using dual_shape = shape<ndim,dual>;

   // dimensions and grid type
   using primal_shape1 = shape<1,primal>;
   using primal_shape2 = shape<2,primal>;
   using primal_shape3 = shape<3,primal>;

   using   dual_shape1 = shape<1,dual>;
   using   dual_shape2 = shape<2,dual>;
   using   dual_shape3 = shape<3,dual>;

/*
 * ===============================================================
 */

/*
 * convert shapes from primal to dual grid (dual extent = primal extent - 1)
 */
   template<ndim_t ndim>
   [[nodiscard]]
   constexpr auto to_primal( const primal_shape<ndim>& pshape )
  {
      return pshape;
  }

   template<ndim_t ndim>
   [[nodiscard]]
   constexpr auto to_primal( const dual_shape<ndim>& dshape )
  {
      primal_shape<ndim> pshape{};
      for( ndim_t i=0; i<ndim; ++i )
     {
         pshape.extents[i] = dshape[i]+1;
     }
      return pshape;
  }

   template<ndim_t ndim>
   [[nodiscard]]
   constexpr auto to_dual( const dual_shape<ndim>& dshape )
  {
      return dshape;
  }

   template<ndim_t ndim>
   [[nodiscard]]
   constexpr auto to_dual( const primal_shape<ndim>& pshape )
  {
      dual_shape<ndim> dshape{};
      for( ndim_t i=0; i<ndim; ++i )
     {
         dshape.extents[i] = pshape[i]-1;
     }
      return dshape;
  }

/*
 * calculation of number of elements
 */
// number of elements in a grid of a given shape
   template<ndim_t ndim,
            grid_t grid>
   constexpr size_t num_elems( const shape<ndim,grid>& shape )
  {
      size_t num=1;
      for( ndim_t i=0; i<ndim; ++i )
     {
         num*=shape[i];
     }
      return num;
  }

// number of elements in a grid range with given begin/end indices
   template<ndim_t ndim,
            grid_t grid>
   constexpr size_t num_elems( const index<ndim,grid>& begin_index,
                               const index<ndim,grid>&   end_index )
  {
      size_t num=1;
      for( ndim_t i=0; i<ndim; ++i )
     {
         if( end_index[i] > begin_index[i] )
        {
            num*=end_index[i]-begin_index[i];
        }
         else
        {
            num = 0;
            break;
        }
     }
      return num;
  }

}

