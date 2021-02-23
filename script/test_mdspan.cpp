
# include <yamdal/mdspan.h>

# include <array>
# include <iostream>

   int main()
  {
      int* data=new int[100];
//    const auto mds2 = stx::mdspan<int,stx::dynamic_extent,2>(data,4);
      const auto mds2 = stx::mdspan<int,4,2>(data);

//    const auto size = mds2.size();
//    const auto required_size = mds2.mapping().required_span_size();
//    const auto ex0 = mds2.extent(0);
//    const auto ex1 = mds2.extent(1);

      mds2(0,0)=15;
      mds2(std::array{1,2})=12;

//    std::cout << ex0 << "\n";
//    std::cout << ex1 << "\n";
//    std::cout << size << "\n";
//    std::cout << required_size << "\n";
//    std::cout << mds2(std::array{0,0}) << "\n";
//    std::cout << mds2(1,2) << "\n";

      const stx::extents<1,3,stx::dynamic_extent> exts(5);

      std::cout << exts.extent(0) << "\n";
      std::cout << exts.extent(1) << "\n";
      std::cout << exts.extent(2) << "\n";

      delete[] data;

      return 0;
  }
