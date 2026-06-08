#include "lexer.h"

int main() {
  std::string code = "#";
  Lexer lex = Lexer(code);
  lex.tokenize();
  lex.printtokens();
}