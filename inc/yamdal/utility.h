
# pragma once

# include <tuple>
# include <utility>
# include <concepts>
# include <algorithm>

namespace yam::utl
{
/*
 * get nth element of parameter pack
 */
   template<size_t N>
   [[nodiscard]]
   constexpr auto get( auto... vals )
      requires (N<sizeof...(vals))
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
      if constexpr( sizeof...(Ints) > 0 )
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

   template<typename T,
            size_t   N=64>
   struct alignas(std::max(N,sizeof(T))) aligned_t
  {
      constexpr static size_t line_length = N;

      using type = T;
      using ref = T&;
      using cref = const T&;

      type data;

//    explicit operator type() const { return data; }
      operator ref()        { return data; }
      operator cref() const { return data; }
  };

}
