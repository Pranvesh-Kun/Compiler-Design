#pragma once
#include "value.h"
#include <unordered_map>
#include "../parser/ast.h"

class Interpreter {
  public:
    Value evaluate(ExpressionNode*);
    void execute(StatementNode*);
    void execute(ProgramNode*);
    void interpreter_error(std::string);
    std::unordered_map<std::string, Value> variables;
};