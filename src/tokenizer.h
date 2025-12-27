#pragma once

#include "primitives.h"
#include "str.h"

enum tokentype {
	TOKENTYPE_INVALID,
	TOKENTYPE_END,
	TOKENTYPE_IDENTIFIER,
	TOKENTYPE_SLASH		= '\\',
	TOKENTYPE_DOT 		= '.',
	TOKENTYPE_OPENPAR 	= '(',
	TOKENTYPE_CLOSEPAR	= ')',
};

struct token {
	enum tokentype type;
	struct string identifier;
};

struct tokenizer {
	struct token current_token;
	const char *input;
	const char *input_end;
	u32 current_line;
	u32 current_column;
};

enum tokentype tokenizer_lookahead(struct tokenizer *tokenizer);
void tokenizer_next(struct tokenizer *tokenizer);

const char *tokentype_descriptor(enum tokentype type);
