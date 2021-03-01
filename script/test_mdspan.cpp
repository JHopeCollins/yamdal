
# include <yamdal/external/mdspan.h>
# include <yamdal/span.h>

# include <array>
# include <iostream>

   template<typename   LayoutPolicy,
            ptrdiff_t...       Exts>
      requires ((Exts!=stx::dynamic_extent)&&...)
   [[nodiscard]]
   constexpr auto required_span_size(
      stx::extents<Exts...> exts )
  {
      using extents_t = stx::extents<Exts...>;
      using mapping_t = typename LayoutPolicy::template mapping<extents_t>;
      return mapping_t(exts).required_span_size();
  }

   template<typename   LayoutPolicy,
            ptrdiff_t...       Exts>
   [[nodiscard]]
   auto required_span_size(
      stx::extents<Exts...> exts )
  {
      using extents_t = stx::extents<Exts...>;
      using mapping_t = typename LayoutPolicy::template mapping<extents_t>;
      return mapping_t(exts).required_span_size();
  }

   template<typename ElementType,
            ptrdiff_t...    Exts>
   bool is_mdspan( const stx::mdspan<ElementType,Exts...>& )
  {
      return true;
  }

   int main()
  {
      std::cout << std::boolalpha;

      constexpr ptrdiff_t ni = 3;
      const ptrdiff_t nj = 4;
      using mds_3dyn_t = stx::mdspan<int,ni,stx::dynamic_extent>;

      int* data0=new int[100];
      int* data1=new int[100];

      using layout = stx::layout_right;

      const auto size0 = [](auto dim2)
     {
         using mapping_t = mds_3dyn_t::mapping_type;
         using extents_t = mds_3dyn_t::extents_type;
         
         return mapping_t(extents_t(dim2)).required_span_size();
     }(nj);

      const auto size9 = required_span_size<layout>( mds_3dyn_t::extents_type(nj) );

      std::cout << size9 << "\n";

      std::cout << "size0: " << size0 << "\n";

      const auto mds = mds_3dyn_t(data0,nj);
      const auto size = mds.mapping().required_span_size();
      std::cout << "required_span_size: " << size << "\n";

      std::cout << "mds is mds: " << is_mdspan(mds) << "\n";

      const auto ys = yam::span<int,3,3>(data1);

      std::cout << "ys is mds: " << is_mdspan(ys) << "\n";
      std::cout << ys.extents().extent(0) << "\n";

      [[maybe_unused]] auto ex = ys.extents();
      [[maybe_unused]] auto map = ys.mapping();
      [[maybe_unused]] const auto s = ys.size();
      [[maybe_unused]] const auto t = map.required_span_size();

      std::cout << "stx::mdspan:\n";
      mds(0,0)=15;
      mds(std::array{1,2})=12;
      std::cout << mds(0,0) << "\n";
      std::cout << mds(1,2) << "\n";

      std::cout << "yam::span:\n";
      ys({0,0})=15;
      ys({1,2})=12;
      std::cout << ys({0,0}) << "\n";
      std::cout << ys({1,2}) << "\n";

      delete[] data0;
      delete[] data1;

      return 0;
  }
