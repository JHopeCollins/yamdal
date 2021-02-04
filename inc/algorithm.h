
# pragma once

# include <execution.h>
# include <concepts.h>
# include <yamdal.h>

# include <concepts>

namespace yam
{
/*
 * ===============================================================
 *
 * yam::assign
 * Assign values in range [begin_index,end_index) from one indexable to another indexable
 *
 * ===============================================================
 */

/*
 * Serial execution --------------------------------------------------
 */

/*
 * 1D grid
 */
   template<indexable1 DST,
            indexable1 SRC>
      requires compatible_grids<SRC,DST>
            && std::assignable_from<element_type_of_t<DST>,
                                    element_type_of_t<SRC>>
   constexpr void assign(       execution::serial_policy,
                          const index_type_of_t<SRC>& begin_index,
                          const index_type_of_t<SRC>&   end_index,
                                DST&  dst,
                          const SRC&  src )
  {
      if( num_elems( begin_index, end_index ) == 0 ){ return; }

      for( idx_t i=begin_index[0]; i<end_index[0]; ++i )
     {
         const index_type_of_t<SRC> idx{i};
         dst(idx) = src(idx);
     }
      return;
  }

/*
 * 2D grid
 */
   template<indexable2 DST,
            indexable2 SRC>
      requires compatible_grids<SRC,DST>
            && std::assignable_from<element_type_of_t<DST>,
                                    element_type_of_t<SRC>>
   constexpr void assign(       execution::serial_policy,
                          const index_type_of_t<SRC>& begin_index,
                          const index_type_of_t<SRC>&   end_index,
                                DST&  dst,
                          const SRC&  src )
  {
      if( num_elems( begin_index, end_index ) == 0 ){ return; }

      for( idx_t i=begin_index[0]; i<end_index[0]; ++i )
     {
         for( idx_t j=begin_index[1]; j<end_index[1]; ++j )
        {
            const index_type_of_t<SRC> idx{i,j};
            dst(idx) = src(idx);
        }
     }
      return;
  }

/*
 * 3D grid
 */
   template<indexable3 DST, indexable3 SRC>
      requires compatible_grids<SRC,DST>
            && std::assignable_from<element_type_of_t<DST>,
                                    element_type_of_t<SRC>>
   constexpr void assign(       execution::serial_policy,
                          const index_type_of_t<SRC>& begin_index,
                          const index_type_of_t<SRC>&   end_index,
                                DST&  dst,
                          const SRC&  src )
  {
      if( num_elems( begin_index, end_index ) == 0 ){ return; }

      for( idx_t i=begin_index[0]; i<end_index[0]; ++i )
     {
         for( idx_t j=begin_index[1]; j<end_index[1]; ++j )
        {
            for( idx_t k=begin_index[2]; k<end_index[2]; ++k )
           {
               const index_type_of_t<SRC> idx{i,j,k};
               dst(idx) = src(idx);
           }
        }
     }
      return;
  }


/*
 * OpenMP execution --------------------------------------------------
 */

# ifdef _OPENMP
/*
 * 1D grid
 */
   template<indexable1 DST,
            indexable1 SRC>
      requires compatible_grids<SRC,DST>
            && std::assignable_from<element_type_of_t<DST>,
                                    element_type_of_t<SRC>>
   constexpr void assign(       execution::openmp_policy,
                          const index_type_of_t<SRC>& begin_index,
                          const index_type_of_t<SRC>&   end_index,
                                DST&  dst,
                          const SRC&  src )
  {
      if( num_elems( begin_index, end_index ) == 0 ){ return; }

   # pragma omp parallel for
      for( idx_t i=begin_index[0]; i<end_index[0]; ++i )
     {
         const index_type_of_t<SRC> idx{i};
         dst(idx) = src(idx);
     }
      return;
  }

/*
 * 2D grid
 */
   template<indexable2 DST,
            indexable2 SRC>
      requires compatible_grids<SRC,DST>
            && std::assignable_from<element_type_of_t<DST>,
                                    element_type_of_t<SRC>>
   constexpr void assign(       execution::openmp_policy,
                          const index_type_of_t<SRC>& begin_index,
                          const index_type_of_t<SRC>&   end_index,
                                DST&  dst,
                          const SRC&  src )
  {
      if( num_elems( begin_index, end_index ) == 0 ){ return; }

   # pragma omp parallel for
      for( idx_t i=begin_index[0]; i<end_index[0]; ++i )
     {
         for( idx_t j=begin_index[1]; j<end_index[1]; ++j )
        {
            const index_type_of_t<SRC> idx{i,j};
            dst(idx) = src(idx);
        }
     }
      return;
  }

/*
 * 3D grid
 */
   template<indexable3 DST,
            indexable3 SRC>
      requires compatible_grids<SRC,DST>
            && std::assignable_from<element_type_of_t<DST>,
                                    element_type_of_t<SRC>>
   constexpr void assign(       execution::openmp_policy,
                          const index_type_of_t<SRC>& begin_index,
                          const index_type_of_t<SRC>&   end_index,
                                DST&  dst,
                          const SRC&  src )
  {
      if( num_elems( begin_index, end_index ) == 0 ){ return; }

   # pragma omp parallel for
      for( idx_t i=begin_index[0]; i<end_index[0]; ++i )
     {
         for( idx_t j=begin_index[1]; j<end_index[1]; ++j )
        {
            for( idx_t k=begin_index[2]; k<end_index[2]; ++k )
           {
               const index_type_of_t<SRC> idx{i,j,k};
               dst(idx) = src(idx);
           }
        }
     }
      return;
  }

# endif

/*
 * Convenience overloads -----------------------------------------
 */

/*
 * if no execution policy is specified, use serial
 */
   template<indexable DST,
            indexable SRC>
      requires compatible_grids<SRC,DST>
            && std::assignable_from<element_type_of_t<DST>,
                                    element_type_of_t<SRC>>
   constexpr void assign( const index_type_of_t<SRC>& begin_index,
                          const index_type_of_t<SRC>&   end_index,
                                DST&  dst,
                          const SRC&  src )
  {
      assign( execution::seq,
              begin_index, end_index,
              dst, src );
      return;
  }


/*
 * ===============================================================
 *
 * yam::transform
 *    yam::transform applies the given function TFunc to a variadic pack of indexables
 *    Two overload types are available
 *       1) returns an indexable lambda that lazily evaluates the transform when called with an index
 *       2) eagerly evaluate the transformation and assign result into given indexable
 *
 * ===============================================================
 */

/*
 * lazy transform
 */
   template<typename    TFunc,
            indexable    Src0,
            indexable... Srcs>
      requires compatible_grids<Src0,Srcs...>
            && transformation<TFunc,
                              element_type_of_t<Src0>,
                              element_type_of_t<Srcs>...>
   [[nodiscard]]
   constexpr auto transform( TFunc  tfunc,
                             Src0    src0,
                             Srcs... srcs )
  {
      using index_type = common_index_type_t<Src0,Srcs...>;

      return [=]( index_type idx )
     {
         return std::invoke( tfunc, src0(idx), srcs(idx)... );
     };
  }

/*
 * eager transform
 */
   template<typename          Index,
            typename          TFunc,
            indexable           Dst,
            indexable...       Srcs>
      requires compatible_grids<Dst,Srcs...>
            && std::same_as<Index,index_type_of_t<Dst>>
            && transformation_r<element_type_of_t<Dst>,
                                TFunc,
                                element_type_of_t<Srcs>...>
   constexpr void transform(       execution_policy auto policy,
                             const Index            begin_index,
                             const Index              end_index,
                                   Dst&                     dst,
                                   TFunc                  tfunc,
                             const Srcs&...                srcs )
  {
      assign( policy,
              begin_index, end_index,
              dst, transform( tfunc, yam::view(srcs)... ) );
  }

/*
 * Convenience overload for eager serial execution
 */
   template<typename    Index,
            typename    TFunc,
            indexable     Dst,
            indexable... Srcs>
      requires compatible_grids<Dst,Srcs...>
            && std::same_as<Index,index_type_of_t<Dst>>
            && transformation_r<element_type_of_t<Dst>,
                                TFunc,
                                element_type_of_t<Srcs>...>
   constexpr void transform( Index   begin_index,
                             Index     end_index,
                             Dst&            dst,
                             TFunc         tfunc,
                             Srcs...        srcs )
  {
      transform( execution::seq,
                 begin_index, end_index,
                 dst, tfunc, srcs... );
  }

}
