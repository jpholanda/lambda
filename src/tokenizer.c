#include "tokenizer.h"
#include <ctype.h>

bool tokenizer_end(const struct tokenizer *tokenizer) {
	return tokenizer->input == tokenizer->input_end;
}

enum tokentype tokenizer_lookahead(struct tokenizer *tokenizer) {
	while (tokenizer->input != tokenizer->input_end && isspace(*tokenizer->input)) {
		tokenizer->input++;
	}
	if (tokenizer->input == tokenizer->input_end) {
		return TOKENTYPE_END;
	}

	switch (*tokenizer->input) {
	case '\\':
	case '.':
	case '(':
	case ')':
		return *tokenizer->input;
	default:
		if (isalnum(*tokenizer->input)) {
			return TOKENTYPE_IDENTIFIER;
		} else {
			return TOKENTYPE_INVALID;
		}
	}
}

void tokenizer_next(struct tokenizer *tokenizer) {
	tokenizer->current_token.type = tokenizer_lookahead(tokenizer);

	if (tokenizer->current_token.type == TOKENTYPE_IDENTIFIER) {
		tokenizer->current_token.identifier.data = tokenizer->input;
		tokenizer->input++;
		while (tokenizer->input != tokenizer->input_end && isalnum(*tokenizer->input)) {
			tokenizer->input++;
		}
		u32 chars_read = tokenizer->input - tokenizer->current_token.identifier.data;
		tokenizer->current_token.identifier.len = chars_read;
		tokenizer->current_column += chars_read;
	} else if (tokenizer->current_token.type != TOKENTYPE_INVALID) {
		tokenizer->input++;
		tokenizer->current_column++;
	}
}

const char *tokentype_descriptor(enum tokentype type) {
	switch (type) {
	case TOKENTYPE_END:
		return "end of input";
	case TOKENTYPE_IDENTIFIER:
		return "identifier";
	case TOKENTYPE_INVALID:
		return "unrecognized";
	case TOKENTYPE_SLASH:
		return "\\";
	case TOKENTYPE_DOT:
		return ".";
	case TOKENTYPE_OPENPAR:
		return "(";
	case TOKENTYPE_CLOSEPAR:
		return ")";
	}
}
