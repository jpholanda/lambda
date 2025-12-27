#include "memory.h"
#include "tokenizer.h"
#include "parser.h"
#include "eval.h"
#include <stdio.h>
#include <string.h>

constexpr u32 MAX_LINE_SIZE = 1 << 24;
char line[MAX_LINE_SIZE];

int main() {
	while (true) {
		fputs("λ> ", stdout);
		if (!fgets(line, MAX_LINE_SIZE, stdin)) {
			break;
		}

		struct tokenizer tokenizer = {
			.current_column = 1,
			.current_line = 1,
			.current_token = {},
			.input = line,
			.input_end = line + strlen(line)
		};

		struct expression *expr = expression_parse(&tokenizer, arena_allocator);
		expression_bind_variables(expr);

		struct expression *evaluated;
		for (u32 i = 0; i < 8; i++) {
			if (i != 0) {
				fputs("=β= ", stdout);
			}
			expression_print(expr);
			fputc('\n', stdout);
			evaluated = expression_eval(expr);
			if (evaluated == expr) {
				break;
			}
			expr = evaluated;
		}
		if (evaluated != expr) {
			puts("...\n");
		}
	}

	return 0;
}
