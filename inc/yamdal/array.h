
# pragma once

# include "concepts.h"
# include "index.h"

# include "external/mdspan.h"

# include <memory>

namespace yam
{
/*
 * ===============================================================
 *
 * yam::basic_array
 *    Memory-managing class representing a multi-dimensional array.
 *    Allocates and deallocates required memory at construction and destruction.
 *    Array elements are mapped to memory exactly as for stx::basic_mdspan
 *    
 * ===============================================================
 */
   
// declaration
   template<typename    ElementType,
            typename        Extents,
            typename   LayoutPolicy = default_layout,
            typename AccessorPolicy = default_accessor<ElementType>,
            grid_t             GRID = primal>
   class [[nodiscard("yam::basic_array allocates memory so should not be immediately discarded")]]
   /*class*/ basic_array;

// only allowed specialisation
   template<typename    ElementType,
            ptrdiff_t...       Exts,
            typename   LayoutPolicy,
            typename AccessorPolicy,
            grid_t             GRID>
   class basic_array<ElementType,
                     stx::extents<Exts...>,
                     LayoutPolicy,
                     AccessorPolicy,
                     GRID>
  {
   private :

      using mdspan_t =
         stx::basic_mdspan<ElementType,
                           stx::extents<Exts...>,
                           LayoutPolicy,
                           AccessorPolicy>;

   public :

      void check()
     {
         assert( storage.get() );
         assert( mdspan_member.data() );
         assert( storage.get() == mdspan_member.data() );
     }

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

      constexpr basic_array( basic_array&& ) = default;
      ~basic_array() = default;

   // all members have move ctors, so default is viable
      constexpr basic_array& operator=( basic_array&& ) = default;

   // user defined constructors -------------------------------------------

   // deep copy ctor from another basic_array
      constexpr basic_array( const basic_array& other  ) :
         storage( new_storage(
            (size_t)other.mapping().required_span_size() ) ),
         mdspan_member( storage.get(),
                        dynamic_extents(other.extents()) )
     {
         check();
         const auto size = mapping().required_span_size();
         std::copy( other.storage.get(),
                    other.storage.get()+size, 
                    storage.get() );
     }

   // ctors with dynamic extents

      // dynamic extent parameter pack
      template<typename... IndexTypes>
         requires (sizeof...(IndexTypes) == mdspan_t::rank_dynamic())
               && (std::convertible_to<IndexTypes,
                                       typename mdspan_t::index_type>&&...)
      constexpr explicit basic_array( IndexTypes... dynamic_extents ) :
         storage( new_storage(
            extents_type{dynamic_extents...} ) ),
         mdspan_member( storage.get(), dynamic_extents... ) 
     {
         check();
     }

      // dynamic extent array
      template<typename IndexType,
               size_t N>
         requires (N==mdspan_t::rank_dynamic())
               && (std::convertible_to<IndexType,typename mdspan_t::index_type>)
      constexpr explicit basic_array( const std::array<IndexType,N>& dynamic_extents ) :
         storage( new_storage(
            extents_type{dynamic_extents} ) ),
         mdspan_member( storage.get(), dynamic_extents ) 
     { }

   // assignment -------------------------------------------

   // deep copy assignment from another basic_array
      constexpr basic_array& operator=( const basic_array& other )
     {
      // new allocation size
         const auto size = other.mapping().required_span_size();

      // resize and reallocate if necessary
         if( !(extents() == other.extents()) )
        {
            storage = std::make_unique<element_type[]>( size );
            mdspan_member =
               mdspan_t( storage.get(),
                         dynamic_extents(other.extents()) );
        }

         check();
      // deep copy of storage
         std::copy( other.storage.get(),
                    other.storage.get()+size, 
                    storage.get() );
     }

   // element accessors ---------------------------------------------

   // only allow element access with yam::index
      [[nodiscard]]
      constexpr const_reference operator()( index_type i ) const
     {
         check();
         return element_access( mdspan_member, i );
     }

      [[nodiscard]]
      constexpr reference operator()( index_type i )
     {
         check();
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

      [[nodiscard]]
      constexpr auto new_storage( const extents_type& extents )
     {
         return new_storage(size_t(
            yam::required_span_size<layout_type>(extents)));
     }

      [[nodiscard]]
      constexpr auto new_storage( size_t size )
     {
         return std::make_unique<element_type[]>(size);
     }

      std::unique_ptr<element_type[]> storage;
      mdspan_t mdspan_member;
  };


// convenience typedefs

   template<typename ElementType,
            ptrdiff_t...    Exts>
   using array = basic_array<ElementType,
                             stx::extents<Exts...>>;

// specify grid type
   template<typename ElementType,
            ptrdiff_t...    Exts>
   using primal_array = basic_array<ElementType,
                                    stx::extents<Exts...>,
                                    default_layout,
                                    default_accessor<ElementType>,
                                    primal>;

   template<typename ElementType,
            ptrdiff_t...    Exts>
   using dual_array = basic_array<ElementType,
                                  stx::extents<Exts...>,
                                  default_layout,
                                  default_accessor<ElementType>,
                                  dual>;

// specify dimension

   template<typename ElementType,
            grid_t          grid= primal>
   using array1 = basic_array<ElementType,
                              stx::extents<stx::dynamic_extent>,
                              default_layout,
                              default_accessor<ElementType>,
                              grid>;

   template<typename ElementType,
            grid_t          grid= primal>
   using array2 = basic_array<ElementType,
                              stx::extents<stx::dynamic_extent,
                                           stx::dynamic_extent>,
                              default_layout,
                              default_accessor<ElementType>,
                              grid>;

   template<typename ElementType,
            grid_t          grid= primal>
   using array3 = basic_array<ElementType,
                              stx::extents<stx::dynamic_extent,
                                           stx::dynamic_extent,
                                           stx::dynamic_extent>,
                              default_layout,
                              default_accessor<ElementType>,
                              grid>;

// specify dimension and grid type

   // primal
   template<typename ElementType,
            grid_t          grid= primal>
   using primal_array1 = basic_array<ElementType,
                                     stx::extents<stx::dynamic_extent>,
                                     default_layout,
                                     default_accessor<ElementType>,
                                     primal>;

   template<typename ElementType,
            grid_t          grid= primal>
   using primal_array2 = basic_array<ElementType,
                                     stx::extents<stx::dynamic_extent,
                                                  stx::dynamic_extent>,
                                     default_layout,
                                     default_accessor<ElementType>,
                                     primal>;

   template<typename ElementType,
            grid_t          grid= primal>
   using primal_array3 = basic_array<ElementType,
                                     stx::extents<stx::dynamic_extent,
                                                  stx::dynamic_extent,
                                                  stx::dynamic_extent>,
                                     default_layout,
                                     default_accessor<ElementType>,
                                     primal>;

   // dual
   template<typename ElementType,
            grid_t          grid= primal>
   using dual_array1 = basic_array<ElementType,
                                   stx::extents<stx::dynamic_extent>,
                                   default_layout,
                                   default_accessor<ElementType>,
                                   dual>;

   template<typename ElementType,
            grid_t          grid= primal>
   using dual_array2 = basic_array<ElementType,
                                   stx::extents<stx::dynamic_extent,
                                                stx::dynamic_extent>,
                                   default_layout,
                                   default_accessor<ElementType>,
                                   dual>;

   template<typename ElementType,
            grid_t          grid= primal>
   using dual_array3 = basic_array<ElementType,
                                   stx::extents<stx::dynamic_extent,
                                                stx::dynamic_extent,
                                                stx::dynamic_extent>,
                                   default_layout,
                                   default_accessor<ElementType>,
                                   dual>;
}
