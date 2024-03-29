
# include <iostream>

# include <yamdal/all.h>

# include <array>

   template<typename T,
            size_t  N0>
   using matrix1_t = std::array<T,N0>;

   template<typename T,
            size_t  N0,
            size_t  N1>
   using matrix2_t = std::array<std::array<T,N1>,N0>;


   int main()
  {
      static_assert( yam::execution_policy<yam::execution::serial_policy> );
      static_assert( yam::execution_policy<yam::execution::openmp_policy> );

      static_assert( yam::execution_policy<decltype(yam::execution::seq)> );
      static_assert( yam::execution_policy<decltype(yam::execution::openmp)> );

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
         [=]( const yam::index2<>& ij ) -> int
        {
            const auto i = int(ij[0]);
            const auto j = int(ij[1]);
            return i*int(nj) + j;
        };

   // deduction guide or using default alias parameters
      constexpr yam::index begin_index{0,0};
      constexpr stx::extents<ni,nj> exts;

      const auto print_2d_array =
         []<yam::indexable2 Arr,
            ptrdiff_t...   Exts>
         ( const Arr& arr,
           const yam::index_type_of_t<Arr> bi,
           const stx::extents<Exts...>     ex ) -> void
        {
            for( yam::idx_t i=bi[0]; i<bi[0]+ex.extent(0); ++i )
           {
               for( yam::idx_t j=bi[1]; j<bi[1]+ex.extent(1); ++j )
              {
                  std::cout << arr({i,j}) << " ";
              }
               std::cout << "\n";
           }
        };

      std::cout << "before:\n";
      print_2d_array( arr2, begin_index, exts );

      yam::assign( begin_index, exts,
                   arr2, flat2 );

      std::cout << "after:\n";
      print_2d_array( arr2, begin_index, exts );

   // new matrix view
      matrix2_t<int,ni,nj> m3{};
      const yam::indexable2_r<int&> auto arr3 =
         [&m3]( const yam::index2<>& ij ) -> int&
        {
            const auto i = size_t(ij[0]);
            const auto j = size_t(ij[1]);
            return m3[i][j];
        };

      using idx_t = const yam::index<2>&;

      static_assert( yam::has_ndim_member_v<idx_t> );
      static_assert( yam::has_grid_member_v<idx_t> );

      static_assert( yam::has_ndim_v<idx_t> );
      static_assert( yam::has_grid_v<idx_t> );

      static_assert( yam::has_index_type_member_v<idx_t> );

      yam::transform( yam::execution::openmp,
                      begin_index, exts,
                      arr3,
                      []( int i ){ return i+1; },
                      arr2 );

      std::cout << "eager plus 1:\n";
      print_2d_array( arr3, begin_index, exts );

      const auto sum =
         yam::reduce( yam::execution::seq,
                      begin_index, exts,
                      std::plus<int>{},
                      0, /* identity_v */
                      0, /* init */
                      arr3 );

      std::cout << "sum of plus1: " << sum << "\n";

      yam::fill( begin_index, exts,
                 arr2, 1 );
      std::cout << "all 1\n";
      print_2d_array( arr2, begin_index, exts );

      yam::generate( begin_index, exts,
                 arr2, []{return 42;} );
      std::cout << "all 42\n";
      print_2d_array( arr2, begin_index, exts );

      return 0;
  }
