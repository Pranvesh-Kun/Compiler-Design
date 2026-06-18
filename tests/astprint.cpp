#include "astprint.h"
#include <iostream>

void indent(int depth) {
  for (int i = 0; i<depth; i++) std::cout << " ";
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
}