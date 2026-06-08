#pragma once
#include "token.h"
#include <vector>

class Lexer {

  private:
    std::string buffer;
    std::string source;
    std::vector<Token> tokens;
    int row;
    int col;
    int ind;
    bool isSymbol(char);
    bool isKeyword();
    bool isIdentifier();
    bool isInteger();
    bool isFloat();
    bool need_break();
    void lexical_error();
    std::string TokenToString(Token);

  public:
    Lexer(std::string);
    std::vector<Token> tokenize();
    void assign_token();
    void printtokens();
};