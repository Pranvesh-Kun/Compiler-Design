#pragma once
#include "value.h"
#include "../parser/ast.h"

class Interpreter {
  public:
    Value evaluate(ExpressionNode*);
    void execute(StatementNode*);
    void interpreter_error(std::string);
};