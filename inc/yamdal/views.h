
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
   template<typename I>
      requires indexable<const I>
   [[nodiscard]]
   constexpr view auto window( const I& source )
  {
      using index_type  = index_type_of_t<I>;
      using return_type = element_type_of_t<decltype(source)>;

   // trailing return type ensures that the view returns a reference if the source returns a reference
      return [ &source ]
            ( index_type i ) -> return_type
           { return source(i); };
  }

   template<indexable I>
      requires (!std::is_const_v<I>)
   [[nodiscard]]
   constexpr view auto window( I& source )
  {
      using index_type  = index_type_of_t<I>;
      using return_type = element_type_of_t<decltype(source)>;

   // trailing return type ensures that the view returns a reference if the source returns a reference
      return [ &source ]
            ( index_type i ) -> return_type
           { return source(i); };
  }

/*
 * cwindow is a non-modifying view over a source indexable
 */
   template<indexable I>
   [[nodiscard]]
   constexpr view auto cwindow( const I& source )
  {
      using index_type  = index_type_of_t<I>;

      using element_type = element_type_of_t<decltype(source)>;

      using return_type =
         std::conditional_t<std::is_reference_v<element_type>,
                            const std::remove_reference_t<element_type>&,
                            element_type>;

      return [ &source ]
            ( index_type i ) -> return_type
           { return source(i); };
  }

}
