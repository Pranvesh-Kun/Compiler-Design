#include "parser.h"
#include <iostream>

Parser::Parser(std::vector<Token> tks) {
  tokens = tks;
  ind = 0;
};

void Parser::parser_error(TokenType expected) {
  std::cout << "Parser Error at line " << current().line_num << ", column " << current().col_num << ".\n";
  std::cout << "Expected: " << tokenmap[expected] << ", Found: " << tokenmap[current().type] << "\n";
  abort();
}

void Parser::parser_error_statement() {
  std::cout << "Parser Error at line " << current().line_num << ", column " << current().col_num << ".\n";
  std::cout << "Expected a statement, Found: " << tokenmap[current().type] << "\n";
  abort();
}

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
  if (cur.type == TokenType::KW_INT || cur.type == TokenType::KW_FLOAT || cur.type == TokenType::KW_STRING
  || cur.type == TokenType::KW_BOOL || cur.type == TokenType::KW_ARRAY || cur.type == TokenType::KW_LET) {
    // starts with type
    if (cur.type == TokenType::KW_LET) {
      return parseVariableDeclaration();
    }
    advance();
    cur = current();
    if (cur.type != TokenType::IDENTIFIER) {
      parser_error(TokenType::IDENTIFIER);
    }
    if (peek().type == TokenType::L_PAREN) {
      ind--;
      return parseFunctionDeclaration();
    }    
    else {
      ind--;
      return parseVariableDeclaration();
    }
  }
  else if (cur.type ==  TokenType::KW_IF) {
    return parseIfStatement();
  }
  else if (cur.type == TokenType::KW_FOR) {
    return parseForLoop();
  }
  else if (cur.type == TokenType::KW_WHILE) {
    return parseWhileLoop();
  }
  else if (cur.type == TokenType::KW_RETURN) {
    return parseReturn();
  }
  else if (cur.type == TokenType::IDENTIFIER) {
    if (peek().type == TokenType::L_PAREN) {
      return parseExpressionStatement();
    }
    else if (peek().type == TokenType::L_SQUARE) {
      return parseIndexAssignment();
    }
    else {
      return parseAssignment();
    }
  }
  else {
    parser_error_statement();
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