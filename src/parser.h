#pragma once

#include "memory.h"
#include "tokenizer.h"
#include "str.h"

enum expression_type {
	EXPRESSIONTYPE_VARIABLE,
	EXPRESSIONTYPE_ABSTRACTION,
	EXPRESSIONTYPE_APPLY
};

struct expression {
	enum expression_type type;
	union {
		struct {
			struct string identifier;
			i32 de_brujin_index; // negative denotes free variable!
		} variable;
		struct {
			struct string argument_variable;
			struct expression *body;
		} abstraction;
		struct {
			struct expression *function;
			struct expression *argument;
		} apply;
	} as;
};

struct expression *expression_parse(struct tokenizer *tokenizer, allocator_func allocator);
