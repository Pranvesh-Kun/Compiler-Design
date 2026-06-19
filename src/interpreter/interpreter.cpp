#include "interpreter.h"
#include <iostream>

void Interpreter::interpreter_error(std::string msg) {
  std::cerr << "Interpreter Error: " << msg << "\n";
  exit(1);
}

Value Interpreter::evaluate(ExpressionNode* node) {
  if (auto n = dynamic_cast<IntLiteralNode*>(node)) {
    Value val;
    val.intval = n->value;
    val.type = ValueType::INT;
    return val;
  }
  else if (auto n = dynamic_cast<FloatLiteralNode*>(node)) {
    Value val;
    val.floatval = n->value;
    val.type = ValueType::FLOAT;
    return val;
  }
  else if (auto n = dynamic_cast<StringLiteralNode*>(node)) {
    Value val;
    val.stringval = n->value;
    val.type = ValueType::STRING;
    return val;
  }
  else if (auto n = dynamic_cast<BoolLiteralNode*>(node)) {
    Value val;
    val.boolval = n->value;
    val.type = ValueType::BOOL;
    return val;
  }
  else if (auto n = dynamic_cast<ArrayLiteralNode*>(node)) {
    Value val;
    val.type = ValueType::ARRAY;
    for (auto it: n->elements) val.arrayval.push_back(evaluate(it));
    return val;
  }
  else if (auto n = dynamic_cast<BinaryExpressionNode*>(node)) {
    Value left = evaluate(n->left);
    Value right = evaluate(n->right);
    if (left.type != right.type) interpreter_error("Operands must have the same type.");
    Value val;
    if (n->operation == TokenType::PLUS || n->operation == TokenType::MINUS || 
      n->operation == TokenType::ASTERISK || n->operation == TokenType::SLASH || n->operation == TokenType::MOD) {
      if (left.type == ValueType::BOOL || left.type == ValueType::STRING || left.type == ValueType::ARRAY) interpreter_error("Arithmetic operators require int or float operands.");
      val.type = left.type;
      if (n->operation == TokenType::PLUS) {
        if (val.type == ValueType::INT) val.intval = left.intval + right.intval;
        else val.floatval = left.floatval + right.floatval;
      } 
      if (n->operation == TokenType::MINUS) {
        if (val.type == ValueType::INT) val.intval = left.intval - right.intval;
        else val.floatval = left.floatval - right.floatval;
      } 
      if (n->operation == TokenType::ASTERISK) {
        if (val.type == ValueType::INT) val.intval = left.intval * right.intval;
        else val.floatval = left.floatval * right.floatval;
      } 
      if (n->operation == TokenType::SLASH) {
        if (val.type == ValueType::INT && right.intval == 0) interpreter_error("Division by zero.");
        if (val.type == ValueType::FLOAT && right.floatval == 0.0f) interpreter_error("Division by zero.");
        if (val.type == ValueType::INT) val.intval = left.intval / right.intval;
        else val.floatval = left.floatval / right.floatval;
      } 
      if (n->operation == TokenType::MOD) {
        if (val.type == ValueType::INT && right.intval == 0) interpreter_error("Modulo by zero.");
        if (val.type == ValueType::INT) val.intval = left.intval % right.intval;
        else interpreter_error("Modulo operator only supports integers");
      } 
    }
    else if (n->operation == TokenType::GRT_EQUALS || n->operation == TokenType::GRT_THAN ||
      n->operation == TokenType::LESS_EQUALS || n->operation == TokenType::LESS_THAN) {
      if (left.type == ValueType::STRING || left.type == ValueType::ARRAY || left.type == ValueType::BOOL) interpreter_error("Comparison operators only support int and float values");
      val.type = ValueType::BOOL;
      if (n->operation == TokenType::GRT_EQUALS) {
        if (left.type == ValueType::INT) val.boolval = (left.intval >= right.intval);
        else if (left.type == ValueType::FLOAT) val.boolval = (left.floatval >= right.floatval);
      }
      if (n->operation == TokenType::GRT_THAN) {
        if (left.type == ValueType::INT) val.boolval = (left.intval > right.intval);
        else if (left.type == ValueType::FLOAT) val.boolval = (left.floatval > right.floatval);
      }
      if (n->operation == TokenType::LESS_EQUALS) {
        if (left.type == ValueType::INT) val.boolval = (left.intval <= right.intval);
        else if (left.type == ValueType::FLOAT) val.boolval = (left.floatval <= right.floatval);
      }
      if (n->operation == TokenType::LESS_THAN) {
        if (left.type == ValueType::INT) val.boolval = (left.intval < right.intval);
        else if (left.type == ValueType::FLOAT) val.boolval = (left.floatval < right.floatval);
      }
    }
    else if (n->operation == TokenType::EQUALS) {
      if (left.type == ValueType::ARRAY) interpreter_error("Equals operators does not support array values");
      val.type = ValueType::BOOL;
      if (left.type == ValueType::INT) val.boolval = (left.intval == right.intval);
      else if (left.type == ValueType::BOOL) val.boolval = (left.boolval == right.boolval);
      else if (left.type == ValueType::STRING) val.boolval = (left.stringval == right.stringval);
      else val.boolval = (left.floatval == right.floatval);
    }
    else if (n->operation == TokenType::NOT_EQUALS) {
      if (left.type == ValueType::ARRAY) interpreter_error("Not Equals operators does not support array values");
      val.type = ValueType::BOOL;
      if (left.type == ValueType::INT) val.boolval = (left.intval != right.intval);
      else if (left.type == ValueType::BOOL) val.boolval = (left.boolval != right.boolval);
      else if (left.type == ValueType::STRING) val.boolval = (left.stringval != right.stringval);
      else val.boolval = (left.floatval != right.floatval);
    }
    else if (n->operation == TokenType::KW_AND) {
      if (left.type != ValueType::BOOL) interpreter_error("Logical operators require boolean operands");
      val.type = ValueType::BOOL;
      val.boolval = (left.boolval && right.boolval);
    }
    else if (n->operation == TokenType::KW_OR) {
      if (left.type != ValueType::BOOL) interpreter_error("Logical operators require boolean operands");
      val.type = ValueType::BOOL;
      val.boolval = (left.boolval || right.boolval);
    }
    return val;
  }  
  else if (auto n = dynamic_cast<UnaryExpressionNode*>(node)) {
    Value val;
    val.type = ValueType::BOOL;
    Value operand = evaluate(n->operand);
    if (operand.type != ValueType::BOOL) interpreter_error("'not' operator requires a boolean operand");
    val.boolval = !(operand.boolval);
    return val;
  }
  interpreter_error("Unknown expression node");
}

void Interpreter::execute(StatementNode* node) {

}