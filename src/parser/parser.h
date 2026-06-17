#pragma once
#include "ast.h"

class Parser {
  private:
    std::vector<Token> tokens;
    int ind;
    StatementNode* parseStatement();
    ExpressionNode* parseExpression();
    IdentifierNode* parseIdentifier();
    IntLiteralNode* parseIntLiteral();
    FloatLiteralNode* parseFloatLiteral();
    BoolLiteralNode* parseBoolLiteral();
    StringLiteralNode* parseStringLiteral();
    ExpressionNode* parseBinaryExpression();
    ExpressionNode* parseUnaryExpression();
    FunctionCallNode* parseFunctionCall();
    ArrayLiteralNode* parseArrayLiteral();
    IndexAccessNode* parseIndexAccess();
    VariableDeclarationNode* parseVariableDeclaration();
    AssignmentNode* parseAssignment();
    IndexAssignmentNode* parseIndexAssignment();
    IfStatementNode* parseIfStatement();
    ElseIfNode* parseElseIf();
    WhileLoopNode* parseWhileLoop();
    ForLoopNode* parseForLoop();
    FunctionDeclarationNode* parseFunctionDeclaration();
    ReturnNode* parseReturn();
    ExpressionStatementNode* parseExpressionStatement();
    BreakNode* parseBreak();
    ContinueNode* parseContinue();
    ParameterNode* parseParameter();
    ExpressionNode* parsePrimary();
    IncrementNode* parseIncrement();
    DecrementNode* parseDecrement();
    void advance();
    Token peek();
    Token current();
    bool check(TokenType);
    void parser_error(TokenType expected);
    void parser_error_statement();
    void parser_error_expression();

  public:
    ProgramNode* parseProgram();
    Parser(std::vector<Token> tokens);
};