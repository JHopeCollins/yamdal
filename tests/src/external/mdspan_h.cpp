
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

   TEST_CASE( "get_dynamic_extent_indices", "[mdspan]" )
  {
      constexpr auto dyn = stx::dynamic_extent;

      constexpr auto ex2_53 = stx::extents<5,3>();
      const auto ex2_d3 = stx::extents<dyn,3>();
      const auto ex4_3d7d = stx::extents<3,dyn,7,dyn>();

      using result0_t = decltype(yam::get_dynamic_extent_indices(ex2_53));
      using result1_t = decltype(yam::get_dynamic_extent_indices(ex2_d3));
      using result2_t = decltype(yam::get_dynamic_extent_indices(ex4_3d7d));

      using expected0_t = std::index_sequence<>;
      using expected1_t = std::index_sequence<0>;
      using expected2_t = std::index_sequence<1,3>;

      REQUIRE( (std::same_as<result0_t,expected0_t>) );
      REQUIRE( (std::same_as<result1_t,expected1_t>) );
      REQUIRE( (std::same_as<result2_t,expected2_t>) );
  }

   TEST_CASE( "replace_nth_extent", "[mdspan]" )
  {
      constexpr auto dyn = stx::dynamic_extent;

      constexpr auto ex2_53 = stx::extents<5,3>();
      const auto ex2_d3 = stx::extents<dyn,3>(5);
      const auto ex4_3d7d = stx::extents<3,dyn,7,dyn>(9,4);

      using result0_t = decltype(yam::replace_nth_extent<1,6>(ex2_53));
      using result1_t = decltype(yam::replace_nth_extent<0,1>(ex2_d3));
      using result2_t = decltype(yam::replace_nth_extent<3,1>(ex4_3d7d));

      using expected0_t = stx::extents<5,6>;
      using expected1_t = stx::extents<1,3>;
      using expected2_t = stx::extents<3,dyn,7,1>;

   // check static extents are correct
      REQUIRE( (std::same_as<result0_t,expected0_t>) );
      REQUIRE( (std::same_as<result1_t,expected1_t>) );
      REQUIRE( (std::same_as<result2_t,expected2_t>) );

   // check dynamic extents are correct
      REQUIRE( ex4_3d7d.extent(1) == (yam::replace_nth_extent<3,1>(ex4_3d7d)).extent(1) );
  }

   TEST_CASE( "required_span_size", "[mdspan]" )
  {
      constexpr auto dyn = stx::dynamic_extent;

      using layout = stx::layout_left;

   // non-zero
      stx::extents<3,4,5> ex60{};
      stx::extents<2,dyn> ex16(8);

      REQUIRE( yam::required_span_size<layout>(ex60) == 60 );
      REQUIRE( yam::required_span_size<layout>(ex16) == 16 );

      REQUIRE(  (yam::required_span_size<layout>(stx::extents<3,4,5>{}))
              ==(yam::required_span_size<layout,3,4,5>()) );

      REQUIRE(  (yam::required_span_size<layout>(stx::extents<2,dyn>(8)))
              ==(yam::required_span_size<layout,2,dyn>(8)) );

   // zero
      stx::extents<3,4,0> ex0a;
      stx::extents<2,dyn> ex0b(0);

      REQUIRE( yam::required_span_size<layout>(ex0a) == 0 );
      REQUIRE( yam::required_span_size<layout>(ex0b) == 0 );
  }

