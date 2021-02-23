
# pragma once

# include "mdspan.h"

namespace yam::util
{
/*
 * get nth element of parameter pack
 */
   template<size_t N>
   [[nodiscard]]
   constexpr auto get( auto... vals )
  {
      return std::get<N>(std::tuple{vals...});
  }

   namespace detail
  {
      template<size_t             N,
               size_t        NewIdx,
               ptrdiff_t     NewVal,
               size_t...       Idxs,
               ptrdiff_t... OldExts>
         requires ( N <= sizeof...(OldExts) )
               && ( sizeof...(Idxs) == sizeof...(OldExts))
               && ( NewIdx < sizeof...(OldExts) )
      [[nodiscard]]
      constexpr auto impl_replace_nth_extent(
            stx::extents<OldExts...> exts,
            std::index_sequence<Idxs...>,
            std::same_as<ptrdiff_t> auto... dynamic_exts )
     {
      // filter OldExts... for dynamic extents
         if constexpr( N<sizeof...(OldExts) )
        {
         // this extent will be replaced, so do not need to test if dynamic or static, just recurse
            if constexpr( N == NewIdx )
           {
               return impl_replace_nth_extent<N+1,NewIdx,NewVal>(
                        exts,
                        std::index_sequence<Idxs...>(),
                        dynamic_exts... );
           }
         // extent N is dynamic, add its value to list and recurse to next extent
            else if constexpr( get<N>( OldExts... ) == stx::dynamic_extent )
           {
               return impl_replace_nth_extent<N+1,NewIdx,NewVal>(
                        exts,
                        std::index_sequence<Idxs...>(),
                        dynamic_exts...,
                        exts.extent(N) );
           }
            else // extent N is static, just recurse to next extent
           {
               return impl_replace_nth_extent<N+1,NewIdx,NewVal>(
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

      template<size_t             N,
               size_t        NewIdx,
               size_t...       Idxs,
               ptrdiff_t... OldExts>
         requires ( N <= sizeof...(OldExts) )
               && ( sizeof...(Idxs) == sizeof...(OldExts))
               && ( NewIdx < sizeof...(OldExts) )
      [[nodiscard]]
      constexpr auto impl_replace_nth_extent(
            ptrdiff_t new_val,
            stx::extents<OldExts...> exts,
            std::index_sequence<Idxs...>,
            std::same_as<ptrdiff_t> auto... dynamic_exts )
     {
      // filter OldExts... for dynamic extents
         if constexpr( N<sizeof...(OldExts) )
        {
         // this is extent to be replaced, so need to add on new_value to list
            if constexpr( N == NewIdx )
           {
               return impl_replace_nth_extent<N+1,NewIdx>(
                        new_val,
                        exts,
                        std::index_sequence<Idxs...>(),
                        dynamic_exts...,
                        new_val );
           }
         // extent N is dynamic, add its value to list and recurse to next extent
            else if constexpr( get<N>( OldExts... ) == stx::dynamic_extent )
           {
               return impl_replace_nth_extent<N+1,NewIdx>(
                        new_val,
                        exts,
                        std::index_sequence<Idxs...>(),
                        dynamic_exts...,
                        exts.extent(N) );
           }
            else // extent N is static, just recurse to next extent
           {
               return impl_replace_nth_extent<N+1,NewIdx>(
                        new_val,
                        exts,
                        std::index_sequence<Idxs...>(),
                        dynamic_exts... );
           }
        }
      // once all extents filtered, return new extents, replacing nth extent with dynamic_extent, and all other extents copied
         else
        {
            constexpr auto replace_nth =
               []( size_t i, ptrdiff_t old )
              {
                  return (i==NewIdx) ? stx::dynamic_extent : old;
              };
   
            using new_ext_t =
               stx::extents<replace_nth(Idxs,OldExts)...>;
   
            return new_ext_t(dynamic_exts...);
        }
  }
  }

/*
 * replace the NewIdx-th extent of exts with the (static) NewVal
 */
   template<size_t     NewIdx,
            ptrdiff_t  NewVal,
            ptrdiff_t... Exts>
   [[nodiscard]]
   constexpr auto replace_nth_extent( stx::extents<Exts...> exts )
  {
      return detail::impl_replace_nth_extent<0,NewIdx,NewVal>(
                exts,
                std::make_index_sequence<sizeof...(Exts)>() );
  }

/*
 * replace the NewIdx-th extent of exts with the (dynamic) new_val
 */
   template<size_t     NewIdx,
            ptrdiff_t... Exts>
   [[nodiscard]]
   constexpr auto replace_nth_extent( ptrdiff_t new_val,
                                      stx::extents<Exts...> exts )
  {
      return detail::impl_replace_nth_extent<0,NewIdx>(
                new_val,
                exts,
                std::make_index_sequence<sizeof...(Exts)>() );
  }
}
