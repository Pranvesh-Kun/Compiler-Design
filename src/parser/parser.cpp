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

void Parser::parser_error_expression() {
  std::cout << "Parser Error at line " << current().line_num << ", column " << current().col_num << ".\n";
  std::cout << "Expected an expression, Found: " << tokenmap[current().type] << "\n";
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

bool Parser::check(TokenType type) {
  return (current().type == type);
}

IdentifierNode* Parser::parseIdentifier() {
  Token cur = current();
  if (cur.type != TokenType::IDENTIFIER) parser_error(TokenType::IDENTIFIER);
  IdentifierNode* node = new IdentifierNode();
  node->name = cur.text;
  advance();
  return node;
}

IntLiteralNode* Parser::parseIntLiteral() {
  IntLiteralNode* node = new IntLiteralNode();
  node->value = std::stoi(current().text);
  advance();
  return node;
}

FloatLiteralNode* Parser::parseFloatLiteral() {
  FloatLiteralNode* node = new FloatLiteralNode();
  node->value = std::stof(current().text);
  advance();
  return node;
}

BoolLiteralNode* Parser::parseBoolLiteral() {
  BoolLiteralNode* node = new BoolLiteralNode();
  if (check(TokenType::BOOL_LITERAL_FALSE)) node->value = false;
  else node->value = true;
  advance();
  return node;
}

StringLiteralNode* Parser::parseStringLiteral() {
  StringLiteralNode* node = new StringLiteralNode();
  node->value = current().text;
  advance();
  return node;
}

FunctionCallNode* Parser::parseFunctionCall() {
  FunctionCallNode* node = new FunctionCallNode();
  node->name = parseIdentifier();
  if (!check(TokenType::L_PAREN)) parser_error(TokenType::L_PAREN);
  advance();
  while (!check(TokenType::R_PAREN)) {
    if (check(TokenType::END_OF_FILE)) parser_error(TokenType::R_PAREN);
    node->arguments.push_back(parseExpression());
    if (check(TokenType::COMMA)) {
      if (peek().type == TokenType::R_PAREN) parser_error(TokenType::R_PAREN);
      advance();
    }
    else if (!check(TokenType::R_PAREN)) parser_error(TokenType::COMMA);
  }
  advance();
  return node;
}

IndexAccessNode* Parser::parseIndexAccess() {
  IndexAccessNode* node = new IndexAccessNode();
  node->object = parseIdentifier();
  if (!check(TokenType::L_SQUARE)) parser_error(TokenType::L_SQUARE);
  advance();
  node->index = parseExpression();
  if (!check(TokenType::R_SQUARE)) parser_error(TokenType::R_SQUARE);
  advance();
  return node;
}

ArrayLiteralNode* Parser::parseArrayLiteral() {
  ArrayLiteralNode* node = new ArrayLiteralNode();
  advance();
  while (!check(TokenType::R_SQUARE)) {
    if (check(TokenType::END_OF_FILE)) parser_error(TokenType::END_OF_FILE);
    node->elements.push_back(parseExpression());
    if (check(TokenType::COMMA)) {
      if (peek().type == TokenType::R_SQUARE) parser_error(TokenType::R_SQUARE);
      advance();
    }
    else if (!check(TokenType::R_SQUARE)) parser_error(TokenType::COMMA);
  }
  advance();
  return node;
}

ExpressionNode* Parser::parsePrimary() {
  if (check(TokenType::INT_LITERAL)) return parseIntLiteral();
  if (check(TokenType::FLOAT_LITERAL)) return parseFloatLiteral();
  if (check(TokenType::BOOL_LITERAL_FALSE) || check(TokenType::BOOL_LITERAL_TRUE)) return parseBoolLiteral();
  if (check(TokenType::STR_LITERAL)) return parseStringLiteral();
  if (check(TokenType::IDENTIFIER)) {
    if (peek().type == TokenType::L_PAREN) return parseFunctionCall();
    if (peek().type == TokenType::L_SQUARE) return parseIndexAccess();
    return parseIdentifier();
  }
  if (check(TokenType::L_PAREN)) {
    advance();
    ExpressionNode* node = parseExpression();
    if (!check(TokenType::R_PAREN)) parser_error(TokenType::R_PAREN);
    advance();
    return node;
  }
  if (check(TokenType::L_SQUARE)) return parseArrayLiteral();
  parser_error_expression();
}

ExpressionNode* Parser::parseUnaryExpression() {
  if (check(TokenType::KW_NOT)) {
    UnaryExpressionNode* node = new UnaryExpressionNode();
    node->operation = TokenType::KW_NOT;
    advance();
    node->operand = parseUnaryExpression();
    return node;
  }
  return parsePrimary();
}

IncrementNode* Parser::parseIncrement() {
  IncrementNode* node = new IncrementNode();
  node->name = parseIdentifier();
  advance();
  return node;
}

DecrementNode* Parser::parseDecrement() {
  DecrementNode* node = new DecrementNode();
  node->name = parseIdentifier();
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

WhileLoopNode* Parser::parseWhileLoop() {
  WhileLoopNode* node = new WhileLoopNode();
  advance();
  if (!check(TokenType::L_PAREN)) parser_error(TokenType::L_PAREN);
  advance();
  node->condition = parseExpression();
  if (!check(TokenType::R_PAREN)) parser_error(TokenType::R_PAREN);
  advance();
  if (!check(TokenType::L_CURLY)) parser_error(TokenType::L_CURLY);
  advance();
  while (!check(TokenType::R_CURLY)) {
    if (check(TokenType::END_OF_FILE)) parser_error(TokenType::R_CURLY);
    node->codeblock.push_back(parseStatement());
  }
  advance();
  return node;
}

ReturnNode* Parser::parseReturn() {
  advance();
  ReturnNode* node = new ReturnNode();
  node->value = parseExpression();
  return node;
}

BreakNode* Parser::parseBreak() {
  BreakNode* node = new BreakNode();
  advance();
  return node;
}

ContinueNode* Parser::parseContinue() {
  ContinueNode* node = new ContinueNode();
  advance();
  return node;
}

ElseIfNode* Parser::parseElseIf() {
  ElseIfNode* node = new ElseIfNode();
  advance();
  if (current().type != TokenType::L_PAREN) parser_error(TokenType::L_PAREN);
  advance();
  node->condition = parseExpression();
  if (current().type != TokenType::R_PAREN) parser_error(TokenType::R_PAREN);
  advance();
  if (current().type != TokenType::L_CURLY) parser_error(TokenType::L_CURLY);
  advance();
  while (!check(TokenType::R_CURLY)) {
    if (check(TokenType::END_OF_FILE)) parser_error(TokenType::R_CURLY);
    node->codeblock.push_back(parseStatement());
  }
  advance();
  return node;
}

IfStatementNode* Parser::parseIfStatement() {
  IfStatementNode* node = new IfStatementNode();
  advance();
  if (!check(TokenType::L_PAREN)) parser_error(TokenType::L_PAREN);
  advance();
  node->condition = parseExpression();
  if (!check(TokenType::R_PAREN)) parser_error(TokenType::R_PAREN);
  advance();
  if (!check(TokenType::L_CURLY)) parser_error(TokenType::L_CURLY);
  advance();
  while (!check(TokenType::R_CURLY)) {
    if (check(TokenType::END_OF_FILE)) parser_error(TokenType::R_CURLY);
    node->codeblock.push_back(parseStatement());
  } 
  advance();
  while (check(TokenType::KW_ELSE)) {
    advance();
    if (check(TokenType::KW_IF)) node->elseif.push_back(parseElseIf());
    else {
      if (!check(TokenType::L_CURLY)) parser_error(TokenType::L_CURLY);
      advance();
      while (!check(TokenType::R_CURLY)) {
        if (check(TokenType::END_OF_FILE)) parser_error(TokenType::R_CURLY);
        node->elseblock.push_back(parseStatement());
      }
      advance();
      break;
    }
  }
  return node;
}

ForLoopNode* Parser::parseForLoop() {
  ForLoopNode* node = new ForLoopNode();
  advance();
  if (!check(TokenType::L_PAREN)) parser_error(TokenType::L_PAREN);
  advance();
  node->iterator = parseIdentifier();
  if (!check(TokenType::KW_IN)) parser_error(TokenType::KW_IN);
  advance();
  node->iterable = parseExpression();
  if (!check(TokenType::R_PAREN)) parser_error(TokenType::R_PAREN);
  advance();
  if (!check(TokenType::L_CURLY)) parser_error(TokenType::L_CURLY);
  advance();
  while (!check(TokenType::R_CURLY)) {
    if (check(TokenType::END_OF_FILE)) parser_error(TokenType::R_CURLY);
    node->codeblock.push_back(parseStatement());
  }
  advance();
  return node;
}

ParameterNode* Parser::parseParameter() {
  ParameterNode* node = new ParameterNode();
  if (!check(TokenType::KW_INT) && !check(TokenType::KW_FLOAT) && !check(TokenType::KW_BOOL) 
  && !check(TokenType::KW_ARRAY) && !check(TokenType::KW_STRING)) {
    std::cout << "Parser Error at line " << current().line_num << ", column " << current().col_num << ".\n";
    std::cout << "Expected a parameter, Found: " << tokenmap[current().type] << "\n";
    abort();   
  } 
  node->type = current().type;
  node->subtype = current().type;
  if (node->type == TokenType::KW_ARRAY) {
    advance();
    if (!check(TokenType::L_PAREN)) parser_error(TokenType::L_PAREN);
    advance();
    node->subtype = current().type;
    if (!check(TokenType::KW_INT) && !check(TokenType::KW_FLOAT) 
        && !check(TokenType::KW_BOOL) && !check(TokenType::KW_STRING)) {
      std::cout << "Parser Error at line " << current().line_num << ", column " << current().col_num << ".\n";
      std::cout << "Expected a valid array element type " << "(int, float, bool, string), Found: " << tokenmap[current().type] << "\n";
      abort();
    }
    advance();
    if (!check(TokenType::R_PAREN)) parser_error(TokenType::R_PAREN);
  }
  advance();
  node->name = parseIdentifier();
  return node;
}

FunctionDeclarationNode* Parser::parseFunctionDeclaration() {
  FunctionDeclarationNode* node = new FunctionDeclarationNode();
  node->returntype = current().type;
  advance();
  node->name = parseIdentifier();
  if (!check(TokenType::L_PAREN)) parser_error(TokenType::L_PAREN);
  advance();
  while (!check(TokenType::R_PAREN)) {
    if (check(TokenType::END_OF_FILE)) parser_error(TokenType::R_PAREN);
    node->parameters.push_back(parseParameter());
    if (check(TokenType::COMMA)) {
      if (peek().type == TokenType::R_PAREN) parser_error(TokenType::R_PAREN);
      advance();
    }
    else if (!check(TokenType::R_PAREN)) parser_error(TokenType::COMMA);
  }
  advance();
  if (!check(TokenType::L_CURLY)) parser_error(TokenType::L_CURLY);
  advance();
  while (!check(TokenType::R_CURLY)) {
    if (check(TokenType::END_OF_FILE)) parser_error(TokenType::R_CURLY);
    node->body.push_back(parseStatement());
  }
  advance();
  return node;
}

StatementNode* Parser::parseStatement() {
  Token cur = current();
  if (cur.type == TokenType::KW_INT || cur.type == TokenType::KW_FLOAT || cur.type == TokenType::KW_STRING
  || cur.type == TokenType::KW_BOOL || cur.type == TokenType::KW_ARRAY || cur.type == TokenType::KW_LET || cur.type == TokenType::KW_VOID) {
    // starts with type
    if (cur.type == TokenType::KW_LET) {
      return parseVariableDeclaration();
    }
    if (cur.type == TokenType::KW_VOID) {
      return parseFunctionDeclaration();
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
  else if (check(TokenType::KW_BREAK)) {
    return parseBreak();
  }
  else if (check(TokenType::KW_CONTINUE)) {
    return parseContinue();
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
    else if (peek().type == TokenType::INCREMENT) {
      return parseIncrement();
    }
    else if (peek().type == TokenType::DECREMENT) {
      return parseDecrement();
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