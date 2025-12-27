#pragma once

#include "primitives.h"
#include <string.h>

struct string {
	const char *data;
	u32 len;
};

#define string_literal(str_lit) ((struct string){ .data = str_lit, .len = sizeof(str_lit) })
#define string_from_cstring(cstr) ((struct string){ .data = cstr, .len = strlen(cstr) })
#define string_format(str) (str).len, (str).data


bool string_equal(struct string left, struct string right);
