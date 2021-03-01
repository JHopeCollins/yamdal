
# include <yamdal/external/mdspan.h>

# include <catch.hpp>

   TEST_CASE( "stx::extents element ranges", "[mdspan]" )
  {
      constexpr auto dyn = stx::dynamic_extent;
   // 1D
      constexpr stx::extents<5> ex1_s;
      const stx::extents<dyn> ex1_d(5);

      REQUIRE( yam::num_elems(ex1_s) == 5 );
      REQUIRE( yam::num_elems(ex1_d) == 5 );

   // empty ranges
      constexpr stx::extents<0> ex1_s0;
      const stx::extents<dyn> ex1_d0{};

      REQUIRE( !yam::is_empty_range(ex1_s)  );
      REQUIRE( !yam::is_empty_range(ex1_d)  );

      REQUIRE(  yam::is_empty_range(ex1_s0) );
      REQUIRE(  yam::is_empty_range(ex1_d0) );

   // 2D
      using ex2_53_t = stx::extents<5,3>;
      using ex2_d3_t = stx::extents<dyn,3>;
      using ex2_5d_t = stx::extents<5,dyn>;
      using ex2_dd_t = stx::extents<dyn,dyn>;

      constexpr ex2_53_t ex2_s;
      const ex2_dd_t ex2_d(5,3);
      const ex2_5d_t ex2_sd(3);
      const ex2_d3_t ex2_ds(5);

      REQUIRE( yam::num_elems(ex2_s)  == 5*3 );
      REQUIRE( yam::num_elems(ex2_d)  == 5*3 );
      REQUIRE( yam::num_elems(ex2_sd) == 5*3 );
      REQUIRE( yam::num_elems(ex2_ds) == 5*3 );

   // range empty if any extent == 0
      REQUIRE( !yam::is_empty_range(ex2_s)  );
      REQUIRE( !yam::is_empty_range(ex2_d)  );
      REQUIRE( !yam::is_empty_range(ex2_sd) );
      REQUIRE( !yam::is_empty_range(ex2_ds) );

      REQUIRE( yam::is_empty_range( stx::extents<5,0>() ) );
      REQUIRE( yam::is_empty_range( stx::extents<0,3>() ) );
      REQUIRE( yam::is_empty_range( ex2_5d_t(0) ) );
      REQUIRE( yam::is_empty_range( ex2_d3_t(0) ) );
      REQUIRE( yam::is_empty_range( ex2_dd_t(0,0) ) );
  }
