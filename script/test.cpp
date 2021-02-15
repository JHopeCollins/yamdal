
# include <iostream>

# include <algorithm.h>
# include <execution.h>
# include <concepts.h>
# include <type_traits.h>
# include <layouts.h>
# include <yamdal.h>

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
      constexpr yam::primal_index2 ip2{7,15};
      constexpr yam::dual_index<2> id2{3,4};

      constexpr yam::primal_index_range2 prng1(ip2);

      constexpr yam::primal_index_range2 prng2(yam::to_primal(prng1));
      constexpr yam::dual_index_range2 drng2(yam::to_dual(prng2));

      constexpr size_t ni=2;
      constexpr size_t nj=4;

      constexpr yam::index_range<2,yam::dual> irng1(id2);

      static_assert( irng1.begin_index[0] == 0 );
      static_assert( irng1.begin_index[1] == 0 );

      static_assert( irng1.end_index[0] == id2[0] );
      static_assert( irng1.end_index[1] == id2[1] );

   // underlying memory
      matrix2_t<int,ni,nj> m2{};

   // indexable view into memory
      yam::indexable2_r<int&> auto arr2 =
         [&m2]( const yam::index2<>& ij ) -> int&
        {
            const auto i = ij[0];
            const auto j = ij[1];
            return m2[i][j];
        };

   // generator returning 'flattened' index
      yam::indexable2_r<int> auto flat2 =
         [=]( const yam::index2<>& ij ) -> int
        {
            const auto i = ij[0];
            const auto j = ij[1];
            return i*nj + j;
        };

   // deduction guide or using default alias parameters
      constexpr yam::index    begin_index{0,0};
      constexpr yam::index2<> end_index{ni,nj};

      const auto print_2d_array =
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

//    std::cout << "before:\n";
//    print_2d_array( arr2, begin_index, end_index );

      yam::assign( begin_index, end_index,
                   arr2, flat2 );

//    std::cout << "after:\n";
//    print_2d_array( arr2, begin_index, end_index );

   // new matrix view
      matrix2_t<int,ni,nj> m3{};
      const yam::indexable2_r<int&> auto arr3 =
         [&m3]( const yam::index2<>& ij ) -> int&
        {
            const auto i = ij[0];
            const auto j = ij[1];
            return m3[i][j];
        };

      using idx_t = const yam::index<2>&;

      static_assert( yam::has_ndim_member_v<idx_t> );
      static_assert( yam::has_grid_member_v<idx_t> );

      static_assert( yam::has_ndim_v<idx_t> );
      static_assert( yam::has_grid_v<idx_t> );

      static_assert( yam::has_index_type_member_v<idx_t> );

      yam::transform( yam::execution::seq,
                      begin_index, end_index,
                      arr3,
                      []( int i ){ return i+1; },
                      arr2 );

      std::cout << "eager plus 1:\n";
      print_2d_array( arr3, begin_index, end_index );

      const auto sum =
         yam::reduce( yam::execution::seq,
                      begin_index, end_index,
                      std::plus<int>{},
                      0,
                      arr3 );

      std::cout << "sum of plus1: " << sum << "\n";

      return 0;
  }
