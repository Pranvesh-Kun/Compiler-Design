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

IdentifierNode* Parser::parseIdentifier() {
  Token cur = current();
  if (cur.type != TokenType::IDENTIFIER) parser_error(TokenType::IDENTIFIER);
  IdentifierNode* node = new IdentifierNode();
  node->name = cur.text;
  advance();
  return node;
}

VariableDeclarationNode* Parser::parseVariableDeclaration() {
  VariableDeclarationNode* node = new VariableDeclarationNode();
  node->type = current().type;
  node->subtype = current().type;
  if (node->type == TokenType::KW_LET) {
    advance();
    node->name = parseIdentifier();
    if (current().type != TokenType::ASSIGN) parser_error(TokenType::ASSIGN);
    advance();
    node->initializer = parseExpression();
  }
  else if (node->type == TokenType::KW_ARRAY) {
    advance();
    if (current().type != TokenType::L_PAREN) parser_error(TokenType::L_PAREN);
    advance();
    if (current().type != TokenType::KW_INT &&
        current().type != TokenType::KW_FLOAT &&
        current().type != TokenType::KW_BOOL &&
        current().type != TokenType::KW_STRING) {
      std::cout << "Parser Error at line " << current().line_num << ", column " << current().col_num << ".\n";
      std::cout << "Expected a valid array element type " << "(int, float, bool, string), Found: " << tokenmap[current().type] << "\n";
      abort();
    }
    node->subtype = current().type;
    advance();
    if (current().type != TokenType::R_PAREN) parser_error(TokenType::R_PAREN);
    advance();
    node->name = parseIdentifier();
    if (current().type == TokenType::L_SQUARE) {
      advance();
      node->size = parseExpression();
      if (current().type != TokenType::R_SQUARE) parser_error(TokenType::R_SQUARE);
      advance();
    }
    else if (current().type != TokenType::ASSIGN) parser_error(TokenType::ASSIGN);
    if (current().type == TokenType::ASSIGN) {
      advance();
      node->initializer = parseExpression();
    }
  }
  else {
    advance();
    node->name = parseIdentifier();
    if (current().type != TokenType::ASSIGN) {
      if (node->type == TokenType::KW_INT) {
        IntLiteralNode* temp = new IntLiteralNode();
        temp->value = 0;  
        node->initializer = temp;
      }
      if (node->type == TokenType::KW_FLOAT) {
        FloatLiteralNode* temp = new FloatLiteralNode();
        temp->value = 0.0;  
        node->initializer = temp;
      }
      if (node->type == TokenType::KW_STRING) {
        StringLiteralNode* temp = new StringLiteralNode();
        temp->value = "";  
        node->initializer = temp;
      }
      if (node->type == TokenType::KW_BOOL) {
        BoolLiteralNode* temp = new BoolLiteralNode();
        temp->value = false;  
        node->initializer = temp;
      }
      return node;
    }
    advance();
    node->initializer = parseExpression();
  }
  return node;
}

AssignmentNode* Parser::parseAssignment() {
    AssignmentNode* node = new AssignmentNode();
    node->name = parseIdentifier();
    if (current().type != TokenType::ASSIGN) parser_error(TokenType::ASSIGN);
    advance();
    node->value = parseExpression();
    return node;
}

IndexAssignmentNode* Parser::parseIndexAssignment() {
  IndexAssignmentNode* node = new IndexAssignmentNode();
  node->object = parseIdentifier();
  advance();
  node->index = parseExpression();
  if (current().type != TokenType::R_SQUARE) parser_error(TokenType::R_SQUARE);
  advance();
  if (current().type != TokenType::ASSIGN) parser_error(TokenType::ASSIGN);
  advance();
  node->value = parseExpression();
  return node;
}

ExpressionStatementNode* Parser::parseExpressionStatement() {
  ExpressionStatementNode* node = new ExpressionStatementNode();
  node->expression = parseExpression();
  return node;
}

ReturnNode* Parser::parseReturn() {
  advance();
  ReturnNode* node = new ReturnNode();
  node->value = parseExpression();
  return node;
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