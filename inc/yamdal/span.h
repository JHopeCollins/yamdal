
# pragma once

# include "concepts.h"
# include "type_traits.h"
# include "index.h"

# include "external/mdspan.h"

namespace yam
{
// default policy types for spans

   using default_layout = stx::layout_right;

   template<typename ElementType>
   using default_accessor = stx::accessor_basic<ElementType>;

/*
 * ===============================================================
 *
 * yam::span
 *    thin wrapper around mdspan, adding an operator()(yam::index) and relevant members eg ndim, grid
 *
 * ===============================================================
 */
   
// declaration
   template<typename    ElementType,
            typename        Extents,
            typename   LayoutPolicy = default_layout,
            typename AccessorPolicy = default_accessor<ElementType>,
            grid_t             GRID = primal>
   class basic_span;

// only allowed specialisation
   template<typename    ElementType,
            ptrdiff_t...       Exts,
            typename   LayoutPolicy,
            typename AccessorPolicy,
            grid_t             GRID>
   class basic_span<ElementType,
                    stx::extents<Exts...>,
                    LayoutPolicy,
                    AccessorPolicy,
                    GRID>
      : public stx::basic_mdspan<ElementType,
                                 stx::extents<Exts...>,
                                 LayoutPolicy,
                                 AccessorPolicy>
  {
   private :

      using base_t = stx::basic_mdspan<ElementType,
                                 stx::extents<Exts...>,
                                 LayoutPolicy,
                                 AccessorPolicy>;

   public :

      static constexpr ndim_t ndim = sizeof...(Exts);
      static constexpr grid_t grid = GRID;

   // only index_type is different from stx::basic_mdspan to facilitate `indexable` concept
      using index_type = index<ndim,grid>;

      using typename base_t::element_type;
      using typename base_t::extents_type;
      using typename base_t::layout_type;
      using typename base_t::accessor_type;
      using typename base_t::mapping_type;

   // constructors, assignment -------------------------------------------
      using base_t::base_t;

      constexpr basic_span() = default;
      constexpr basic_span( const basic_span&  ) = default;
      constexpr basic_span(       basic_span&& ) = default;

      constexpr basic_span& operator=( const basic_span&  ) = default;
      constexpr basic_span& operator=(       basic_span&& ) = default;

      ~basic_span() = default;

   // element accessors ---------------------------------------------
   // only allow element access with yam::index

   public :
   // expand out indices into basic_mdspan::operator()
      [[nodiscard]]
      constexpr element_type& operator()( const index_type i ) const
     {
         return [&]<size_t... Idxs>
         ( const base_t& self,
           std::index_sequence<Idxs...> ) -> element_type&
        {
            return self(i[Idxs]...);
        }( *this, std::make_index_sequence<ndim>());
     }
  };

// deduction guide to construct from mdspan
   template<typename    ElementType,
            ptrdiff_t...       Exts,
            typename   LayoutPolicy,
            typename AccessorPolicy>
   basic_span( stx::basic_mdspan<ElementType,
                                 stx::extents<Exts...>,
                                 LayoutPolicy,
                                 AccessorPolicy> )
      -> basic_span<ElementType,
                    stx::extents<Exts...>,
                    LayoutPolicy,
                    AccessorPolicy>;

// convenience typedefs

   template<typename ElementType,
            ptrdiff_t...    Exts>
   using span = basic_span<ElementType,
                           stx::extents<Exts...>>;

   template<typename ElementType,
            ptrdiff_t...    Exts>
   using primal_span = basic_span<ElementType,
                                  stx::extents<Exts...>,
                                  default_layout,
                                  default_accessor<ElementType>,
                                  primal>;

   template<typename ElementType,
            ptrdiff_t...    Exts>
   using dual_span = basic_span<ElementType,
                                stx::extents<Exts...>,
                                default_layout,
                                default_accessor<ElementType>,
                                dual>;

   template<typename ElementType,
            grid_t          grid= primal>
   using span1 = basic_span<ElementType,
                            stx::extents<stx::dynamic_extent>,
                            default_layout,
                            default_accessor<ElementType>,
                            grid>;

   template<typename ElementType,
            grid_t          grid= primal>
   using span2 = basic_span<ElementType,
                            stx::extents<stx::dynamic_extent,
                                         stx::dynamic_extent>,
                            default_layout,
                            default_accessor<ElementType>,
                            grid>;

   template<typename ElementType,
            grid_t          grid= primal>
   using span3 = basic_span<ElementType,
                            stx::extents<stx::dynamic_extent,
                                         stx::dynamic_extent,
                                         stx::dynamic_extent>,
                            default_layout,
                            default_accessor<ElementType>,
                            grid>;
}

