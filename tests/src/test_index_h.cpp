
# include <yamdal/index.h>

# include <catch.hpp>

   TEST_CASE( "index accessors", "[index]" )
  {
      constexpr yam::index3<> idx3{0,1,2};

      REQUIRE( idx3[0] == 0 );
      REQUIRE( idx3[1] == 1 );
      REQUIRE( idx3[2] == 2 );

      yam::index2<yam::dual> idx2;
      idx2[0]=4;
      idx2[1]=5;

      REQUIRE( idx2[0] == 4 );
      REQUIRE( idx2[1] == 5 );
  }

   TEST_CASE( "index comparators", "[index]" )
  {
      constexpr yam::index2<> i0{0,1};
      constexpr yam::index2<> i1{0,1};
      constexpr yam::index2<> i2{1,1};
      constexpr yam::index2<> i3{1,0};

      REQUIRE( i0 == i1 );
      REQUIRE( i0 != i2 );
      REQUIRE( i0 != i3 );
  }

   TEST_CASE( "index grid conversions", "[index][grid]" )
  {
      constexpr yam::index2<yam::primal> ip0{12,13};
      constexpr yam::index2<yam::dual> id0(yam::to_dual(ip0));

   // conversion doesn't change indices
      REQUIRE( id0[0] == ip0[0] );
      REQUIRE( id0[1] == ip0[1] );

   // conversion to same grid doesn't change index
      REQUIRE( id0 == yam::to_dual(id0) );
      REQUIRE( ip0 == yam::to_primal(ip0) );

      constexpr auto ip1 = yam::to_primal(id0);
      constexpr auto id1 = yam::to_dual(ip1);

   // to_primal(to_dual) and vice-versa doesn't change index
      REQUIRE( ip1 == ip0 );
      REQUIRE( id1 == id0 );

   // to_grid<grid> acts same as to_primal/dual
      REQUIRE( id0 == yam::to_grid<yam::dual>( yam::to_grid<yam::primal>(ip1) ) );
  }

   TEST_CASE( "index element range", "[index]" )
  {
   // 1D
      constexpr yam::index begin1{3};
      constexpr yam::index end1{3+5};

      REQUIRE( yam::num_elems(begin1,end1) == 5 );
      REQUIRE( yam::num_elems(end1,begin1) == 0 );

      REQUIRE( !yam::is_empty_range(begin1,end1) );
      REQUIRE(  yam::is_empty_range(end1,begin1) );

   // 2D
      constexpr yam::dual_index2 begin2{3,-2};
      constexpr yam::dual_index2 end2{3+5,-2+3};

      REQUIRE( yam::num_elems(begin2,end2) == 5*3 );
      REQUIRE( yam::num_elems(end2,begin2) == 0 );

      REQUIRE( !yam::is_empty_range(begin2,end2) );
      REQUIRE(  yam::is_empty_range(end2,begin2) );
  }
