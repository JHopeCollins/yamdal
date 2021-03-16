
# include <yamdal/views.h>
# include <yamdal/algorithm.h>
# include <yamdal/execution.h>
# include <yamdal/concepts.h>
# include <yamdal/type_traits.h>
# include <yamdal/index.h>

# include <iostream>
# include <array>

   template<typename T,
            size_t  N0>
   using matrix1_t = std::array<T,N0>;

   template<typename T,
            size_t  N0,
            size_t  N1>
   using matrix2_t = std::array<std::array<T,N1>,N0>;

   constexpr auto print_2d_array =
      []<yam::indexable2 Arr>
      ( const Arr& arr,
        const yam::index_type_of_t<Arr> bidx,
        const yam::index_type_of_t<Arr> eidx ) -> void
     {
         for( size_t i=bidx[0]; i<eidx[0]; ++i )
        {
            for( size_t j=bidx[1]; j<eidx[1]; ++j )
           {
               std::cout << arr({i,j}) << " ";
           }
            std::cout << "\n";
        }
     };

   int main()
  {
      constexpr size_t ni=2;
      constexpr size_t nj=4;

   // underlying memory
      matrix2_t<int,ni,nj> m2{};

   // indexable view into memory
      yam::indexable2_r<int&> auto arr2 =
         [&m2]( const yam::index2<>& ij ) -> int&
        {
            const auto i = size_t(ij[0]);
            const auto j = size_t(ij[1]);
            return m2[i][j];
        };

   // generator returning 'flattened' index
      yam::indexable2_r<int> auto flat2 =
         [=]( const yam::index2<>& ij )
        {
            const auto i = size_t(ij[0]);
            const auto j = size_t(ij[1]);
            return i*nj + j;
        };

      static_assert( std::copy_constructible<decltype(arr2)> );
      static_assert( std::move_constructible<decltype(arr2)> );

      auto arr2_cview = yam::cwindow(arr2);
      auto arr2_view = yam::window(arr2);

      auto temp_view =
         yam::window([=]( const yam::index2<> ij )
                     { return flat2(ij); });

      return 0;
  }
