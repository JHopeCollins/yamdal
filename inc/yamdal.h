
# pragma once

# include <array>
# include <concepts>
# include <utility>

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
   template<ndim_t NDIM, grid_t GRID> struct index_range;

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

      std::array<idx_t,ndim> idxs;

      [[nodiscard]]
      constexpr idx_t& operator[]( const ndim_t i ) { return idxs[i]; }

      [[nodiscard]]
      constexpr const idx_t& operator[]( const ndim_t i ) const { return idxs[i]; }
  };

// deduction guide
   template<std::integral... Is>
   index( Is... )
      -> index<sizeof...(Is),primal>;

/*
 * index comparisons
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
 * yam::index_range
 * type definition and associated functions and type aliases
 *
 * ===============================================================
 */

/*
 * A semi-open range of indices in ndims, [begin_index,end_index)
 *    if constructed from single index, assumes begin_index is zero in every dimension
 */
   template<ndim_t NDIM,
            grid_t GRID= primal>
   struct index_range
  {
      static constexpr ndim_t ndim = NDIM;
      static constexpr grid_t grid = GRID;

      using index_type = index<ndim,grid>;

      index_type begin_index;
      index_type   end_index;

      index_range() = default;
      index_range( const index_range&  ) = default;
      index_range(       index_range&& ) = default;

      constexpr explicit index_range( const index_type& end_idx )
         : begin_index{0}, end_index(end_idx) {}

      constexpr explicit index_range( const index_type& begin_idx,
                                      const index_type&   end_idx )
         : begin_index(begin_idx), end_index(end_idx) {}

      index_range& operator=( const index_range&  ) = default;
      index_range& operator=(       index_range&& ) = default;

      [[nodiscard]]
      constexpr idx_t extent( const ndim_t i ) const
     {
         if( begin_index[i] < end_index[i] )
        {
            return end_index[i] - begin_index[i];
        }
         else
        {
            return 0;
        }
     }
  };

// deduction guides
   template<ndim_t ndim,
            grid_t grid>
   explicit index_range( const index<ndim,grid>& )
      -> index_range<ndim,grid>;

   template<ndim_t ndim,
            grid_t grid>
   explicit index_range( const index<ndim,grid>&,
                         const index<ndim,grid>& )
      -> index_range<ndim,grid>;

// access begin/end indices
   template<ndim_t ndim,
            grid_t grid>
   [[nodiscard]]
   constexpr index<ndim,grid> begin_index( const index_range<ndim,grid>& irng )
  {
      return irng.begin_index;
  }

   template<ndim_t ndim,
            grid_t grid>
   [[nodiscard]]
   constexpr index<ndim,grid> end_index( const index_range<ndim,grid>& irng )
  {
      return irng.end_index;
  }


// comparisons

   template<ndim_t ndim,
            grid_t grid>
   [[nodiscard]]
   constexpr bool operator==( const index_range<ndim,grid>& lhs,
                              const index_range<ndim,grid>& rhs )
  {
      return ( begin_index(lhs) == begin_index(rhs) )
             &&( end_index(lhs) ==   end_index(rhs) );
  }

   template<ndim_t ndim,
            grid_t grid>
   [[nodiscard]]
   constexpr bool operator!=( const index_range<ndim,grid>& lhs,
                              const index_range<ndim,grid>& rhs )
  {
      return !(lhs==rhs);
  }

// convenience typedefs

   // dimensions
   template<grid_t grid= primal>
   using index_range1 = index_range<1,grid>;

   template<grid_t grid= primal>
   using index_range2 = index_range<2,grid>;

   template<grid_t grid= primal>
   using index_range3 = index_range<3,grid>;

   // grid type
   template<ndim_t ndim>
   using primal_index_range = index_range<ndim,primal>;

   template<ndim_t ndim>
   using dual_index_range = index_range<ndim,dual>;

   // dimensions and grid type
   using primal_index_range1 = index_range<1,primal>;
   using primal_index_range2 = index_range<2,primal>;
   using primal_index_range3 = index_range<3,primal>;

   using   dual_index_range1 = index_range<1,dual>;
   using   dual_index_range2 = index_range<2,dual>;
   using   dual_index_range3 = index_range<3,dual>;

/*
 * ===============================================================
 */

/*
 * convert index between primal and dual grids (no change in indices, just template tags)
 */
   template<ndim_t ndim>
   [[nodiscard]]
   constexpr auto to_primal( const primal_index<ndim>& pidx )
      -> primal_index<ndim>
  {
      return pidx;
  }

   template<ndim_t ndim>
   [[nodiscard]]
   constexpr auto to_primal( const dual_index<ndim>& didx )
      -> primal_index<ndim>
  {
      primal_index<ndim> pidx;
      for( ndim_t i=0; i<ndim; ++i ){ pidx[i]=didx[i]; }
      return pidx;
  }

   template<ndim_t ndim>
   [[nodiscard]]
   constexpr auto to_dual( const dual_index<ndim>& didx )
      -> dual_index<ndim>
  {
      return didx;
  }

   template<ndim_t ndim>
   [[nodiscard]]
   constexpr auto to_dual( const primal_index<ndim>& pidx )
      -> dual_index<ndim>
  {
      dual_index<ndim> didx;
      for( ndim_t i=0; i<ndim; ++i ){ didx[i]=pidx[i]; }
      return didx;
  }

/*
 * convert index_ranges between primal and dual grids (dual extent = primal extent - 1)
 */
   template<ndim_t ndim>
   [[nodiscard]]
   constexpr auto to_primal( const primal_index_range<ndim>& prng )
      -> primal_index_range<ndim>
  {
      return prng;
  }

   template<ndim_t ndim>
   [[nodiscard]]
   constexpr auto to_primal( const dual_index_range<ndim>& drng )
      -> primal_index_range<ndim>
  {
      primal_index_range<ndim> prng(to_primal(begin_index(drng)),
                                    to_primal(  end_index(drng)));
      for( ndim_t i=0; i<ndim; ++i )
     {
         prng.end_index[i]+=1;
     }
      return prng;
  }

   template<ndim_t ndim>
   [[nodiscard]]
   constexpr auto to_dual( const dual_index_range<ndim>& drng )
      -> dual_index_range<ndim>
  {
      return drng;
  }

   template<ndim_t ndim>
   [[nodiscard]]
   constexpr auto to_dual( const primal_index_range<ndim>& prng )
      -> dual_index_range<ndim>
  {
      dual_index_range<ndim> drng(to_dual(begin_index(prng)),
                                  to_dual(  end_index(prng)));
      for( ndim_t i=0; i<ndim; ++i )
     {
         drng.end_index[i]-=1;
     }
      return drng;
  }

/*
 * calculation of number of elements
 */
// number of elements in a grid of a given shape
   template<ndim_t ndim,
            grid_t grid>
   constexpr size_t num_elems( const index_range<ndim,grid>& irng )
  {
      size_t num=1;
      for( ndim_t i=0; i<ndim; ++i )
     {
         num*=irng.extent(i);
     }
      return num;
  }

// number of elements in a grid range with given begin/end indices
   template<ndim_t ndim,
            grid_t grid>
   constexpr size_t num_elems( const index<ndim,grid>& begin_index,
                               const index<ndim,grid>&   end_index )
  {
      return num_elems( index_range(begin_index,end_index) );
  }

}

