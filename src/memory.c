#include "memory.h"
#include <assert.h>

// from https://www.reddit.com/r/C_Programming/comments/jpt7dw/comment/gbhl5xp/
#if defined(__GNUC__) || defined(__clang__)
  static inline u64 popcount64(u64 x) {
    return __builtin_popcountll(x);
  }
#elif defined(_MSC_VER) && defined(_WIN64)
  #include <intrin.h>
  static inline u64 popcount64(u64 x) {
    return __popcnt64(x);
  }
#else
  /* http://en.wikipedia.org/wiki/Hamming_weight */
  static inline u64 popcount64(u64 x) {
    x -= (x >> 1) & 0x5555555555555555;
    x = (x & 0x3333333333333333) + ((x >> 2) & 0x3333333333333333);
    x = (x + (x >> 4)) & 0x0f0f0f0f0f0f0f0f;
    return (x * 0x0101010101010101) >> 56;
  }
#endif

constexpr u32 ARENA_SIZE = 1 << 24;
u8 arena[ARENA_SIZE];
u8 *current = &arena[0];

void *arena_alloc_(u64 alignment, u64 size) {
	assert(popcount64(alignment) == 1);
	uaddr current_addr = (uaddr)current;
	uaddr next_addr = (current_addr & -alignment) + alignment;
	u8 *next = (u8 *)next_addr;
	current = next + size;
	assert(current <= arena + ARENA_SIZE);
	return next;
}

const allocator_func arena_allocator = arena_alloc_;
