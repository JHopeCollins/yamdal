
# include <yamdal/span.h>
# include <yamdal/external/mdspan.h>

# include <catch.hpp>

# include <memory>

   TEST_CASE( "yam::span has same meta-data as stx::mdspan", "[span]" )
  {
      using layout = yam::default_layout;

      const auto size1 =
         yam::required_span_size<layout,10>();

      auto data1 = std::make_unique<int[]>(size1);

   // compare yam::span and stx::mdspan - should be equal
      yam::span<int,10>   ysp1(data1.get());
      stx::mdspan<int,10> mds1(data1.get());

      static_assert(
         std::same_as<
            decltype(ysp1)::layout_type,
            decltype(mds1)::layout_type> );

      REQUIRE( ysp1.extent(0) == mds1.extent(0) );
      REQUIRE( ysp1.size()    == mds1.size()    );

      REQUIRE(   ysp1.mapping().required_span_size()
              == mds1.mapping().required_span_size() );

      REQUIRE( ysp1.extents() == mds1.extents() );
  }

   TEST_CASE( "1D yam::span has same accessor behaviour as stx::mdspan", "[span][1D]" )
  {
      using layout = yam::default_layout;
      using integer = yam::idx_t;

      const size_t size1 =
         yam::required_span_size<layout,10>();

      auto data1 = std::make_unique<integer[]>(size1);

      yam::span<integer,  10> ysp1(data1.get());
      stx::mdspan<integer,10> mds1(data1.get());

      static_assert(
         std::same_as<
            decltype(ysp1)::layout_type,
            decltype(mds1)::layout_type> );

   // accessors to set data
      using idx = decltype(ysp1)::index_type;

      const auto ni = ysp1.extent(0);

      for( integer i=0; i<ni; ++i )
     {
         ysp1( idx{i} ) = i;

         REQUIRE( mds1(     i  ) == i );
         REQUIRE( ysp1( idx{i} ) == i );
     }
  }

   TEST_CASE( "2D yam::span has same accessor behaviour as stx::mdspan", "[span][2D]" )
  {
      using layout = yam::default_layout;
      using integer = yam::idx_t;

      constexpr auto dyn = stx::dynamic_extent;

      const size_t size2 =
         yam::required_span_size<layout,4,dyn>(3);

      auto data2 = std::make_unique<integer[]>(size2);

      yam::span<integer,  4,dyn> ysp2(data2.get(),3);
      stx::mdspan<integer,4,dyn> mds2(data2.get(),3);

      static_assert(
         std::same_as<
            decltype(ysp2)::layout_type,
            decltype(mds2)::layout_type> );

   // accessors to set data
      using idx = decltype(ysp2)::index_type;

      const auto ni = ysp2.extent(0);
      const auto nj = ysp2.extent(1);

      const auto flatten =
         [=]( integer i, integer j )
        {
            return i*nj + j;
        };

      for( integer i=0; i<ni; ++i )
     {
         for( integer j=0; j<nj; ++j )
        {
            const auto x = flatten(i,j);

            ysp2( idx{i,j} ) = x;
   
            REQUIRE( mds2(     i,j  ) == x );
            REQUIRE( ysp2( idx{i,j} ) == x );
        }
     }
  }

   TEST_CASE( "3D yam::span has same accessor behaviour as stx::mdspan", "[span][3D]" )
  {
      using layout = yam::default_layout;
      using integer = yam::idx_t;

      constexpr auto dyn = stx::dynamic_extent;

      const size_t size3 =
         yam::required_span_size<layout,dyn,dyn,dyn>(3,4,5);

      auto data3 = std::make_unique<integer[]>(size3);

      yam::span<integer,  dyn,dyn,dyn> ysp3(data3.get(),3,4,5);
      stx::mdspan<integer,dyn,dyn,dyn> mds3(data3.get(),3,4,5);

      static_assert(
         std::same_as<
            decltype(ysp3)::layout_type,
            decltype(mds3)::layout_type> );

   // accessors to set data
      using idx = decltype(ysp3)::index_type;

      const auto ni = ysp3.extent(0);
      const auto nj = ysp3.extent(1);
      const auto nk = ysp3.extent(2);

      const auto flatten =
         [=]( integer i, integer j, integer k )
        {
            return i*nj*nk + j*nk + k;
        };

      for( integer i=0; i<ni; ++i )
     {
         for( integer j=0; j<nj; ++j )
        {
            for( integer k=0; k<nk; ++k )
           {
               const auto x = flatten(i,j,k);

               ysp3( idx{i,j,k} ) = x;
   
               REQUIRE( mds3(     i,j,k  ) == x );
               REQUIRE( ysp3( idx{i,j,k} ) == x );
           }
        }
     }
  }

