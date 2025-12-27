#include "parser.h"
#include "str.h"
#include <assert.h>
#include <stdio.h>

void expression_bind_variable(struct expression *expression, struct string identifier, i32 de_brujin_index) {
	assert(expression != nullptr);

	switch (expression->type) {
	case EXPRESSIONTYPE_VARIABLE:
		if (string_equal(expression->as.variable.identifier, identifier)) {
			expression->as.variable.de_brujin_index = de_brujin_index;
		}
		return;

	case EXPRESSIONTYPE_ABSTRACTION:
		if (!string_equal(expression->as.abstraction.argument_variable, identifier)) {
			expression_bind_variable(expression->as.abstraction.body, identifier, de_brujin_index+1);
		}
		return;

	case EXPRESSIONTYPE_APPLY:
		expression_bind_variable(expression->as.apply.function, identifier, de_brujin_index);
		expression_bind_variable(expression->as.apply.argument, identifier, de_brujin_index);
		return;
	}
}

void expression_bind_variables(struct expression *expression) {
	switch (expression->type) {
	case EXPRESSIONTYPE_VARIABLE:
		return;

	case EXPRESSIONTYPE_ABSTRACTION:
		expression_bind_variable(expression->as.abstraction.body, expression->as.abstraction.argument_variable, 0);
		expression_bind_variables(expression->as.abstraction.body);
		return;

	case EXPRESSIONTYPE_APPLY:
		expression_bind_variables(expression->as.apply.function);
		expression_bind_variables(expression->as.apply.argument);
		return;
	}
}

struct expression *expression_subst(struct expression *expression, i32 de_brujin_index, struct expression *replacement) {
	switch (expression->type) {
	case EXPRESSIONTYPE_VARIABLE:
		if (expression->as.variable.de_brujin_index == de_brujin_index) {
			return replacement;
		}
		return expression;

	case EXPRESSIONTYPE_ABSTRACTION:
		expression->as.abstraction.body = expression_subst(expression->as.abstraction.body, de_brujin_index + 1, replacement);
		return expression;

	case EXPRESSIONTYPE_APPLY:
		expression->as.apply.function = expression_subst(expression->as.apply.function, de_brujin_index, replacement);
		expression->as.apply.argument = expression_subst(expression->as.apply.argument, de_brujin_index, replacement);
		return expression;
	}
}

struct expression *expression_eval(struct expression *expression) {
	assert(expression != nullptr);

	switch (expression->type) {
	case EXPRESSIONTYPE_VARIABLE:
	case EXPRESSIONTYPE_ABSTRACTION:
		return expression;

	case EXPRESSIONTYPE_APPLY:
		expression->as.apply.function = expression_eval(expression->as.apply.function);
		expression->as.apply.argument = expression_eval(expression->as.apply.argument);

		struct expression *function_expr = expression->as.apply.function;
		struct expression *argument_expr = expression->as.apply.argument;
		if (function_expr->type == EXPRESSIONTYPE_ABSTRACTION) {
			return expression_subst(
				function_expr->as.abstraction.body,
				0,
				argument_expr
			);
		}
		return expression;
	}
}

void expression_print(struct expression *expression) {
	if (!expression) {
		return;
	}

	switch (expression->type) {
	case EXPRESSIONTYPE_VARIABLE:
		printf("%.*s", string_format(expression->as.variable.identifier));
		return;

	case EXPRESSIONTYPE_ABSTRACTION:
		printf("\\%.*s. ",string_format(expression->as.abstraction.argument_variable));
		expression_print(expression->as.abstraction.body);
		return;

	case EXPRESSIONTYPE_APPLY:
		bool function_needs_parenthesis = expression->as.apply.function->type != EXPRESSIONTYPE_VARIABLE;
		if (function_needs_parenthesis) {
			fputc('(', stdout);
		}
		expression_print(expression->as.apply.function);
		if (function_needs_parenthesis) {
			fputc(')', stdout);
		}

		fputc(' ', stdout);

		bool argument_needs_parenthesis = expression->as.apply.argument->type != EXPRESSIONTYPE_VARIABLE;
		if (argument_needs_parenthesis) {
			fputc('(', stdout);
		}
		expression_print(expression->as.apply.argument);
		if (argument_needs_parenthesis) {
			fputc(')', stdout);
		}
		return;
	}
}
