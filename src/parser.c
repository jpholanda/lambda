#include "tokenizer.h"
#include "parser.h"
#include "memory.h"
#include <stdio.h>

bool expect(struct tokenizer *tokenizer, enum tokentype type) {
	tokenizer_next(tokenizer);

	if (tokenizer->current_token.type != type) {
		fprintf(stderr,
			"at line %d, column %d: expected %s, got %s\n",
			tokenizer->current_line,
			tokenizer->current_column,
			tokentype_descriptor(type),
			tokentype_descriptor(tokenizer->current_token.type));
		return false;
	}

	return true;
}

struct expression *expression_parse_single(struct tokenizer *tokenizer, allocator_func allocator) {
	enum tokentype next_token = tokenizer_lookahead(tokenizer);
	switch (next_token) {
		default:
		case TOKENTYPE_END: {
			return nullptr;
		}

		case TOKENTYPE_INVALID: {
			fprintf(stderr, "at line %d, column %d: invalid character %c\n",
				tokenizer->current_line,
				tokenizer->current_column,
				*tokenizer->input);
			return nullptr;
		}

		case TOKENTYPE_OPENPAR: {
			tokenizer_next(tokenizer);
			struct expression *inner = expression_parse(tokenizer, allocator);
			if (!expect(tokenizer, TOKENTYPE_CLOSEPAR)) {
				return nullptr;
			}
			return inner;
		}

		case TOKENTYPE_IDENTIFIER: {
			tokenizer_next(tokenizer);
			struct expression *result = allocator(alignof(struct expression), sizeof(struct expression));
			*result = (struct expression){
				.type = EXPRESSIONTYPE_VARIABLE,
				.as = {
					.variable = {
						.identifier = tokenizer->current_token.identifier,
						.de_brujin_index = -1 // initialize all variables as free
					}
				}
			};
			return result;
		}

		case TOKENTYPE_SLASH: {
			const u32 starting_line = tokenizer->current_line;
			const u32 starting_column = tokenizer->current_column;

			tokenizer_next(tokenizer);
			if (!expect(tokenizer, TOKENTYPE_IDENTIFIER)) {
				return nullptr;
			}

			struct string argument_variable = tokenizer->current_token.identifier;

			if (!expect(tokenizer, TOKENTYPE_DOT)) {
				return nullptr;
			}

			struct expression *body = expression_parse(tokenizer, allocator);
			if (!body) {
				fprintf(stderr,
					"inside function starting in line %d, column %d\n",
					starting_line,
					starting_column
				);
				return nullptr;
			}

			struct expression *result = allocator(alignof(struct expression), sizeof(struct expression));
			*result = (struct expression){
				.type = EXPRESSIONTYPE_ABSTRACTION,
				.as = {
					.abstraction = {
						.argument_variable = argument_variable,
						.body = body
					}
				}
			};
			return result;
		}
	}
}

struct expression *expression_parse(struct tokenizer *tokenizer, allocator_func allocator) {
	struct expression *result = nullptr;

	while (true) {
		struct expression *next_expression = expression_parse_single(tokenizer, allocator);
		if (next_expression == nullptr) {
			break;
		}

		if (!result) {
			result = next_expression;
			continue;
		}

		struct expression *previous_expression = result;
		result = allocator(alignof(struct expression), sizeof(struct expression));
		*result = (struct expression){
			.type = EXPRESSIONTYPE_APPLY,
			.as = {
				.apply = {
					.function = previous_expression,
					.argument = next_expression
				}
			}
		};
	}

	return result;
}
