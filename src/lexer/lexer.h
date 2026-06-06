#pragma once
#include "token.h"
#include <vector>

class Lexer {

  private:
    std::string buffer;
    std::string source;
    int row;
    int col;
    int ind;
    bool isSymbol(char);
    bool isKeyword(std::string&);

  public:
    Lexer(std::string);
    std::vector<Token> tokenize();
};