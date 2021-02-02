
# include <algorithm.h>
# include <execution.h>
# include <concepts.h>
# include <type_traits.h>
# include <layouts.h>
# include <yamdal.h>

# include <iostream>
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

      std::cout << "before:\n";
      for( size_t i=0; i<ni; ++i )
     {
         for( size_t j=0; j<nj; ++j )
        {
            std::cout << arr2({i,j}) << " ";
        }
         std::cout << "\n";
     }

      yam::assign( begin_index, end_index,
                   arr2, flat2 );

      std::cout << "after:\n";
      for( size_t i=0; i<ni; ++i )
     {
         for( size_t j=0; j<nj; ++j )
        {
            std::cout << arr2({i,j}) << " ";
        }
         std::cout << "\n";
     }

      return 0;
  }
