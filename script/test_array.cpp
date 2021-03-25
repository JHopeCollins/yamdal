
# include <yamdal/array.h>

# include <yamdal/external/mdspan.h>

# include <concepts>
# include <memory>
# include <iostream>

constexpr auto dyn = stx::dynamic_extent;

/*
namespace yam
{
   template<typename    ElementType,
            typename        Extents,
            typename   LayoutPolicy = default_layout,
            typename AccessorPolicy = default_accessor<ElementType>,
            grid_t             GRID = primal>
   class barry;

   template<typename    ElementType,
            ptrdiff_t...       Exts,
            typename   LayoutPolicy,
            typename AccessorPolicy,
            grid_t             GRID>
   class barry<ElementType,
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

      static constexpr ndim_t ndim = sizeof...(Exts);
      static constexpr grid_t grid = GRID;

      using index_type = index<ndim,grid>;

      using element_type    = typename mdspan_t::element_type;
      using extents_type    = typename mdspan_t::extents_type;
      using layout_type     = typename mdspan_t::layout_type;
      using accessor_type   = typename mdspan_t::accessor_type;
      using mapping_type    = typename mdspan_t::mapping_type;
      using reference       = typename mdspan_t::reference;
      using const_reference =
         const std::remove_reference_t<reference>&;

   // constructors

      template<typename... IndexTypes>
         requires (sizeof...(IndexTypes)==extents_type::rank_dynamic())
               && (std::convertible_to<IndexTypes,
                                       typename mdspan_t::index_type>&&...)
      [[nodiscard]]
      constexpr barry( IndexTypes... dynamic_extents ) :
         storage( new_storage( dynamic_extents... ) ),
         mds( storage.get(), dynamic_extents... )
     {
         check();
     }

      [[nodiscard]]
      constexpr reference operator()( index_type i )
     {
         check();
         return element_access( mds, i );
     }

      [[nodiscard]]
      constexpr const_reference operator()( index_type i ) const
     {
         check();
         return element_access( mds, i );
     }

      constexpr void check()
     {
         assert( storage.get() );
         assert( mds.data() );
         assert( storage.get() == mds.data() );
     }

      ~barry() = default;

   private :

      template<typename... IndexTypes>
         requires (sizeof...(IndexTypes) == mdspan_t::rank_dynamic())
               && (std::convertible_to<IndexTypes,typename mdspan_t::index_type>&&...)
      [[nodiscard]]
      auto new_storage( IndexTypes... dynamic_extents )
     {
         return std::make_unique<element_type[]>((size_t)
            yam::required_span_size<layout_type>(
               extents_type{dynamic_extents...}));
     }

      std::unique_ptr<element_type[]> storage;
      mdspan_t mds;
  };

   template<typename ElementType,
            ptrdiff_t...    Exts>
   using arry = barry<ElementType,
                             stx::extents<Exts...>>;
}
*/

   int main()
  {
      using integer = int;
      constexpr ptrdiff_t size0 = 4;
      constexpr ptrdiff_t size1 = 5;

      yam::array<integer,size0> arr1{};
      yam::array<integer,dyn>   arr2(size0);

      arr1.check();
      arr2.check();

      const yam::index1<> idx0{0};
      const yam::index1<> idx1{1};

      arr1(idx0) = size0;
      arr1(idx1) = arr1(idx0);

      arr2(idx0) = size0;
      arr2(idx1) = arr2(idx0);

      arr1.check();
      arr2.check();

      for( integer i=0; i<size0; ++i )
     {
         arr1({i})=i;
         arr2({i})=i;
     }

      const yam::array<integer,size0> arra{arr1};
      const yam::array<integer,dyn>   arrb{arr2};

      yam::array<integer,size0,size1> arr3{};
      yam::array<integer,dyn,size1>   arr4(size0);

      arr3.check();
      arr4.check();

      const yam::index2<> idx2{0,2};
      const yam::index2<> idx3{1,2};

      arr3(idx2) = size1;
      arr3(idx3) = arr3(idx2);

      arr4(idx2) = size1;
      arr4(idx3) = arr4(idx2);

      arr3.check();
      arr4.check();

      return 0;
  }
