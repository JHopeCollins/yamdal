
# pragma once

# include "concepts.h"
# include "type_traits.h"
# include "index.h"

namespace yam
{
/*
 * ===============================================================
 *
 * yam::window / yam::cwindow
 * simple implementation of a view over an indexable
 *    return_type of window(index) is the same as return_type of source indexable (possibly a modifiying lvalue_ref, possibly not, depending on source)
 *    return_type of cwindow(index) is the const-modified return_type of source indexable (cannot modify source)
 *
 * ===============================================================
 */

/*
 * window is a possibly-modifying view over a source indexable
 */

// overload for lvalue references - capture source by reference
   template<indexable I>
   constexpr view auto window( I& source )
  {
      using index_type  = index_type_of_t<I>;
      using return_type = element_type_of_t<I>;

      return [ &source ]
            ( const index_type i ) -> return_type
           { return source(i); };
  }

// overload for rvalue references - capture source by value
   template<indexable I>
   constexpr view auto window( I&& source )
  {
      using index_type  = index_type_of_t<I>;
      using return_type = element_type_of_t<I>;

      return [ src{std::move(source)} ]
            ( const index_type i ) -> return_type
           { return src(i); };
  }

/*
 * cwindow is a non-modifying view over a source indexable
 */

// overload for lvalue references - capture source by reference
   template<indexable I>
   constexpr view auto cwindow( I& source )
  {
      using index_type  = index_type_of_t<I>;
      using return_type = const element_type_of_t<I>;

      return [ &source ]
            ( const index_type i ) -> return_type
           { return source(i); };
  }

// overload for rvalue references - capture source by value
   template<indexable I>
   constexpr view auto cwindow( I&& source )
  {
      using index_type  = index_type_of_t<I>;
      using return_type = const element_type_of_t<I>;

      return [ src{std::move(source)} ]
            ( const index_type i ) -> return_type
           { return src(i); };
  }
}
