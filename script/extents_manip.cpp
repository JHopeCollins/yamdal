
# include <yamdal/external/mdspan.h>
# include <yamdal/utility.h>

# include <concepts>
# include <type_traits>
# include <iostream>

/*
   template<size_t N>
   [[nodiscard]]
   constexpr auto get( auto... vals )
  {
      return std::get<N>(std::tuple{vals...});
  }

   template<ptrdiff_t... Exts>
   [[nodiscard]]
   constexpr auto get_dynamic_extent_indices( std::integer_sequence<ptrdiff_t,Exts...> )
  {
      constexpr auto extent_is_dynamic =
         []<size_t I>
         ( std::integral_constant<size_t,I> )
        { return get<I>(Exts...)==stx::dynamic_extent; };

      return filter_sequence_c(
               std::make_index_sequence<sizeof...(Exts)>(),
               extent_is_dynamic );
  }

   template<ptrdiff_t... Exts>
   [[nodiscard]]
   constexpr auto get_dynamic_extent_indices( stx::extents<Exts...> )
  {
      return get_dynamic_extent_indices( std::integer_sequence<ptrdiff_t,Exts...>{} );
  }

   template<size_t             N,
            size_t        NewIdx,
            ptrdiff_t     NewVal,
            size_t...       Idxs,
            ptrdiff_t... OldExts>
      requires ( N <= sizeof...(OldExts) )
            && ( sizeof...(Idxs) == sizeof...(OldExts))
            && ( NewIdx < sizeof...(OldExts) )
   [[nodiscard]]
   constexpr auto impl_replace_nth( stx::extents<OldExts...> exts,
                                    std::index_sequence<Idxs...>,
                                    std::same_as<ptrdiff_t> auto... dynamic_exts )
  {
   // filter OldExts... for dynamic extents
      if constexpr( N<sizeof...(OldExts) )
     {
      // this extent will be replaced, so do not need to test if dynamic or static, just recurse
         if constexpr( N == NewIdx )
        {
            return impl_replace_nth<N+1,NewIdx,NewVal>(
                     exts,
                     std::index_sequence<Idxs...>(),
                     dynamic_exts... );
        }
      // extent N is dynamic, add its value to list and recurse to next extent
         else if constexpr( get<N>( OldExts... ) == stx::dynamic_extent )
        {
            return impl_replace_nth<N+1,NewIdx,NewVal>(
                     exts,
                     std::index_sequence<Idxs...>(),
                     dynamic_exts...,
                     exts.extent(N) );
        }
         else // extent N is static, just recurse to next extent
        {
            return impl_replace_nth<N+1,NewIdx,NewVal>(
                     exts,
                     std::index_sequence<Idxs...>(),
                     dynamic_exts... );
        }
     }
   // once all extents filtered, return new extents with replaced nth extent and dynamic extents copied
      else
     {
         constexpr auto replace_nth =
            []( size_t i, ptrdiff_t old )
           {
               return (i==NewIdx) ? NewVal : old;
           };

         using new_ext_t =
            stx::extents<replace_nth(Idxs,OldExts)...>;

         return new_ext_t(dynamic_exts...);
     }
  }

   template<size_t     NewIdx,
            ptrdiff_t  NewVal,
            ptrdiff_t... Exts>
   [[nodiscard]]
   constexpr auto replace_nth( stx::extents<Exts...> exts )
  {
      return impl_replace_nth<0,NewIdx,NewVal>(
                exts,
                std::make_index_sequence<sizeof...(Exts)>() );
  }

   template<size_t        NewIdx,   // index of extent to replace
            ptrdiff_t     NewVal,   // new value of extent to replace
            size_t...       Idxs,   // index sequence to enumerate extents
            ptrdiff_t... OldExts>   // old static extents
      requires ( sizeof...(Idxs) == sizeof...(OldExts))
            && ( NewIdx < sizeof...(Idxs) )
   [[nodiscard]]
   constexpr auto impl_replace_mth( stx::extents<OldExts...> exts,
                                    std::index_sequence<Idxs...> )
  {
   // replace static_extent(NewIdx) with NewVal
      constexpr auto new_extents =
         []( size_t i, ptrdiff_t old ) -> ptrdiff_t
        { return (i==NewIdx) ? NewVal : old; };

      using new_extents_t = stx::extents<new_extents(Idxs,OldExts)...>;

      constexpr std::integer_sequence new_dynamic_indices =
          get_dynamic_extent_indices( new_extents_t{} );

   // return new extents, copying dynamic values where needed
      return [&]<size_t... Jdxs>
             ( std::index_sequence<Jdxs...> )
     {
         return new_extents_t( exts.extent(Jdxs)... );
     }( new_dynamic_indices );
  }

   template<size_t     NewIdx,
            ptrdiff_t  NewVal,
            ptrdiff_t... Exts>
   [[nodiscard]]
   constexpr auto replace_mth( stx::extents<Exts...> exts )
  {
      return impl_replace_mth<NewIdx,NewVal>(
                exts,
                std::make_index_sequence<sizeof...(Exts)>() );
  }
*/

   int main()
  {
      constexpr size_t j=1;
      constexpr auto result =
         yam::utl::filter_sequence( std::index_sequence<0,1,2>{},
                          [](auto i){ return i!=j; } );

      using result_t = std::remove_cvref_t<decltype(result)>;

      static_assert( std::same_as<
                        std::index_sequence<0,2>,
                        result_t> );

      using ext3_2d9 = stx::extents<2,stx::dynamic_extent,9>;

      constexpr ext3_2d9 ex1(8);
      constexpr auto ex2 = yam::replace_nth_extent<2,12>(ex1);

      std::clog << ex1.extent(0) << "\n";
      std::clog << ex1.extent(1) << "\n";
      std::clog << ex1.extent(2) << "\n";
      std::clog << "\n";
      std::clog << decltype(ex1)::static_extent(0) << "\n";
      std::clog << decltype(ex1)::static_extent(1) << "\n";
      std::clog << decltype(ex1)::static_extent(2) << "\n";
      std::clog << "\n";

      std::clog << ex2.extent(0) << "\n";
      std::clog << ex2.extent(1) << "\n";
      std::clog << ex2.extent(2) << "\n";
      std::clog << "\n";
      std::clog << decltype(ex2)::static_extent(0) << "\n";
      std::clog << decltype(ex2)::static_extent(1) << "\n";
      std::clog << decltype(ex2)::static_extent(2) << "\n";
      std::clog << "\n";

      return 0;
  }

