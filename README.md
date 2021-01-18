# yamdal - yet another multi-dimensional array library

yamdal is a c++ library for computations over multi-dimensional arrays.

yamdal is built on the concept of `indexable` types which map from a multi-dimensional index to data (any type that has a function call operator with a single `yam::index` parameter is `indexable`).
This concept includes types for which the mapping is:
* a memory access (arrays),
* a computation (generators)
* a combination of memory access and computation (views and other lazy evaluations).

Computations are expressed as algorithms (such as transform and reduce) over `indexable` types, so are agnostic to exactly which index->data mapping is used by each type. Many algorithms can either be evaluated eagerly, or can return `indexable` views to be evaluated lazily.

The initial version supports only serial evaluation. Future versions will support multithreaded evaluation with OpenMP, and possibly evaluation on GPUs with OpenACC.

The initial version supports only lexicographical order for sweeping through indices when evaluating an algorithm (row-major ordering). Future versions will support different index orderings to maximise cache efficiency depending on the memory layout of the underlying data.
