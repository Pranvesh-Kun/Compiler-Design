#include "parser.h"

Parser::Parser(std::vector<Token> tks) {
  tokens = tks;
  cur = 0;
};

void Parser::advance() {
  cur++;  
}

Token Parser::peek() {
  return tokens[cur+1];
}

Token Parser::current() {
  return tokens[cur];
}

ProgramNode* Parser::parseProgram() {
  ProgramNode* root = new ProgramNode();
  while (current().type != TokenType::END_OF_FILE) {
    StatementNode* statement = parseStatement();
    root->statements.push_back(statement);
  }
  return root;
}