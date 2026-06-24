#include "astprint.h"
#include <iostream>

void indent(int depth) {
  for (int i = 0; i<depth; i++) std::cout << "  ";
}

void printAST(ASTNode* node, int depth = 0) {
  indent(depth);
  if (node == nullptr) {
      std::cout << "NULL\n";
      return;
  }
  if (auto n = dynamic_cast<ProgramNode*>(node)) {
    std::cout << "ProgramNode:\n";
    for (auto it: n->statements) {
      printAST(it, depth+1);
    }
  }
  else if (auto n = dynamic_cast<IntLiteralNode*>(node)) {
    std::cout << "IntLiteralNode:\n";
    indent(depth+1);
    std::cout << "value: ";
    std::cout << n->value << "\n";
  }
  else if (auto n = dynamic_cast<FloatLiteralNode*>(node)) {
    std::cout << "FloatLiteralNode:\n";
    indent(depth+1);
    std::cout << "value: ";
    std::cout << n->value << "\n";
  }
  else if (auto n = dynamic_cast<BoolLiteralNode*>(node)) {
    std::cout << "BoolLiteralNode:\n";
    indent(depth+1);
    std::cout << "value: ";
    std::cout << std::boolalpha << n->value << "\n";
  }
  else if (auto n = dynamic_cast<StringLiteralNode*>(node)) {
    std::cout << "StringLiteralNode:\n";
    indent(depth+1);
    std::cout << "value: ";
    std::cout << n->value << "\n";
  }
  else if (auto n = dynamic_cast<ArrayLiteralNode*>(node)) {
    std::cout << "ArrayLiteralNode:\n";
    indent(depth+1);
    std::cout << "elements:\n";
    for (auto it: n->elements) printAST(it, depth+2);
  }
  else if (auto n = dynamic_cast<IdentifierNode*>(node)) {
    std::cout << "IdentifierNode:\n";
    indent(depth+1);
    std::cout << "name: ";
    std::cout << n->name << "\n";
  }
  else if (auto n = dynamic_cast<IndexAccessNode*>(node)) {
    std::cout << "IndexAccessNode:\n";
    indent(depth+1);
    std::cout << "object:\n";
    printAST(n->object, depth+2);
    indent(depth+1);
    std::cout << "index:\n";
    printAST(n->index, depth+2);
  }
  else if (auto n = dynamic_cast<BinaryExpressionNode*>(node)) {
    std::cout << "BinaryExpressionNode(";
    std::cout << tokenmap[n->operation] << ")\n";
    indent(depth+1);
    std::cout << "left:\n";
    printAST(n->left, depth+2);
    indent(depth+1);
    std::cout << "right:\n";
    printAST(n->right, depth+2);
  }
  else if (auto n = dynamic_cast<UnaryExpressionNode*>(node)) {
    std::cout << "UnaryExpressionNode(";
    std::cout << tokenmap[n->operation] << ")\n";
    indent(depth+1);
    std::cout << "operand:\n";
    printAST(n->operand, depth+2);
  }
  else if (auto n = dynamic_cast<ParameterNode*>(node)) {
    std::cout << "ParameterNode:\n";
    indent(depth+1);
    std::cout << "name:\n";
    printAST(n->name, depth+2);
    indent(depth+1);
    std::cout << "type: ";
    std::cout << tokenmap[n->type] << "\n";
    if (n->type != TokenType::KW_ARRAY) return;
    indent(depth+1);
    std::cout << "subtype: ";
    std::cout << tokenmap[n->subtype] << "\n";
  }
  else if (auto n = dynamic_cast<FunctionCallNode*>(node)) {
    std::cout << "FunctionCallNode:\n";
    indent(depth+1);
    std::cout << "name:\n";
    printAST(n->name, depth+2);
    indent(depth+1);
    std::cout << "arguments:\n";
    for (auto it: n->arguments) printAST(it, depth+2);
  }
    else if (auto n = dynamic_cast<VariableDeclarationNode*>(node)) {
    std::cout << "VariableDeclarationNode:\n";
    indent(depth+1);
    std::cout << "name:\n";
    printAST(n->name, depth+2);
    indent(depth+1);
    std::cout << "type: ";
    std::cout << tokenmap[n->type] << "\n";
    indent(depth+1);
    if (n->type == TokenType::KW_ARRAY) {
      std::cout << "subtype: ";
      std::cout << tokenmap[n->subtype] << "\n";
      indent(depth+1);
      std::cout << "size:\n";
      printAST(n->size, depth+2);
      indent(depth+1);
    }
    std::cout << "initializer:\n";
    printAST(n->initializer, depth+2);
  }
  else if (auto n = dynamic_cast<AssignmentNode*>(node)) {
    std::cout << "AssignmentNode:\n";
    indent(depth+1);
    std::cout << "name:\n";
    printAST(n->name, depth+2);
    indent(depth+1);
    std::cout << "value:\n";
    printAST(n->value, depth+2);
  }
  else if (auto n = dynamic_cast<IncrementNode*>(node)) {
    std::cout << "IncrementNode:\n";
    indent(depth+1);
    std::cout << "name:\n";
    printAST(n->name, depth+2);
  }
  else if (auto n = dynamic_cast<DecrementNode*>(node)) {
    std::cout << "DecrementNode:\n";
    indent(depth+1);
    std::cout << "name:\n";
    printAST(n->name, depth+2);
  }
  else if (auto n = dynamic_cast<IndexAssignmentNode*>(node)) {
    std::cout << "IndexAssignmentNode:\n";
    indent(depth+1);
    std::cout << "name:\n";
    printAST(n->object, depth+2);
    indent(depth+1);
    std::cout << "index:\n";
    printAST(n->index, depth+2);
    indent(depth+1);
    std::cout << "value:\n";
    printAST(n->value, depth+2);
  }
  else if (auto n = dynamic_cast<ElseIfNode*>(node)) {
    std::cout << "ElseIfNode:\n";
    indent(depth+1);
    std::cout << "condition:\n";
    printAST(n->condition, depth+2);
    indent(depth+1);
    std::cout << "codeblock:\n";
    for (auto it: n->codeblock) printAST(it, depth+2);
  }
  else if (auto n = dynamic_cast<IfStatementNode*>(node)) {
    std::cout << "IfStatementNode:\n";
    indent(depth+1);
    std::cout << "condition:\n";
    printAST(n->condition, depth+2);
    indent(depth+1);
    std::cout << "codeblock:\n";
    for (auto it: n->codeblock) printAST(it, depth+2);
    indent(depth+1);
    std::cout << "elseif:\n";
    for (auto it: n->elseif) printAST(it, depth+2);
    indent(depth+1);
    std::cout << "elseblock:\n";
    for (auto it: n->elseblock) printAST(it, depth+2);
  }
  else if (auto n = dynamic_cast<WhileLoopNode*>(node)) {
    std::cout << "WhileLoopNode:\n";
    indent(depth+1);
    std::cout << "condition:\n";
    printAST(n->condition, depth+2);
    indent(depth+1);
    std::cout << "codeblock:\n";
    for (auto it: n->codeblock) printAST(it, depth+2);
  }  
  else if (auto n = dynamic_cast<ForLoopNode*>(node)) {
    std::cout << "ForLoopNode:\n";
    indent(depth+1);
    std::cout << "iterator:\n";
    printAST(n->iterator, depth+2);
    indent(depth+1);
    std::cout << "iterable:\n";
    printAST(n->iterable, depth+2);
    indent(depth+1);
    std::cout << "codeblock:\n";
    for (auto it: n->codeblock) printAST(it, depth+2);
  }
  else if (auto n = dynamic_cast<FunctionDeclarationNode*>(node)) {
    std::cout << "FunctionDeclarationNode:\n";
    indent(depth+1);
    std::cout << "name:\n";
    printAST(n->name, depth+2);
    indent(depth+1);
    std::cout << "returntype: ";
    std::cout << tokenmap[n->returntype] << "\n";
    indent(depth+1);
    std::cout << "returnsubtype: ";
    std::cout << tokenmap[n->returnsubtype] << "\n";
    indent(depth+1);
    std::cout << "parameters:\n";
    for (auto it: n->parameters) printAST(it, depth+2);
    indent(depth+1);
    std::cout << "body:\n";
    for (auto it: n->body) printAST(it, depth+2);
  }
  else if (auto n = dynamic_cast<ReturnNode*>(node)) {
    std::cout << "ReturnNode:\n";
    indent(depth+1);
    std::cout << "value:\n";
    printAST(n->value, depth+2);
  }
  else if (auto n = dynamic_cast<BreakNode*>(node)) {
    std::cout << "BreakNode:\n";
  }
  else if (auto n = dynamic_cast<ContinueNode*>(node)) {
    std::cout << "ContinueNode:\n";
  }
  else if (auto n = dynamic_cast<ExpressionStatementNode*>(node)) {
    std::cout << "ExpressionStatementNode:\n";
    indent(depth+1);
    std::cout << "expression:\n";
    printAST(n->expression, depth+2);
  }
  else {
      std::cout << "Unknown Node Type\n";
  }
}