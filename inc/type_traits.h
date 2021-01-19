
# pragma once

# include <yamdal.h>

# include <cpp20/concepts.h>
# include <cpp20/type_traits.h>

# include <type_traits>

namespace yam
{
/*
 * specialisations for static constants
 */
   template<ndim_t ndim>
   using ndim_constant = std::integral_constant<ndim_t,ndim>;

   template<grid_t grid>
   using grid_constant = std::integral_constant<grid_t,grid>;

/*
 * ===============================================================
 *
 * Type traits to test if a type is a specialisation of the basic yamdal template types (index, shape etc).
 * Cannot do simple generic 'is_same_template' type trait because of non-type template parameters
 *
 * ===============================================================
 */

/*
 * Returns if a type is a specialisation of yam::index
 */
   // false
   template<typename A>
   struct is_index_type : std::false_type {};

   // true
   template<ndim_t ndim,
            grid_t grid>
   struct is_index_type<index<ndim,grid>> : std::true_type {};

// helper variable template
   template<typename A>
   inline constexpr bool is_index_type_v = is_index_type<A>::value;

/*
 * Returns if a type is a specialisation of yam::shape
 */
   // false
   template<typename A>
   struct is_shape_type : std::false_type {};

   // true
   template<ndim_t ndim,
            grid_t grid>
   struct is_shape_type<shape<ndim,grid>> : std::true_type {};

// helper variable template
   template<typename A>
   inline constexpr bool is_shape_type_v = is_shape_type<A>::value;

/*
 * ===============================================================
 *
 * Type traits to test if an indexable has helper members
 *    These helper members give information about the grid that an indexable is defined over, eg the dimension, and on related types, eg the index type of the grid
 *
 * ===============================================================
 */

/*
 * Returns if type has a static member variable for the number of dimensions
 */
   // false
   template<typename A>
   struct has_ndim_member : std::false_type {};

   // true
   template<typename A>
      requires requires(){ { A::ndim } -> ndim_t; }
   struct has_ndim_member<A> : std::true_type {};

// helper variable template
   template<typename A>
   inline constexpr bool has_ndim_member_v = has_ndim_member<A>::value;

/*
 * Returns if type has a static member variable for the grid type
 */
   // false
   template<typename A>
   struct has_grid_member : std::false_type {};

   // true
   template<typename A>
      requires requires(){ { A::grid } -> grid_t; }
   struct has_grid_member<A> : std::true_type {};

// helper variable template
   template<typename A>
   inline constexpr bool has_grid_member_v = has_grid_member<A>::value;

/*
 * Returns if type has a type alias for the index
 */
   // fase
   template<typename A>
   struct has_index_type_member : std::false_type {};

   // true
   template<typename A>
      requires requires(){ typename A::index_type; }
            && is_index_type_v<typename A::index_type>
   struct has_index_type_member<A> : std::true_type {};

// helper variable template
   template<typename A>
   inline constexpr bool has_index_type_member_v = has_index_type_member<A>::value;

/*
 * Returns if type has a type alias for the shape
 */
   // false
   template<typename A>
   struct has_shape_type_member : std::false_type {};

   // true
   template<typename A>
      requires requires(){ typename A::shape_type; }
            && is_shape_type_v<typename A::shape_type>
   struct has_shape_type_member<A> : std::true_type {};

// helper variable template
   template<typename A>
   inline constexpr bool has_shape_type_member_v = has_shape_type_member<A>::value;

/*
 * Returns if type has a type alias for the element type
 */
   // false
   template<typename A>
   struct has_element_type_member : std::false_type {};

