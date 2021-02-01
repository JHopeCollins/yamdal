
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
      requires has_same_grid_v<SRC,DST>
            && std::assignable_from<element_type_of_t<DST>,
                                    element_type_of_t<SRC>>
   constexpr void assign(       execution::serial_policy,
                          const index_type_of_t<SRC>& begin_index,
                          const index_type_of_t<SRC>&   end_index,
                                DST&  dst,
                          const SRC&  src )
  {
      if( !(begin_index < end_index) ){ return; }

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
      requires has_same_grid_v<SRC,DST>
            && std::assignable_from<element_type_of_t<DST>,
                                    element_type_of_t<SRC>>
   constexpr void assign(       execution::serial_policy,
                          const index_type_of_t<SRC>& begin_index,
                          const index_type_of_t<SRC>&   end_index,
                                DST&  dst,
                          const SRC&  src )
  {
      if( !(begin_index < end_index) ){ return; }

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
      requires has_same_grid_v<SRC,DST>
            && std::assignable_from<element_type_of_t<DST>,
                                    element_type_of_t<SRC>>
   constexpr void assign(       execution::serial_policy,
                          const index_type_of_t<SRC>& begin_index,
                          const index_type_of_t<SRC>&   end_index,
                                DST&  dst,
                          const SRC&  src )
  {
      if( !(begin_index < end_index) ){ return; }

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
      requires has_same_grid_v<SRC,DST>
            && std::assignable_from<element_type_of_t<DST>,
                                    element_type_of_t<SRC>>
   constexpr void assign(       execution::openmp_policy,
                          const index_type_of_t<SRC>& begin_index,
                          const index_type_of_t<SRC>&   end_index,
                                DST&  dst,
                          const SRC&  src )
  {
      if( !(begin_index < end_index) ){ return; }

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
      requires has_same_grid_v<SRC,DST>
            && std::assignable_from<element_type_of_t<DST>,
                                    element_type_of_t<SRC>>
   constexpr void assign(       execution::openmp_policy,
                          const index_type_of_t<SRC>& begin_index,
                          const index_type_of_t<SRC>&   end_index,
                                DST&  dst,
                          const SRC&  src )
  {
      if( !(begin_index < end_index) ){ return; }

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
      requires has_same_grid_v<SRC,DST>
            && std::assignable_from<element_type_of_t<DST>,
                                    element_type_of_t<SRC>>
   constexpr void assign(       execution::openmp_policy,
                          const index_type_of_t<SRC>& begin_index,
                          const index_type_of_t<SRC>&   end_index,
                                DST&  dst,
                          const SRC&  src )
  {
      if( !(begin_index < end_index) ){ return; }

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
      requires has_same_ndim_v<SRC,DST>
            && has_same_grid_v<SRC,DST>
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

}
