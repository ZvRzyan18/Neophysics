#ifndef NP_COMPILER_HINT_H
#define NP_COMPILER_HINT_H

/*
 compiler hints
*/

/*
 NOTE : Always use these, hoping that the compiler
  is smart enough to optimize the code further..
  use every advantage possible for optimization.
*/

#define NPLikely(x) (x)
#define NPUnlikely(x) (x)
#define NPUnreachable()

#define NP_HOT
#define NP_COLD

#define NP_INLINE static inline

#endif