   // true
   template<typename A>
      requires requires(){ typename A::element_type; }
   struct has_element_type_member<A> : std::true_type {};

// helper variable template
   template<typename A>
   inline constexpr bool has_element_type_member_v = has_element_type_member<A>::value;

/*
 * ===============================================================
 *
 * Type traits related to deducing information about the underlying grid of an indexable:
 *    This information includes values, eg the dimension, and types, eg the index type of the grid
 *    Type traits of form "x_of" deduce grid information from an indexable
 *    Type traits of form "has_x" return if grid information can be deduced (checks if the corresponding "x_of" type trait compile)
 *       Checking these before using the "x_of" type traits gives better error messages 
 *
 * ===============================================================
 */

/*
 * Returns the number of dimensions of the grid over which the indexable A is defined.
 * The behaviour of a program for which A is not indexable, or is indexable with with multiple index types is undefined.
 */

// no definition if ndim not deducible
   template<typename A> struct ndim_of;

// specialisation with helper member
   template<typename A>
      requires has_ndim_member_v<A>
   struct ndim_of<A> : ndim_constant<A::ndim> {};

// specialisations without helper member
   // 1D
   template<typename A>
      requires !has_ndim_member_v<A>
            && (   cpp::invocable<A,index<1,primal>>
                || cpp::invocable<A,index<1,dual>> )
   struct ndim_of<A> : ndim_constant<1> {};

   // 2D
   template<typename A>
      requires !has_ndim_member_v<A>
            && (   cpp::invocable<A,index<2,primal>>
                || cpp::invocable<A,index<2,dual>> )
   struct ndim_of<A> : ndim_constant<2> {};

   // 3D
   template<typename A>
      requires !has_ndim_member_v<A>
            && (   cpp::invocable<A,index<3,primal>>
                || cpp::invocable<A,index<3,dual>> )
   struct ndim_of<A> : ndim_constant<3> {};

// helper variable template
   template<typename A>
   inline constexpr ndim_t ndim_of_v = ndim_of<A>::value;

/*
 * Return whether ndim_of<A> is able to deduce the dimension of the underlying grid
 */
   template<typename A>
   struct has_ndim : std::false_type {};

   template<typename A>
      requires requires(){ { ndim_of<A>::value } -> ndim_t; }
   struct has_ndim<A> : std::true_type {};

// helper variable template
   template<typename A>
   inline constexpr bool has_ndim_v = has_ndim<A>::value;

/*
 * Returns the type (primal or dual) of the grid over which the indexable A is defined
 * The behaviour of a program for which A is not indexable, or is indexable with with multiple index types is undefined.
 */

// no definition if grid not deducible
   template<typename A> struct grid_of;

// specialisation with helper member
   template<typename A>
      requires has_grid_member_v<A>
   struct grid_of<A> : grid_constant<A::grid> {};

// specialisations without helper member
   // primal
   template<typename A>
      requires !has_grid_member_v<A>
            && (   cpp::invocable<A,index<1,primal>>
                || cpp::invocable<A,index<2,primal>>
                || cpp::invocable<A,index<3,primal>> )
   struct grid_of<A> : grid_constant<primal> {};

   // dual
   template<typename A>
      requires !has_grid_member_v<A>
            && (   cpp::invocable<A,index<1,dual>>
                || cpp::invocable<A,index<2,dual>>
                || cpp::invocable<A,index<3,dual>> )
   struct grid_of<A> : grid_constant<dual> {};

// helper variable template
   template<typename A>
   inline constexpr grid_t grid_of_v = grid_of<A>::value;

/*
 * Return whether grid_of<A> is able to deduce the gridtype of the underlying grid
 */
   template<typename A>
   struct has_grid : std::false_type {};

   template<typename A>
      requires requires(){ { grid_of_v<A> } -> grid_t; }
   struct has_grid<A> : std::true_type {};

// helper variable template
   template<typename A>
   inline constexpr bool has_grid_v = has_grid<A>::value;

/*
 * Returns the index type of the grid over which the indexable A is defined
 * The behaviour of a program for which A is not indexable, or is indexable with multiple index types is undefined.
 */

// no definition if index_type not deducible
   template<typename A> struct index_type_of;

// specialisation with helper member
   template<typename A>
      requires has_index_type_member_v<A>
   struct index_type_of<A> : cpp::type_identity<typename A::index_type> {};

// specialisation without helper member
   template<typename A>
      requires !has_index_type_member_v<A>
            && has_ndim_v<A>
            && has_grid_v<A>
   struct index_type_of<A> : cpp::type_identity<index<ndim_of_v<A>,
                                                      grid_of_v<A>>> {};

// helper type template
   template<typename A>
   using index_type_of_t = typename index_type_of<A>::type;

/*
 * Return whether index_type_of<A> is able to deduce the index type of the underlying grid
 */
   template<typename A>
   struct has_index_type : std::false_type {};

