
# pragma once

# include "concepts.h"
# include "type_traits.h"
# include "index.h"

# include "external/mdspan.h"

namespace yam
{
/*
 * ===============================================================
 *
 * yam::basic_span
 *    thin wrapper around basic_mdspan, adding an operator()(yam::index) and relevant members eg ndim, grid
 *
 * ===============================================================
 */

// declaration
   template<typename    ElementType,
            typename        Extents,
            typename   LayoutPolicy = default_layout,
            typename AccessorPolicy = default_accessor<ElementType>,
            bool           is_cspan = false,
            grid_t             GRID = primal>
   class basic_span;

// only allowed specialisation
   template<typename    ElementType,
            ptrdiff_t...       Exts,
            typename   LayoutPolicy,
            typename AccessorPolicy,
            bool           is_cspan,
            grid_t             GRID>
   class basic_span<ElementType,
                    stx::extents<Exts...>,
                    LayoutPolicy,
                    AccessorPolicy,
                    is_cspan,
                    GRID>
  {
   private :

      using mdspan_t =
         stx::basic_mdspan<ElementType,
                           stx::extents<Exts...>,
                           LayoutPolicy,
                           AccessorPolicy>;

   public :

      static constexpr ndim_t ndim = sizeof...(Exts);
      static constexpr grid_t grid = GRID;

   // only index_type is different from stx::basic_mdspan to facilitate `indexable` concept
      using index_type = index<ndim,grid>;

      using element_type    = typename mdspan_t::element_type;
      using extents_type    = typename mdspan_t::extents_type;
      using layout_type     = typename mdspan_t::layout_type;
      using accessor_type   = typename mdspan_t::accessor_type;
      using mapping_type    = typename mdspan_t::mapping_type;
      using pointer         = typename mdspan_t::pointer;
      using reference       = typename mdspan_t::reference;
      using const_reference =
         const std::remove_reference_t<reference>&;

   // default operations
      constexpr basic_span() = default;
      constexpr basic_span( const basic_span&  ) = default;
      constexpr basic_span(       basic_span&& ) = default;

      constexpr basic_span& operator=( const basic_span&  ) = default;
      constexpr basic_span& operator=(       basic_span&& ) = default;

      ~basic_span() = default;

   // ctors from stx::mdspan

      constexpr explicit basic_span( const mdspan_t& m )
         : mdspan_member(m)
     { }

      constexpr explicit basic_span( mdspan_t&& m )
         : mdspan_member(std::move(m))
     { }

   // ctors forwarding arguments to mdspan member

   // pointer and pack of dynamic indexes
      template<typename... IndexTypes>
      constexpr basic_span( pointer p,
                            IndexTypes... dynamic_extents )
         requires (std::convertible_to<IndexTypes,
                                       typename mdspan_t::index_type>&&...)
               && (sizeof...(IndexTypes)==mdspan_t::rank_dynamic())
               && std::constructible_from<mdspan_t,
                                          decltype(p),
                                          decltype(dynamic_extents)...>
         : mdspan_member( std::move(p), dynamic_extents... )
     { }

   // pointer and array of dynamic indexes
      template<typename IndexType,
               size_t N>
      constexpr basic_span( pointer p,
                            const std::array<IndexType,N>& dynamic_extents )
         requires (std::convertible_to<IndexType,
                                       typename mdspan_t::index_type>)
               && std::constructible_from<mdspan_t,
                                          decltype(p),
                                          decltype(dynamic_extents)>
         : mdspan_member( std::move(p), dynamic_extents )
     { }

   // pointer and mapping
      constexpr basic_span( pointer p,
                            const mapping_type& m )
         requires std::constructible_from<mdspan_t,
                                          decltype(p),
                                          decltype(m)>
         : mdspan_member( std::move(p), m )
     { }

   // pointer, mapping and accessor
      constexpr basic_span( pointer p,
                            const mapping_type& m,
                            const accessor_type& a )
         requires std::constructible_from<mdspan_t,
                                          decltype(p),
                                          decltype(m),
                                          decltype(a)>
         : mdspan_member( std::move(p), m, a )
     { }

   // element accessor ---------------------------------------------

      [[nodiscard]]
      constexpr const_reference operator()( index_type i ) const
         requires (is_cspan)
     {
         return element_access( mdspan_member, i );
     }

      [[nodiscard]]
      constexpr reference operator()( index_type i ) const
         requires (!is_cspan)
     {
         return element_access( mdspan_member, i );
     }

   // stx::basic_mdspan-like interface

      [[nodiscard]]
      constexpr auto accessor() const { return mdspan_member.accessor(); }

      [[nodiscard]]
      constexpr auto mapping() const { return mdspan_member.mapping(); }

      [[nodiscard]]
      constexpr auto extents() const { return mdspan_member.extents(); }

      [[nodiscard]]
      constexpr auto data() const { return mdspan_member.data(); }

   // indexed values
      [[nodiscard]]
      constexpr auto extent(size_t r) const { return mdspan_member.extent(r); }

      [[nodiscard]]
      constexpr auto stride(size_t r) const { return mdspan_member.stride(r); }

   // sizes
      [[nodiscard]]
      constexpr auto size() const { return mdspan_member.size(); }

      [[nodiscard]]
      constexpr auto unique_size() const { return mdspan_member.unique_size(); }

   // static member functions
      [[nodiscard]]
      static constexpr auto rank(){ return mdspan_t::rank(); }

      [[nodiscard]]
      static constexpr auto rank_dynamic(){ return mdspan_t::rank_dynamic(); }

      [[nodiscard]]
      static constexpr auto static_extent(size_t r){ return mdspan_t::static_extent(r); }

      [[nodiscard]]
      static constexpr auto is_always_unique(){ return mdspan_t::is_always_unique(); }

      [[nodiscard]]
      static constexpr auto is_always_contiguous(){ return mdspan_t::is_always_contiguous(); }

      [[nodiscard]]
      static constexpr auto is_always_strided(){ return mdspan_t::is_always_strided(); }

   // non-static layout queries
      [[nodiscard]]
      constexpr auto is_unique() const { return mdspan_member.is_unique(); }

      [[nodiscard]]
      constexpr auto is_contiguous() const { return mdspan_member.is_contiguous(); }

   private :

      mdspan_t mdspan_member;
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

// span --------------------------------------

   template<typename ElementType,
            ptrdiff_t...    Exts>
   using span = basic_span<ElementType,
                           stx::extents<Exts...>>;

// specify grid type

   template<typename ElementType,
            ptrdiff_t...    Exts>
   using primal_span = basic_span<ElementType,
                                  stx::extents<Exts...>,
                                  default_layout,
                                  default_accessor<ElementType>,
                                  false,
                                  primal>;

   template<typename ElementType,
            ptrdiff_t...    Exts>
   using dual_span = basic_span<ElementType,
                                stx::extents<Exts...>,
                                default_layout,
                                default_accessor<ElementType>,
                                false,
                                dual>;

// specify dimension

   template<typename ElementType,
            grid_t          grid= primal>
   using span1 = basic_span<ElementType,
                            stx::extents<stx::dynamic_extent>,
                            default_layout,
                            default_accessor<ElementType>,
                            false,
                            grid>;

   template<typename ElementType,
            grid_t          grid= primal>
   using span2 = basic_span<ElementType,
                            stx::extents<stx::dynamic_extent,
                                         stx::dynamic_extent>,
                            default_layout,
                            default_accessor<ElementType>,
                            false,
                            grid>;

   template<typename ElementType,
            grid_t          grid= primal>
   using span3 = basic_span<ElementType,
                            stx::extents<stx::dynamic_extent,
                                         stx::dynamic_extent,
                                         stx::dynamic_extent>,
                            default_layout,
                            default_accessor<ElementType>,
                            false,
                            grid>;

// specify dimension and grid type

   // primal
   template<typename ElementType,
            grid_t          grid= primal>
   using primal_span1 = basic_span<ElementType,
                                   stx::extents<stx::dynamic_extent>,
                                   default_layout,
                                   default_accessor<ElementType>,
                                   false,
                                   primal>;

   template<typename ElementType,
            grid_t          grid= primal>
   using primal_span2 = basic_span<ElementType,
                                   stx::extents<stx::dynamic_extent,
                                                stx::dynamic_extent>,
                                   default_layout,
                                   default_accessor<ElementType>,
                                   false,
                                   primal>;

   template<typename ElementType,
            grid_t          grid= primal>
   using primal_span3 = basic_span<ElementType,
                                   stx::extents<stx::dynamic_extent,
                                                stx::dynamic_extent,
                                                stx::dynamic_extent>,
                                   default_layout,
                                   default_accessor<ElementType>,
                                   false,
                                   primal>;

   // dual
   template<typename ElementType,
            grid_t          grid= primal>
   using dual_span1 = basic_span<ElementType,
                                 stx::extents<stx::dynamic_extent>,
                                 default_layout,
                                 default_accessor<ElementType>,
                                 false,
                                 dual>;

   template<typename ElementType,
            grid_t          grid= primal>
   using dual_span2 = basic_span<ElementType,
                                 stx::extents<stx::dynamic_extent,
                                              stx::dynamic_extent>,
                                 default_layout,
                                 default_accessor<ElementType>,
                                 false,
                                 dual>;

   template<typename ElementType,
            grid_t          grid= primal>
   using dual_span3 = basic_span<ElementType,
                                 stx::extents<stx::dynamic_extent,
                                              stx::dynamic_extent,
                                              stx::dynamic_extent>,
                                 default_layout,
                                 default_accessor<ElementType>,
                                 false,
                                 dual>;

// cspan --------------------------------------

   template<typename ElementType,
            ptrdiff_t...    Exts>
   using cspan = basic_span<ElementType,
                            stx::extents<Exts...>,
                            default_layout,
                            default_accessor<ElementType>,
                            true>;

// specify grid type

   template<typename ElementType,
            ptrdiff_t...    Exts>
   using primal_cspan = basic_span<ElementType,
                                   stx::extents<Exts...>,
                                   default_layout,
                                   default_accessor<ElementType>,
                                   true,
                                   primal>;

   template<typename ElementType,
            ptrdiff_t...    Exts>
   using dual_cspan = basic_span<ElementType,
                                 stx::extents<Exts...>,
                                 default_layout,
                                 default_accessor<ElementType>,
                                 true,
                                 dual>;

// specify dimension

   template<typename ElementType,
            grid_t          grid= primal>
   using cspan1 = basic_span<ElementType,
                             stx::extents<stx::dynamic_extent>,
                             default_layout,
                             default_accessor<ElementType>,
                             true,
                             grid>;

   template<typename ElementType,
            grid_t          grid= primal>
   using cspan2 = basic_span<ElementType,
                             stx::extents<stx::dynamic_extent,
                                          stx::dynamic_extent>,
                             default_layout,
                             default_accessor<ElementType>,
                             true,
                             grid>;

   template<typename ElementType,
            grid_t          grid= primal>
   using cspan3 = basic_span<ElementType,
                             stx::extents<stx::dynamic_extent,
                                          stx::dynamic_extent,
                                          stx::dynamic_extent>,
                             default_layout,
                             default_accessor<ElementType>,
                             true,
                             grid>;

// specify dimension and grid type

   // primal
   template<typename ElementType,
            grid_t          grid= primal>
   using primal_cspan1 = basic_span<ElementType,
                                    stx::extents<stx::dynamic_extent>,
                                    default_layout,
                                    default_accessor<ElementType>,
                                    true,
                                    primal>;

   template<typename ElementType,
            grid_t          grid= primal>
   using primal_cspan2 = basic_span<ElementType,
                                    stx::extents<stx::dynamic_extent,
                                                 stx::dynamic_extent>,
                                    default_layout,
                                    default_accessor<ElementType>,
                                    true,
                                    primal>;

   template<typename ElementType,
            grid_t          grid= primal>
   using primal_cspan3 = basic_span<ElementType,
                                    stx::extents<stx::dynamic_extent,
                                                 stx::dynamic_extent,
                                                 stx::dynamic_extent>,
                                    default_layout,
                                    default_accessor<ElementType>,
                                    true,
                                    primal>;

   // dual
   template<typename ElementType,
            grid_t          grid= primal>
   using dual_cspan1 = basic_span<ElementType,
                                  stx::extents<stx::dynamic_extent>,
                                  default_layout,
                                  default_accessor<ElementType>,
                                  true,
                                  dual>;

   template<typename ElementType,
            grid_t          grid= primal>
   using dual_cspan2 = basic_span<ElementType,
                                  stx::extents<stx::dynamic_extent,
                                               stx::dynamic_extent>,
                                  default_layout,
                                  default_accessor<ElementType>,
                                  true,
                                  dual>;

   template<typename ElementType,
            grid_t          grid= primal>
   using dual_cspan3 = basic_span<ElementType,
                                  stx::extents<stx::dynamic_extent,
                                               stx::dynamic_extent,
                                               stx::dynamic_extent>,
                                  default_layout,
                                  default_accessor<ElementType>,
                                  true,
                                  dual>;
}
