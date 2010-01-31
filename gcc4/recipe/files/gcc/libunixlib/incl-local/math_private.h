#include <endian.h>
#include <sys/types.h>
#include <internal/math.h>

/* IBM MathLib flags.  */

#if __FLOAT_WORD_ORDER == __LITTLE_ENDIAN
/* The soft-float ABI represents FP words in little-endian format.  */
#define LITTLE_ENDI 1
#define HIGH_HALF 1
#define LOW_HALF 0
#else
/* The ARM FPA represents FP words in big-endian format with the
   word-order swapped.  */
#define BIG_ENDI 1
#define HIGH_HALF 0
#define LOW_HALF 1
#endif

