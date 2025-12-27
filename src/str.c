#include "str.h"

bool string_equal(struct string left, struct string right) {
	if (left.len != right.len) {
		return false;
	}

	for (u32 i = 0; i < left.len; i++) {
		if (left.data[i] != right.data[i]) {
			return false;
		}
	}

	return true;
}
