
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
   template<ndim_t NDIM, grid_t GT> struct index;
   template<ndim_t NDIM, grid_t GT> struct shape;

/*
 * a multi-dimensional index into a grid of ndim dimensions and grid_type type
 */
   template<ndim_t NDIM,
            grid_t   GT= primal>
   struct index
  {
      static constexpr ndim_t ndim = NDIM;
      static constexpr grid_t grid = GT;

      using shape_type = shape<ndim,grid>;

      std::array<idx_t,ndim> idxs;

      [[nodiscard]]
      constexpr const idx_t& operator[]( const ndim_t i ) const { return idxs[i]; }
  };

// comparisons

   template<ndim_t NDIM,
            grid_t   GT>
   [[nodiscard]]
   constexpr bool operator==( const index<NDIM,GT>& lhs,
                              const index<NDIM,GT>& rhs )
  {
      bool eq=true;
      for( ndim_t i=0; i<NDIM; ++i )
     {
         eq = eq and (lhs[i]==rhs[i]);
     }
      return eq;
  }

   template<ndim_t NDIM,
            grid_t   GT>
   [[nodiscard]]
   constexpr bool operator!=( const index<NDIM,GT>& lhs,
                              const index<NDIM,GT>& rhs )
  {
      return !(lhs==rhs);
  }

   template<ndim_t NDIM,
            grid_t   GT>
   [[nodiscard]]
   constexpr bool operator<=( const index<NDIM,GT>& lhs,
                              const index<NDIM,GT>& rhs )
  {
      bool le=true;
      for( ndim_t i=0; i<NDIM; ++i )
     {
         le = le and (lhs[i]<=rhs[i]);
     }
      return le;
  }

   template<ndim_t NDIM,
            grid_t   GT>
   [[nodiscard]]
   constexpr bool operator<( const index<NDIM,GT>& lhs,
                             const index<NDIM,GT>& rhs )
  {
      return (lhs<=rhs) and (lhs!=rhs);
  }

// convenience typedefs

   // dimensions
   template<grid_t GT>
   using index1 = index<1,GT>;

   template<grid_t GT>
   using index2 = index<2,GT>;

   template<grid_t GT>
   using index3 = index<3,GT>;

   // grid type
   template<ndim_t NDIM>
   using primal_index = index<NDIM,primal>;

   template<ndim_t NDIM>
   using dual_index = index<NDIM,dual>;

   // dimensions and grid type
   using primal_index1 = index<1,primal>;
   using primal_index2 = index<2,primal>;
   using primal_index3 = index<3,primal>;

   using   dual_index1 = index<1,dual>;
   using   dual_index2 = index<2,dual>;
   using   dual_index3 = index<3,dual>;

/*
 * The shape of a multi-dimensional grid
 *    stores the extent of the grid in each dimension
 *    assumes each dimension starts at zero
 */
   template<ndim_t NDIM,
            grid_t   GT= primal>
   struct shape
  {
      static constexpr ndim_t ndim = NDIM;
      static constexpr grid_t grid = GT;

      using index_type = index<ndim,grid>;

      std::array<idx_t,ndim> extents;

      [[nodiscard]]
      constexpr const idx_t& operator[]( const ndim_t i ) const { return extents[i]; }
  };

// comparisons

   template<ndim_t NDIM,
            grid_t   GT>
   [[nodiscard]]
   constexpr bool operator==( const shape<NDIM,GT>& lhs,
                              const shape<NDIM,GT>& rhs )
  {
      bool eq=true;
      for( ndim_t i=0; i<NDIM; ++i )
     {
         eq = eq and (lhs[i]==rhs[i]);
     }
      return eq;
  }

   template<ndim_t NDIM,
            grid_t   GT>
   [[nodiscard]]
   constexpr bool operator!=( const shape<NDIM,GT>& lhs,
                              const shape<NDIM,GT>& rhs )
  {
      return !(lhs==rhs);
  }

   template<ndim_t NDIM,
            grid_t   GT>
   [[nodiscard]]
   constexpr bool operator<=( const shape<NDIM,GT>& lhs,
                              const shape<NDIM,GT>& rhs )
  {
      bool le=true;
      for( ndim_t i=0; i<NDIM; ++i )
     {
         le = le and (lhs[i]<=rhs[i]);
     }
      return le;
  }

   template<ndim_t NDIM,
            grid_t   GT>
   [[nodiscard]]
   constexpr bool operator<( const shape<NDIM,GT>& lhs,
                             const shape<NDIM,GT>& rhs )
  {
      return (lhs<=rhs) and (lhs!=rhs);
  }

// convenience typedefs

   // dimensions
   template<grid_t GT>
   using shape1 = shape<1,GT>;

   template<grid_t GT>
   using shape2 = shape<2,GT>;

   template<grid_t GT>
   using shape3 = shape<3,GT>;

   // grid type
   template<ndim_t NDIM>
   using primal_shape = shape<NDIM,primal>;

   template<ndim_t NDIM>
   using dual_shape = shape<NDIM,dual>;

   // dimensions and grid type
   using primal_shape1 = shape<1,primal>;
   using primal_shape2 = shape<2,primal>;
   using primal_shape3 = shape<3,primal>;

   using   dual_shape1 = shape<1,dual>;
   using   dual_shape2 = shape<2,dual>;
   using   dual_shape3 = shape<3,dual>;

/*
 * convert shapes from primal to dual grid (dual extent = primal extent - 1)
 */
   template<ndim_t NDIM>
   [[nodiscard]]
   constexpr auto to_primal( const primal_shape<NDIM>& pshape )
  {
      return pshape;
  }

   template<ndim_t NDIM>
   [[nodiscard]]
   constexpr auto to_primal( const dual_shape<NDIM>& dshape )
  {
      primal_shape<NDIM> pshape{};
      for( ndim_t i=0; i<NDIM; ++i )
     {
         pshape.extents[i] = dshape[i]+1;
     }
      return pshape;
  }

   template<ndim_t NDIM>
   [[nodiscard]]
   constexpr auto to_dual( const dual_shape<NDIM>& dshape )
  {
      return dshape;
  }

   template<ndim_t NDIM>
   [[nodiscard]]
   constexpr auto to_dual( const primal_shape<NDIM>& pshape )
  {
      dual_shape<NDIM> dshape{};
      for( ndim_t i=0; i<NDIM; ++i )
     {
         dshape.extents[i] = pshape[i]-1;
     }
      return dshape;
  }

}

