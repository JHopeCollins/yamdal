
# include <yamdal/views.h>

# include <catch.hpp>

# include <array>

// indexable 1D array of type T and size N
   template<typename T,
            size_t   N,
            yam::grid_t GRID=yam::primal>
   struct array1D
  {
      static constexpr yam::ndim_t ndim = 1;
      static constexpr yam::grid_t grid = GRID;

      using index_type = yam::index<ndim,grid>;

      std::array<T,N> data;
   
      [[nodiscard]] constexpr
      T& operator()( index_type idx )
     {
         const auto i = size_t(idx[0]);
         return data[i];
     }

      [[nodiscard]] constexpr
      const T& operator()( index_type idx ) const
     {
         const auto i = size_t(idx[0]);
         return data[i];
     }
  };

// indexable 2D array of type T and size {N0 x N1}
   template<typename T,
            size_t  N0,
            size_t  N1,
            yam::grid_t GRID=yam::primal>
   struct array2D
  {
      static constexpr yam::ndim_t ndim = 2;
      static constexpr yam::grid_t grid = GRID;

      using index_type = yam::index<ndim,grid>;

      std::array<std::array<T,N1>,N0> data;
   
      [[nodiscard]] constexpr
      T& operator()( index_type idx )
     {
         const auto i = size_t(idx[0]);
         const auto j = size_t(idx[1]);
         return data[i][j];
     }

      [[nodiscard]] constexpr
      const T& operator()( index_type idx ) const
     {
         const auto i = size_t(idx[0]);
         const auto j = size_t(idx[1]);
         return data[i][j];
     }
  };

   TEST_CASE( "non-const view for non-const lvalue 1D source" "[views][1D]" )
  {
      using integer = yam::idx_t;
      using index = yam::index1<>;

      constexpr size_t n=4;

   // indexable array
      yam::indexable1_r<integer> auto arr = array1D<integer,n>{};

      yam::view1_r<integer> auto vw = yam::window( arr );

      static_assert( sizeof(decltype(vw)) < sizeof(decltype(arr)) );

      for( integer i=0; i<integer(n); ++i )
     {
         const index ix{i};

         arr(ix) = i;
         REQUIRE( vw(ix) == arr(ix) );

         const auto y = 2*i;
         vw(ix) = y;
         REQUIRE( arr(ix) == y );

         REQUIRE( &(vw(ix)) == &(arr(ix)) );
     }
  }

   TEST_CASE( "const view for non-const lvalue 1D source" "[views][1D]" )
  {
      using integer = yam::idx_t;
      using index = yam::index1<>;

      constexpr size_t n=4;

   // indexable array
      yam::indexable1_r<integer> auto arr = array1D<integer,n>{};

      yam::view1_r<integer> auto cvw = yam::cwindow( arr );

      static_assert( sizeof(decltype(cvw)) < sizeof(decltype(arr)) );

      for( integer i=0; i<integer(n); ++i )
     {
         const index ix{i};

         arr(ix) = i;
         REQUIRE(   cvw(ix)  ==   arr(ix)  );
         REQUIRE( &(cvw(ix)) == &(arr(ix)) );
     }
  }

