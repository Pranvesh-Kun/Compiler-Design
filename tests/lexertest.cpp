#include "lexer.h"
#include <fstream>
#include <sstream>
#include <iostream>

int main() {
  std::ifstream file("tests/source.txt");
  std::stringstream buffer;
  buffer << file.rdbuf();
  std::string code = buffer.str();
  // std::string code = "int x = 5";
  Lexer lex = Lexer(code);
  lex.tokenize();
  lex.printtokens();
}