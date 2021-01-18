
# include <concepts.h>
# include <type_traits.h>
# include <layouts.h>
# include <yamdal.h>

   int main()
  {
      constexpr yam::primal_index1 ip1{7};
      constexpr yam::dual_index<2> id2{3,4};

      constexpr yam::primal_shape2 sp1{ip1[0],id2[1]};

      constexpr yam::primal_shape2 sp2(yam::to_primal(sp1));
      constexpr yam::dual_shape2 sd2(yam::to_dual(sp2));

      return 0;
  }
