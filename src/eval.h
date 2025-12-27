#pragma once

#include "parser.h"

void expression_bind_variables(struct expression *expression);
void expression_print(struct expression *expression);
struct expression *expression_eval(struct expression *expression);
