
# include <yamdal/utility.h>

# include <catch.hpp>

   TEST_CASE( "utility get", "[utility]" )
  {
      namespace util = yam::util;

      constexpr ptrdiff_t c0=-10;
      constexpr ptrdiff_t c1=1538;
      constexpr ptrdiff_t c2=-289;
      constexpr ptrdiff_t c3=8;

      REQUIRE( util::get<0>( c0,c1,c2,c3 ) == c0 );
      REQUIRE( util::get<1>( c0,c1,c2,c3 ) == c1 );
      REQUIRE( util::get<2>( c0,c1,c2,c3 ) == c2 );
      REQUIRE( util::get<3>( c0,c1,c2,c3 ) == c3 );
  }

   TEST_CASE( "get_dynamic_extent_indices", "[mdspan][utility]" )
  {
      namespace util = yam::util;

      constexpr auto dyn = stx::dynamic_extent;

      constexpr auto ex2_53 = stx::extents<5,3>();
      const auto ex2_d3 = stx::extents<dyn,3>();
      const auto ex4_3d7d = stx::extents<3,dyn,7,dyn>();

      using result0_t = decltype(util::get_dynamic_extent_indices(ex2_53));
      using result1_t = decltype(util::get_dynamic_extent_indices(ex2_d3));
      using result2_t = decltype(util::get_dynamic_extent_indices(ex4_3d7d));

      using expected0_t = std::index_sequence<>;
      using expected1_t = std::index_sequence<0>;
      using expected2_t = std::index_sequence<1,3>;

      REQUIRE( (std::same_as<result0_t,expected0_t>) );
      REQUIRE( (std::same_as<result1_t,expected1_t>) );
      REQUIRE( (std::same_as<result2_t,expected2_t>) );
  }

   TEST_CASE( "replace_nth_extent", "[mdspan][utility]" )
  {
      namespace util = yam::util;

      constexpr auto dyn = stx::dynamic_extent;

      constexpr auto ex2_53 = stx::extents<5,3>();
      const auto ex2_d3 = stx::extents<dyn,3>(5);
      const auto ex4_3d7d = stx::extents<3,dyn,7,dyn>(9,4);

      using result0_t = decltype(util::replace_nth_extent<1,6>(ex2_53));
      using result1_t = decltype(util::replace_nth_extent<0,1>(ex2_d3));
      using result2_t = decltype(util::replace_nth_extent<3,1>(ex4_3d7d));

      using expected0_t = stx::extents<5,6>;
      using expected1_t = stx::extents<1,3>;
      using expected2_t = stx::extents<3,dyn,7,1>;

   // check static extents are correct
      REQUIRE( (std::same_as<result0_t,expected0_t>) );
      REQUIRE( (std::same_as<result1_t,expected1_t>) );
      REQUIRE( (std::same_as<result2_t,expected2_t>) );

   // check dynamic extents are correct
      REQUIRE( ex4_3d7d.extent(1) == (util::replace_nth_extent<3,1>(ex4_3d7d)).extent(1) );
  }

