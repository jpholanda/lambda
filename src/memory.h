#pragma once

#include "primitives.h"

typedef void *(*allocator_func)(u64 alignment, u64 size);

extern const allocator_func arena_allocator;
