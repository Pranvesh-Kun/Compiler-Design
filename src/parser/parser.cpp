#include "parser.h"
#include <iostream>

Parser::Parser(std::vector<Token> tks) {
  tokens = tks;
  ind = 0;
};

void Parser::advance() {
  ind++;  
}

Token Parser::peek() {
  if (tokens[ind].type == TokenType::END_OF_FILE) abort();
  return tokens[ind+1];
}

Token Parser::current() {
  return tokens[ind];
}

StatementNode* Parser::parseStatement() {
  Token cur = current();
  StatementNode* statement;
  if (cur.type == TokenType::KW_INT || cur.type == TokenType::KW_FLOAT || cur.type == TokenType::KW_STRING
  || cur.type == TokenType::KW_BOOL || cur.type == TokenType::KW_ARRAY || cur.type == TokenType::KW_LET) {
    // starts with type
    if (cur.type == TokenType::KW_LET) {
      statement = parseVariableDeclaration();
    }
    advance();
    cur = current();
    if (cur.type == TokenType::END_OF_FILE) {
      // handle error
      std::cout << "Parse Error, at line: " << cur.line_num << ", column: " << cur.col_num << ".\nExpected Token: Identifier, Found EOF.\n";
    }
    if (peek().type == TokenType::L_PAREN) {
      ind--;
      statement = parseFunctionDeclaration();
    }    
    else {
      ind--;
      statement = parseVariableDeclaration();
    }
  }
  else if (cur.type ==  TokenType::KW_IF) {
    statement = parseIfStatement();
  }
  else if (cur.type == TokenType::KW_FOR) {
    statement = parseForLoop();
  }
  else if (cur.type == TokenType::KW_WHILE) {
    statement = parseWhileLoop();
  }
  else if (cur.type == TokenType::KW_RETURN) {
    statement = parseReturn();
  }
}

ProgramNode* Parser::parseProgram() {
  ProgramNode* root = new ProgramNode();
  while (current().type != TokenType::END_OF_FILE) {
    StatementNode* statement = parseStatement();
    root->statements.push_back(statement);
  }
  return root;
}