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
    std::vector<std::unordered_map<std::string, Value>> scopes;
    std::unordered_map<std::string, Value>::iterator lookupVariable(std::string);
    void assignVariable(IdentifierNode*, Value);
    std::unordered_map<std::string, FunctionDeclarationNode*> functions;
};

struct Scope {
  std::vector<std::unordered_map<std::string, Value>>& scopes;
  Scope(auto& s) : scopes(s) {
    scopes.push_back(std::unordered_map<std::string, Value>());
  }; 
  ~Scope() {
    scopes.pop_back();
  }
};

struct ReturnException {
  Value value;
};

struct BreakException {

};

struct ContinueException {

};