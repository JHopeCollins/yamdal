
# pragma once

# include <experimental/mdspan>
# include "../utility.h"
# include "../index.h"

# include <type_traits>
# include <array>
# include <utility>

# include <cassert>

namespace stx = std::experimental;

namespace yam
{

// default policy types for memory layouts / accessors

   using default_layout = stx::layout_right;

   template<typename ElementType>
   using default_accessor = stx::accessor_basic<ElementType>;

/*
 * calculation of number of elements
 */
// number of elements in a grid of a given shape
   template<ptrdiff_t... Exts>
   [[nodiscard]]
   constexpr size_t num_elems( const stx::extents<Exts...> exts )
  {
      return [&]<auto... Idxs>( std::index_sequence<Idxs...> ) -> size_t
     {
         return size_t((std::max<ptrdiff_t>(0,exts.extent(Idxs))*...));
     }(std::make_index_sequence<sizeof...(Exts)>());
  }

   template<ptrdiff_t... Exts>
   [[nodiscard]]
   constexpr bool is_empty_range( const stx::extents<Exts...> exts )
  {
      return num_elems(exts)==0;
  }

/*
 * return a std::index_sequence containing the indices of the dynamic extents (==stx::dynamic_extent)
 *    eg stx::extents<1,stx::dynamic_extent,6,stx::dynamic_extent> will return std::index_sequence<1,3>
 */
   template<ptrdiff_t... Exts>
   [[nodiscard]]
   constexpr auto dynamic_extent_indices(
      std::integer_sequence<ptrdiff_t,Exts...> )
  {
   // enumerate extent list
      using indices_t =
         decltype(std::make_index_sequence<sizeof...(Exts)>());

   // immediately-invoked lambda to use Idxs... enumeration
      return []<size_t... Idxs>( std::index_sequence<Idxs...> )
     {

      // returns true iff ( get<i>(Exts...) == stx::dynamic_extent )
         constexpr auto extent_is_dynamic =
            []( size_t i ) -> bool
           {
               return (( (i==Idxs) && (Exts==stx::dynamic_extent) )||...);
           };

      // actual return statement for dynamic_extent_indices
         return utl::filter_sequence(
                  indices_t{},
                  extent_is_dynamic );

     }( indices_t{} ); // argument for ii-lambda
  }

   template<ptrdiff_t... Exts>
   [[nodiscard]]
   constexpr auto dynamic_extent_indices(
      stx::extents<Exts...> )
  {
      return dynamic_extent_indices( std::integer_sequence<ptrdiff_t,Exts...>{} );
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
             dynamic_extent_indices( new_extents_t{} );

      // return new extents, copying dynamic values where needed
         return [&]<size_t... Jdxs>
                ( std::index_sequence<Jdxs...> )
        {
            return new_extents_t( exts.extent(Jdxs)... );
        }( new_dynamic_indices );
     }( std::make_index_sequence<sizeof...(Exts)>() );
  }

/*
 * return required size of underlying memory that would be required for an mdspan with layout `LayoutPolicy` and extents `exts`
 *    note extents may be static or run-time, but function is only allowed in a constexpr environment if all extents are static
 */
   template<typename   LayoutPolicy,
            ptrdiff_t...       Exts>
   [[nodiscard]]
   constexpr auto required_span_size(
      stx::extents<Exts...> exts )
  {
      if( std::is_constant_evaluated() )
     {
         assert( ((Exts!=stx::dynamic_extent)&&...) );
     }

      using extents_t = stx::extents<Exts...>;
      using mapping_t = typename LayoutPolicy::template mapping<extents_t>;
      return mapping_t(exts).required_span_size();
  }

   template<typename   LayoutPolicy,
            ptrdiff_t...       Exts>
   [[nodiscard]]
   constexpr auto required_span_size(
      std::convertible_to<ptrdiff_t> auto... dynamic_exts )
  {
      return required_span_size<LayoutPolicy>(
         stx::extents<Exts...>(dynamic_exts...) );
  }

/*
 * access elements of an stx::mdspan using a yam::index
 */
   template<typename    ElementType,
            typename        Extents,
            typename   LayoutPolicy,
            typename AccessorPolicy,
            ndim_t             ndim,
            grid_t             grid>
      requires (Extents::rank()==ndim)
   [[nodiscard]]
   constexpr auto element_access(
      const stx::basic_mdspan<ElementType,
                              Extents,
                              LayoutPolicy,
                              AccessorPolicy>& spn,
            yam::index<ndim,grid>              idx )
      -> typename AccessorPolicy::reference
  {
      using reference = typename AccessorPolicy::reference;

      return [&]<size_t... Idxs>
            ( std::index_sequence<Idxs...> ) -> reference
           {
               return spn(idx[Idxs]...);
           }( std::make_index_sequence<ndim>{} );
  }

   template<ptrdiff_t... Exts>
   [[nodiscard]]
   auto dynamic_extents( const stx::extents<Exts...>& exts )
  {
      return [&exts]<size_t... Idxs>
            ( std::index_sequence<Idxs...> )
           {
               using array_t =
                  std::array<ptrdiff_t,
                             stx::extents<Exts...>::rank_dynamic()>;

               return array_t{exts.extent(Idxs)...};
           }( dynamic_extent_indices( exts ) );
  }

}