   template<typename A>
      requires requires(){ typename index_type_of<A>::type; }
   struct has_index_type<A> : std::true_type {};

// helper variable template
   template<typename A>
   inline constexpr bool has_index_type_v = has_index_type<A>::value;

/*
 * Returns the shape type of the grid over which the indexable A is defined
 * The behaviour of a program for which A is not indexable, or is indexable with multiple shape types is undefined.
 */

// no definition if shape_type not deducible
   template<typename A> struct shape_type_of;

// specialisation with helper member
   template<typename A>
      requires has_shape_type_member_v<A>
   struct shape_type_of<A> : cpp::type_identity<typename A::shape_type> {};

// specialisation without helper member
   template<typename A>
      requires !has_shape_type_member_v<A>
            && has_ndim_v<A>
            && has_grid_v<A>
   struct shape_type_of<A> : cpp::type_identity<shape<ndim_of_v<A>,
                                                      grid_of_v<A>>> {};

// helper type template
   template<typename A>
   using shape_type_of_t = typename shape_type_of<A>::type;

/*
 * Return whether shape_type_of<A> is able to deduce the shape type of the underlying grid
 */
   template<typename A>
   struct has_shape_type : std::false_type {};

   template<typename A>
      requires requires(){ typename shape_type_of<A>::type; }
   struct has_shape_type<A> : std::true_type {};

// helper variable template
   template<typename A>
   inline constexpr bool has_shape_type_v = has_shape_type<A>::value;

/*
 * ===============================================================
 *
 * Type traits related to deducing if multiple types are defined on compatible grids
 *
 * ===============================================================
 */

/*
 * Return whether the types A and As... are defined over grids of the same number of dimensions
 */
   template<typename    A,
            typename... As>
      requires has_ndim_v<A>
            && ((has_ndim_v<As>)&&...)
   struct has_same_ndim : std::bool_constant<((ndim_of_v<A> == ndim_of_v<As>)&&...)> {};

// helper variable template
   template<typename    A,
            typename... As>
   inline constexpr bool has_same_ndim_v = has_same_ndim<A,As...>::value;

/*
 * Return whether the types A and As... are defined over grids of the type
 */
   template<typename    A,
            typename... As>
      requires has_grid_v<A>
            && ((has_grid_v<As>)&&...)
   struct has_same_grid : std::bool_constant<((grid_of_v<A> == grid_of_v<As>)&&...)> {};

// helper variable template
   template<typename    A,
            typename... As>
   inline constexpr bool has_same_grid_v = has_same_grid<A,As...>::value;

/*
 * Return whether the types A and As... are defined over grids with the same index
 */
   template<typename    A,
            typename... As>
      requires has_index_type_v<A>
            && ((has_index_type_v<As>)&&...)
   struct has_same_index_type : std::bool_constant<((std::is_same_v<index_type_of_t<A>,
                                                                    index_type_of_t<As>>)&&...)> {};

// helper variable template
   template<typename    A,
            typename... As>
   inline constexpr bool has_same_index_type_v = has_same_index_type<A,As...>::value;

/*
 * ===============================================================
 *
 * Type traits related to deducing the element type of an indexable
 *
 * ===============================================================
 */

/*
 * Deduces the return type of calling type A with a single argument of type index<ndim,grid>.
 */
   template<typename  A,
            ndim_t ndim,
            grid_t grid= primal>
      requires cpp::invocable<A,index<ndim,grid>>
   struct element_type_with : cpp::type_identity<std::invoke_result_t<A,index<ndim,grid>>> {};

// helper type template
   template<typename  A,
            ndim_t ndim,
            grid_t grid= primal>
   using element_type_with_t = typename element_type_with<A,ndim,grid>::type;

/*
 * Deduces the return type of calling type A with a deduced index type. 
 * The behaviour of a program for which A is not indexable, or is indexable with with multiple index types is undefined.
 */
   template<typename A>
      requires has_index_type_v<A>
            && cpp::invocable<A,index_type_of_t<A>>
   struct element_type_of : cpp::type_identity<std::invoke_result_t<A,index_type_of_t<A>>> {};

   template<typename A>
   using element_type_of_t = typename element_type_of<A>::type;
}
