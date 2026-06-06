#pragma once
#include "token.h"
#include <vector>

class Lexer {

  private:
    std::string buffer;
    std::string source;
    int row = 0;
    int col = 0;
    int ind = 0;
    bool isSymbol(char);
    bool isKeyword();
    bool isNumber(char);

  public:
    Lexer(std::string);
    std::vector<Token> tokenize();
};