
# include <yamdal/utility.h>

# include <catch.hpp>

   TEST_CASE( "utility get", "[utility]" )
  {
      namespace utl = yam::utl;

      constexpr ptrdiff_t c0=-10;
      constexpr ptrdiff_t c1=1538;
      constexpr ptrdiff_t c2=-289;
      constexpr ptrdiff_t c3=8;

      REQUIRE( utl::get<0>( c0,c1,c2,c3 ) == c0 );
      REQUIRE( utl::get<1>( c0,c1,c2,c3 ) == c1 );
      REQUIRE( utl::get<2>( c0,c1,c2,c3 ) == c2 );
      REQUIRE( utl::get<3>( c0,c1,c2,c3 ) == c3 );
  }

