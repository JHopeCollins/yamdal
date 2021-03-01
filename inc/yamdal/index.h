
# pragma once

# include <array>
# include <concepts>
# include <utility>
# include <cstddef>

namespace yam
{

/*
 * integer types used for number of dimensions and for indexing
 */
   using ndim_t = unsigned short int;
   using idx_t = ptrdiff_t;

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

      using index_type = index<ndim,grid>;

      std::array<idx_t,ndim> idxs;

      [[nodiscard]]
      constexpr idx_t& operator[]( const ndim_t i ) { return idxs[i]; }

      [[nodiscard]]
      constexpr const idx_t& operator[]( const ndim_t i ) const { return idxs[i]; }
  };

// deduction guide
   template<std::convertible_to<idx_t>... Is>
   index( Is... )
      -> index<ndim_t(sizeof...(Is))>;

/*
 * index comparisons
 */

   template<ndim_t ndim,
            grid_t grid>
   [[nodiscard]]
   constexpr bool operator==( const index<ndim,grid> lhs,
                              const index<ndim,grid> rhs )
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
   constexpr bool operator!=( const index<ndim,grid> lhs,
                              const index<ndim,grid> rhs )
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
 */

/*
 * convert type between primal/dual grids
 */
   template<grid_t grid,
            typename  T>
      requires requires( T t ){ to_primal(t); to_dual(t); }
   [[nodiscard]]
   constexpr auto to_grid( const T& t )
  {
      if constexpr( grid == primal )
     {
         return to_primal( t );
     }
      else /*     ( grid == dual ) */
     {
         return to_dual( t );
     }
  }

/*
 * convert index between primal and dual grids (no change in indices, just template tags)
 */
   template<ndim_t ndim,
            grid_t grid>
   [[nodiscard]]
   constexpr auto to_primal( const index<ndim,grid> idx )
      -> primal_index<ndim>
  {
      return {idx.idxs};
  }

   template<ndim_t ndim,
            grid_t grid>
   [[nodiscard]]
   constexpr auto to_dual( const index<ndim,grid> idx )
      -> dual_index<ndim>
  {
      return {idx.idxs};
  }

// number of elements in a grid range with given begin/end indices
   template<ndim_t ndim,
            grid_t grid>
   [[nodiscard]]
   constexpr size_t num_elems( const index<ndim,grid> begin_index,
                               const index<ndim,grid>   end_index )
  {
      return [&]<auto... Idxs>( std::index_sequence<Idxs...> ) -> size_t
     {
         return ((end_index[Idxs]-begin_index[Idxs])*...);
     }(std::make_index_sequence<ndim>());
  }

   template<ndim_t ndim,
            grid_t grid>
   [[nodiscard]]
   constexpr bool is_empty_range( const index<ndim,grid> begin_index,
                                  const index<ndim,grid>   end_index )
  {
      return num_elems(begin_index,end_index)==0;
  }

}

