
# pragma once

# include "../views.h"
# include "../concepts.h"
# include "../index.h"
# include "../execution.h"
# include "../utility.h"

# include "../serial/algorithm.h"
# include "../external/mdspan.h"

# include <utility>
# include <functional>
# include <concepts>
# include <vector>

# include <omp.h>

# include <cassert>

namespace yam
{
/*
 * OpenMP assign
 */
   template<indexable Destination,
            indexable Source,
            ptrdiff_t... Exts>
      requires same_grid_as<Destination,
                            Source>
            && std::assignable_from<element_type_of_t<Destination>,
                                    element_type_of_t<Source>>
            && (sizeof...(Exts)==ndim_of_v<Source>)
   void assign(       execution::openmp_policy,
                const index_type_of_t<Source> begin_index,
                const stx::extents<Exts...>          exts,
                      Destination&            destination,
                const Source&                      source )
  {
      using index_type = index_type_of_t<Source>;

   # pragma omp parallel for
      for( idx_t i=0; i<exts.extent(0); ++i )
     {
      // create block of only one i index, and all j,k,... etc indices
         index_type block_begin{begin_index};
         block_begin[0]+=i;

      // make the 0th extent a static size of 1
         const auto block_exts = replace_nth_extent<0,1>(exts);

      // each thread processes their own block sequentially
         assign( execution::seq,
                 block_begin,
                 block_exts,
                 destination,
                 source );
     }

      return;
  }

/*
 * OpenMP reduce
 */
   template<typename ReduceFunc,
            typename ReduceType,
            indexable    Source,
            ptrdiff_t...   Exts>
      requires reduction<ReduceFunc,
                         ReduceType,
                         element_type_of_t<Source>>
            && ( sizeof...(Exts) == ndim_of_v<Source> )
            && std::default_initializable<ReduceFunc>
   [[nodiscard]]
   ReduceType reduce(       execution::openmp_policy,
                      const index_type_of_t<Source> begin_index,
                      const stx::extents<Exts...>          exts,
                            ReduceFunc              reduce_func,
                            ReduceType               identity_v,
                            ReduceType                     init,
                      const Source&                      source )
  {
# pragma omp declare reduction( \
   my_reduce : \
   ReduceType : \
   omp_out = ReduceFunc{}( omp_out, omp_in ) ) \
   initializer ( omp_priv = omp_orig )

      using index_type = index_type_of_t<Source>;

   // thread local value for reduction
      auto local_val = identity_v;

# pragma omp parallel for reduction(my_reduce:local_val)
      for( idx_t i=0; i<exts.extent(0); ++i )
     {
      // create block of only one i index, and all j,k,... etc indices
         index_type block_begin{begin_index};
         block_begin[0]+=i;

      // make the 0th extent a static size of 1
         const auto block_exts = replace_nth_extent<0,1>(exts);

      // each thread processes their own block sequentially
         local_val =
            reduce( execution::seq,
                    block_begin,
                    block_exts,
                    reduce_func,
                    local_val,
                    identity_v,
                    source );
     }

      return reduce_func( local_val, init );
  }
/*
 * 1D OpenMP reduce
 */
   template<typename ReduceFunc,
            typename ReduceType,
            indexable    Source,
            ptrdiff_t...   Exts>
      requires reduction<ReduceFunc,
                         ReduceType,
                         element_type_of_t<Source>>
            && ( sizeof...(Exts) == ndim_of_v<Source> )
            && ( sizeof...(Exts) == 1 )
   [[nodiscard]]
   ReduceType reduce(       execution::openmp_policy,
                      const index_type_of_t<Source> begin_index,
                      const stx::extents<Exts...>          exts,
                            ReduceFunc              reduce_func,
                            ReduceType                     init,
                      const Source&                      source )
  {
   // reduce functor using std::invoke
      auto rfunc =
         [&]( auto&& arg0, auto&& arg1 )
        {
            return std::invoke( reduce_func,
                     std::forward<decltype(arg0)>(arg0),
                     std::forward<decltype(arg1)>(arg1) );
        };

   // reduction value for each thread, each on seperate cache lines
      using reduce_t = utl::aligned_t<ReduceType>;
      std::vector<reduce_t> thread_init;

# pragma omp parallel
     {
         using index_type = index_type_of_t<Source>;

         const ptrdiff_t nthreads = omp_get_num_threads();

      // create init values for each thread
   # pragma omp single
        {
            assert(nthreads>0);
            assert( exts.extent(0) >= 2*nthreads );
            thread_init.resize(size_t(nthreads));
        }

      // reference to local reduction value
         const auto thread_id = size_t(omp_get_thread_num());
         ReduceType& my_init = thread_init[thread_id];

      // initialise reduction value for each thread
   # pragma omp for
         for( idx_t i=0; i<nthreads; ++i )
        {
         // each thread uses index pairs: {0,1}, {2,3}, {4,5}... to initialise
            const idx_t j = 2*i + begin_index[0];

            const index_type idx0{j  };
            const index_type idx1{j+1};

            my_init = rfunc( source(idx0),
                             source(idx1) );
        }

      // reduce down rest of array
   # pragma omp for
         for( idx_t i=2*nthreads; i<exts.extent(0); ++i )
        {
            const index_type idx{i+begin_index[0]};

            my_init = rfunc( std::move( my_init ),
                             source(idx) );
        }
     }

   // reduce thread reduction values
      for( const auto& tli : thread_init )
     {
         init = rfunc( std::move(init),
                       tli );
     }

      return init;
  }
}
