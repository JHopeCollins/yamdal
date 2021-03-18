
# include <yamdal/all.h>

# include <iostream>
# include <memory>
# include <functional>
# include <limits>

   int main()
  {
      using integer = int;
      using index_t = yam::index<1>;

      constexpr size_t n0 = 10;

      auto buf = std::make_unique<integer[]>(100);

      auto mds1 = yam::span<integer,n0>(buf.get());
      const index_t begin_index{0};
      const stx::extents<n0> exts;

      yam::generate(
         begin_index, exts,
         mds1,
         [i=0]() mutable { return i++; } );

      for( yam::idx_t i=0; i<yam::idx_t(n0); ++i )
     {
         std::cout << mds1({i}) << "\n";
     }

      constexpr auto policy = yam::execution::openmp;

      const auto sum =
         yam::reduce( policy,
            begin_index, exts,
            std::plus<integer>{},
            0,
            mds1 );

      const auto mn =
         yam::reduce( policy,
            begin_index, exts,
            []( integer l, integer r ){ return std::min(l,r); },
            std::numeric_limits<integer>::max(),
            mds1 );

      const auto mx =
         yam::reduce( policy,
            begin_index, exts,
            []( integer l, integer r ){ return std::max(l,r); },
            std::numeric_limits<integer>::min(),
            mds1 );

      std::cout << "sum: " << sum << "\n";
      std::cout << "min: " << mn  << "\n";
      std::cout << "max: " << mx  << "\n";
            

      return 0;
  }
