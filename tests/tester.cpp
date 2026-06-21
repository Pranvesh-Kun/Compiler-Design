#include "../src/parser/parser.h"
#include "../src/lexer/lexer.h"
#include "../src/interpreter/interpreter.h"
#include "astprint.h"
#include <fstream>
#include <sstream>
#include <iostream>

int main() {
  std::ifstream file("tests/source.txt");
  std::stringstream buffer;
  buffer << file.rdbuf();
  std::string code = buffer.str();
  Lexer lex = Lexer(code);
  std::vector<Token> tokens = lex.tokenize();
  lex.printtokens();
  Parser parser(tokens); 
  ProgramNode* root = parser.parseProgram();
  printAST(root, 0);
  Interpreter interpreter;
  interpreter.execute(root);
}