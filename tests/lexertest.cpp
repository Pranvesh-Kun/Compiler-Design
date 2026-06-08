#include "lexer.h"

int main() {
  std::string code = ".1";
  Lexer lex = Lexer(code);
  lex.tokenize();
  lex.printtokens();
}