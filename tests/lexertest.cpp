#include "lexer.h"

int main() {
  std::string code = "a@bc";
  Lexer lex = Lexer(code);
  lex.tokenize();
  lex.printtokens();
}