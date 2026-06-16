#pragma once
#include <vector>
#include <string>
#include "token.h"

class ASTNode {
  public:
    virtual ~ASTNode() = default;
};

class ExpressionNode: public ASTNode {
  public:
    virtual ~ExpressionNode() = default;
};

class StatementNode: public ASTNode {
  public:
    virtual ~StatementNode() = default;
};

class ProgramNode: public ASTNode {
  public: 
    std::vector<StatementNode*> statements;
};

class IntLiteralNode: public ExpressionNode {
  public:
    int value;
};

class FloatLiteralNode: public ExpressionNode {
  public:
    double value;
};

class StringLiteralNode: public ExpressionNode {
  public:
    std::string value;
};

class BoolLiteralNode: public ExpressionNode {
  public:
    bool value;
};

class ArrayLiteralNode: public ExpressionNode {
  public:
    std::vector<ExpressionNode*> elements;
};

class IdentifierNode: public ExpressionNode {
  public:
    std::string name;
};

class IndexAccessNode: public ExpressionNode {
  public:
    IdentifierNode* object;
    ExpressionNode* index;
};

class BinaryExpressionNode: public ExpressionNode {
  public:
    TokenType operation;
    ExpressionNode* left;
    ExpressionNode* right;
};

class UnaryExpressionNode: public ExpressionNode {
  public:
    TokenType operation;
    ExpressionNode* operand;
};

class ParameterNode: public ASTNode {
  public:
    TokenType type;
    TokenType subtype;
    IdentifierNode* name;
};

class FunctionCallNode: public ExpressionNode {
  public:
    IdentifierNode* name;
    std::vector<ExpressionNode*> arguments;
};

class VariableDeclarationNode: public  StatementNode {
  public:
    TokenType type;
    TokenType subtype;
    IdentifierNode* name;
    ExpressionNode* initializer;
    ExpressionNode* size;
};

class AssignmentNode: public StatementNode {
  public:
    IdentifierNode* name;
    ExpressionNode* value;
};

class IndexAssignmentNode: public StatementNode {
  public:
    IdentifierNode* object;
    ExpressionNode* index;
    ExpressionNode* value;
};

class ElseIfNode: public StatementNode {
  public:
    ExpressionNode* condition;
    std::vector<StatementNode*> codeblock;  
};

class IfStatementNode: public StatementNode {
  public:
    ExpressionNode* condition;
    std::vector<StatementNode*> codeblock;
    std::vector<ElseIfNode*> elseif;
    std::vector<StatementNode*> elseblock;

};

class WhileLoopNode: public StatementNode {
  public:
    ExpressionNode* condition;
    std::vector<StatementNode*> codeblock;
};

class ForLoopNode: public StatementNode {
  public:
    IdentifierNode* iterator;
    ExpressionNode* iterable;
    std::vector<StatementNode*> codeblock;
};

class FunctionDeclarationNode: public StatementNode {
  public:
    TokenType returntype;
    IdentifierNode* name;
    std::vector<ParameterNode*> parameters;
    std::vector<StatementNode*> body;
};

class ReturnNode: public StatementNode {
  public: 
    ExpressionNode* value;
};

class BreakNode: public StatementNode {
  public:
};

class ContinueNode: public StatementNode {
  public:
};

class ExpressionStatementNode: public StatementNode {
  public:
    ExpressionNode* expression;
};
