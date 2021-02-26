
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

/*
 * Concatenate multiple integer_sequences
 */
// single sequence case
   template<std::integral Integral,
            Integral...       Ints>
   [[nodiscard]]
   constexpr std::integer_sequence<Integral,Ints...>
   concat_sequences(
      std::integer_sequence<Integral,Ints...> )
  {
      return {};
  }

// two sequences case
   template<std::integral Integral,
            Integral...      Ints0,
            Integral...      Ints1>
   [[nodiscard]]
   constexpr std::integer_sequence<Integral,Ints0...,Ints1...>
   concat_sequences(
      std::integer_sequence<Integral,Ints0...>,
      std::integer_sequence<Integral,Ints1...> )
  {
      return {};
  }

// multiple sequences case
   template<std::integral Integral,
            Integral...      Ints0,
            Integral...      Ints1,
            typename...     Others>
   [[nodiscard]]
   constexpr auto
   concat_sequences(
      std::integer_sequence<Integral,Ints0...>,
      std::integer_sequence<Integral,Ints1...>,
      Others... )
  {
      return concat_sequences( std::integer_sequence<Integral,Ints0...,Ints1...>{},
                               Others{}... );
  }

/*
 * Filter integer sequence
 */
   template<std::integral Integral,
            Integral...       Ints,
            typename     Predicate>
      requires requires( Predicate p,
                         Integral  i )
                       { { p(i) } -> std::convertible_to<bool>; }
   [[nodiscard]]
   constexpr auto filter_sequence(
      std::integer_sequence<Integral,Ints...>,
      Predicate predicate )
  {
      constexpr auto filter_single =
         []<Integral I>
         ( std::integer_sequence<Integral,I>,
           Predicate p )
        {
            if constexpr(p(I)){ return std::integer_sequence<Integral,I>{}; }
            else /* !(p(I)) */{ return std::integer_sequence<Integral>{}; }
        };

   // recurse down list
      if constexpr( sizeof...(Ints) > 1 )
     {
         return concat_sequences(
                  filter_single(
                     std::integer_sequence<Integral,Ints>{},
                     predicate )... );
     }
   // empty list case
      else
     {
         return std::integer_sequence<Integral>{};
     }
  }

/*
 * return a std::index_sequence containing the indices of the dynamic extents (==stx::dynamic_extent)
 *    eg stx::extents<1,stx::dynamic_extent,6,stx::dynamic_extent> will return std::index_sequence<1,3>
 */
   template<ptrdiff_t... Exts>
   [[nodiscard]]
   constexpr auto get_dynamic_extent_indices(
      std::integer_sequence<ptrdiff_t,Exts...> )
  {
   // enumerate extent list
      using Indices = decltype(std::make_index_sequence<sizeof...(Exts)>());

      return []<size_t... Idxs>( std::index_sequence<Idxs...> )
     {
         constexpr auto extent_is_dynamic =
            []( size_t i )
           {
               return (( (i==Idxs) && (Exts==stx::dynamic_extent) )||...);
           };

         return filter_sequence(
                  Indices{},
                  extent_is_dynamic );

     }( Indices{} );
  }

   template<ptrdiff_t... Exts>
   [[nodiscard]]
   constexpr auto get_dynamic_extent_indices(
      stx::extents<Exts...> )
  {
      return get_dynamic_extent_indices( std::integer_sequence<ptrdiff_t,Exts...>{} );
  }

/*
 * replace the NewIdx-th extent of exts with the (dynamic) new_val
 */
   template<size_t     NewIdx,
            ptrdiff_t  NewVal,
            ptrdiff_t... Exts>
   [[nodiscard]]
   constexpr auto replace_nth_extent(
      stx::extents<Exts...> exts )
  {
   // lambda just to get Idxs...
      return [&]<size_t... Idxs>( std::index_sequence<Idxs...> )
     {
      // replace static_extent(NewIdx) with NewVal
         constexpr auto new_extents =
            []( size_t i, ptrdiff_t old ) -> ptrdiff_t
           { return (i==NewIdx) ? NewVal : old; };

         using new_extents_t =
            stx::extents<new_extents(Idxs,Exts)...>;

         constexpr std::integer_sequence new_dynamic_indices =
             get_dynamic_extent_indices( new_extents_t{} );

      // return new extents, copying dynamic values where needed
         return [&]<size_t... Jdxs>
                ( std::index_sequence<Jdxs...> )
        {
            return new_extents_t( exts.extent(Jdxs)... );
        }( new_dynamic_indices );
     }( std::make_index_sequence<sizeof...(Exts)>() );
  }
}
