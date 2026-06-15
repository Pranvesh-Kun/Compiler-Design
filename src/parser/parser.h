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
    BinaryExpressionNode* parseBinaryExpression();
    UnaryExpressionNode* parseUnaryExpression();
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
    void advance();
    Token peek();
    Token current();

  public:
    ProgramNode* parseProgram();
    Parser(std::vector<Token> tokens);
};