#include "interpreter.h"
#include <iostream>
#include <string>
#include <set>

void Interpreter::interpreter_error(std::string msg, ASTNode* n) {
  std::cerr << "Interpreter Error at line " << n->row << ", column " << n->col << ".\n";
  std::cerr << "Error: " << msg << "\n";
  exit(1);
}

std::unordered_map<std::string, Value>::iterator Interpreter::lookupVariable(std::string name) {
  for (int i = (int)scopes.size()-1; i>=0; i--) {
    auto var = scopes[i].find(name);
    if (var != scopes[i].end()) return var;
  }
  return scopes.back().end();
}

void Interpreter::assignVariable(IdentifierNode* node, Value val) {
  for (int i = scopes.size()-1; i>=0; i--) {
    if (scopes[i].find(node->name) != scopes[i].end()) {
      scopes[i][node->name] = val;
      return;
    }
  }
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
    if (val.arrayval.empty()) {
      val.size = 0;
      return val;
    }
    ValueType t = val.arrayval[0].type;
    for (auto it: val.arrayval) {
      if (it.type != t) interpreter_error("Array elements must be of the same type.", n);
    }
    val.subtype = t;
    val.size = n->elements.size();
    return val;
  }
  else if (auto n = dynamic_cast<BinaryExpressionNode*>(node)) {
    Value left = evaluate(n->left);
    Value right = evaluate(n->right);
    if (left.type != right.type) interpreter_error("Operands must have the same type.", n);
    Value val;
    if (n->operation == TokenType::PLUS || n->operation == TokenType::MINUS || 
      n->operation == TokenType::ASTERISK || n->operation == TokenType::SLASH || n->operation == TokenType::MOD) {
      if (left.type == ValueType::BOOL || left.type == ValueType::STRING || left.type == ValueType::ARRAY) interpreter_error("Arithmetic operators require int or float operands.", n);
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
        if (val.type == ValueType::INT && right.intval == 0) interpreter_error("Division by zero.", n);
        if (val.type == ValueType::FLOAT && right.floatval == 0.0f) interpreter_error("Division by zero.", n);
        if (val.type == ValueType::INT) val.intval = left.intval / right.intval;
        else val.floatval = left.floatval / right.floatval;
      } 
      if (n->operation == TokenType::MOD) {
        if (val.type == ValueType::INT && right.intval == 0) interpreter_error("Modulo by zero.", n);
        if (val.type == ValueType::INT) val.intval = left.intval % right.intval;
        else interpreter_error("Modulo operator only supports integers", n);
      } 
    }
    else if (n->operation == TokenType::GRT_EQUALS || n->operation == TokenType::GRT_THAN ||
      n->operation == TokenType::LESS_EQUALS || n->operation == TokenType::LESS_THAN) {
      if (left.type == ValueType::STRING || left.type == ValueType::ARRAY || left.type == ValueType::BOOL) interpreter_error("Comparison operators only support int and float values", n);
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
      if (left.type == ValueType::ARRAY) interpreter_error("Equals operators does not support array values", n);
      val.type = ValueType::BOOL;
      if (left.type == ValueType::INT) val.boolval = (left.intval == right.intval);
      else if (left.type == ValueType::BOOL) val.boolval = (left.boolval == right.boolval);
      else if (left.type == ValueType::STRING) val.boolval = (left.stringval == right.stringval);
      else val.boolval = (left.floatval == right.floatval);
    }
    else if (n->operation == TokenType::NOT_EQUALS) {
      if (left.type == ValueType::ARRAY) interpreter_error("Not Equals operators does not support array values", n);
      val.type = ValueType::BOOL;
      if (left.type == ValueType::INT) val.boolval = (left.intval != right.intval);
      else if (left.type == ValueType::BOOL) val.boolval = (left.boolval != right.boolval);
      else if (left.type == ValueType::STRING) val.boolval = (left.stringval != right.stringval);
      else val.boolval = (left.floatval != right.floatval);
    }
    else if (n->operation == TokenType::KW_AND) {
      if (left.type != ValueType::BOOL) interpreter_error("Logical operators require boolean operands", n);
      val.type = ValueType::BOOL;
      val.boolval = (left.boolval && right.boolval);
    }
    else if (n->operation == TokenType::KW_OR) {
      if (left.type != ValueType::BOOL) interpreter_error("Logical operators require boolean operands", n);
      val.type = ValueType::BOOL;
      val.boolval = (left.boolval || right.boolval);
    }
    return val;
  }  
  else if (auto n = dynamic_cast<UnaryExpressionNode*>(node)) {
    Value val;
    val.type = ValueType::BOOL;
    Value operand = evaluate(n->operand);
    if (n->operation == TokenType::KW_NOT) {
      if (operand.type != ValueType::BOOL) interpreter_error("'not' operator requires a boolean operand", n);
      val.boolval = !(operand.boolval);
      return val;
    }
    else {
      if (operand.type != ValueType::INT && operand.type != ValueType::FLOAT) interpreter_error("Unary minus requires integer or float operand.", n);
      if (operand.type == ValueType::INT) {
        val.intval = -operand.intval;
        val.type = ValueType::INT;
      }
      else {
        val.floatval = -operand.floatval;
        val.type = ValueType::FLOAT;        
      }
      return val;
    }
  }
  else if (auto n = dynamic_cast<IdentifierNode*>(node)) {
    auto it = lookupVariable(n->name);
    if (it == scopes.back().end()) interpreter_error("Identifier \"" + n->name + "\" is undefined.", n);
    return it->second;
  }
  else if (auto n = dynamic_cast<IndexAccessNode*>(node)) {
    auto it = lookupVariable(n->object->name);
    if (it == scopes.back().end()) interpreter_error("Identifier \"" + n->object->name + "\" is undefined.", n);
    Value ind = evaluate(n->index);
    if (ind.type != ValueType::INT) interpreter_error("Array index value must be Integer.", n);
    if (it->second.type != ValueType::ARRAY) interpreter_error("Identifier \"" + n->object->name + "\" must be Array.", n);
    if (ind.intval >= it->second.size || ind.intval < 0) interpreter_error("Array index out of bounds.", n);
    return it->second.arrayval[ind.intval];
  }
  else if (auto n = dynamic_cast<FunctionCallNode*>(node)) {
    if (n->name->name == "input") {
      if (n->arguments.size() > 1) interpreter_error("Too many arguments, expected 1.", n);
      if (n->arguments.size() == 0) interpreter_error("Too less arguments, expected 1.", n);
      if (dynamic_cast<IdentifierNode*>(n->arguments[0]) == nullptr) interpreter_error("input() expects a variable.", n);
      auto id = dynamic_cast<IdentifierNode*>(n->arguments[0]);
      if (lookupVariable(id->name) == scopes.back().end()) interpreter_error("Identifier \"" + id->name + "\" is undefined.", n);
      auto it = lookupVariable(id->name);
      if (it->second.type == ValueType::INT) std::cin >> it->second.intval;
      if (it->second.type == ValueType::FLOAT) std::cin >> it->second.floatval;
      if (it->second.type == ValueType::STRING) std::cin >> it->second.stringval;
      if (it->second.type == ValueType::BOOL) std::cin >> it->second.boolval; 
      Value temp;
      temp.type = ValueType::VOID;
      return temp;
    }
    else if (n->name->name == "output") {
      if (n->arguments.size() > 1) interpreter_error("Too many arguments, expected 1.", n);
      if (n->arguments.size() == 0) interpreter_error("Too less arguments, expected 1.", n);
      Value val = evaluate(n->arguments[0]);
      if (val.type == ValueType::INT) std::cout << val.intval;
      if (val.type == ValueType::BOOL) std::cout << std::boolalpha << val.boolval;
      if (val.type == ValueType::STRING) std::cout << val.stringval;
      if (val.type == ValueType::FLOAT) std::cout << val.floatval;
      Value temp;
      temp.type = ValueType::VOID;
      return temp;
    }
    auto func = functions.find(n->name->name);
    if (func == functions.end()) interpreter_error("Function '" + n->name->name + "' is undeclared.", n);
    FunctionDeclarationNode* f = func->second;
    if ((int)f->parameters.size() != n->arguments.size()) interpreter_error("Expected " + std::to_string((int)f->parameters.size()) + " arguments, found " + std::to_string((int)n->arguments.size()) + ".", n);
    for (int i = 0; i<(int)f->parameters.size(); i++) {
      Value arg = evaluate(n->arguments[i]);
      if (f->parameters[i]->type == TokenType::KW_INT && arg.type != ValueType::INT) interpreter_error("Expected parameter type integer.", n);
      if (f->parameters[i]->type == TokenType::KW_FLOAT && arg.type != ValueType::FLOAT) interpreter_error("Expected parameter type float.", n);
      if (f->parameters[i]->type == TokenType::KW_BOOL && arg.type != ValueType::BOOL) interpreter_error("Expected parameter type boolean.", n);
      if (f->parameters[i]->type == TokenType::KW_ARRAY && arg.type != ValueType::ARRAY) interpreter_error("Expected parameter type array.", n);
      if (f->parameters[i]->type == TokenType::KW_STRING && arg.type != ValueType::STRING) interpreter_error("Expected parameter type string.", n);
      if (f->parameters[i]->type == TokenType::KW_ARRAY && arg.size > 0) {
        if (f->parameters[i]->subtype == TokenType::KW_INT && arg.arrayval[0].type != ValueType::INT) interpreter_error("Expected parameter array of integer elements.", n);
        if (f->parameters[i]->subtype == TokenType::KW_FLOAT && arg.arrayval[0].type != ValueType::FLOAT) interpreter_error("Expected parameter array of float elements.", n);
        if (f->parameters[i]->subtype == TokenType::KW_BOOL && arg.arrayval[0].type != ValueType::BOOL) interpreter_error("Expected parameter array of boolean elements.", n);
        if (f->parameters[i]->subtype == TokenType::KW_ARRAY && arg.arrayval[0].type != ValueType::ARRAY) interpreter_error("Expected parameter array of array elements.", n);
        if (f->parameters[i]->subtype == TokenType::KW_STRING && arg.arrayval[0].type != ValueType::STRING) interpreter_error("Expected parameter array of string elements.", n);
      }
    }
    Scope guard(scopes);
    for (int i = 0; i<(int)f->parameters.size(); i++) {
      scopes.back()[f->parameters[i]->name->name] = evaluate(n->arguments[i]);
    }
    try {
      for (auto stmt: f->body) execute(stmt);
    }
    catch (ReturnException& r) {
      if (r.value.type != ValueType::INT && f->returntype == TokenType::KW_INT) interpreter_error("Function must return integer.", n);
      if (r.value.type != ValueType::FLOAT && f->returntype == TokenType::KW_FLOAT) interpreter_error("Function must return float.", n);
      if (r.value.type != ValueType::STRING && f->returntype == TokenType::KW_STRING) interpreter_error("Function must return string.", n);
      if (r.value.type != ValueType::BOOL && f->returntype == TokenType::KW_BOOL) interpreter_error("Function must return boolean.", n);
      if (r.value.type != ValueType::ARRAY && f->returntype == TokenType::KW_ARRAY) interpreter_error("Function must return array.", n);
      if (r.value.type == ValueType::ARRAY) {
        if (r.value.subtype != ValueType::INT && f->returnsubtype == TokenType::KW_INT) interpreter_error("Function must return integer array.", n);
        if (r.value.subtype != ValueType::FLOAT && f->returnsubtype == TokenType::KW_FLOAT) interpreter_error("Function must return float array.", n);
        if (r.value.subtype != ValueType::STRING && f->returnsubtype == TokenType::KW_STRING) interpreter_error("Function must return string array.", n);
        if (r.value.subtype != ValueType::BOOL && f->returnsubtype == TokenType::KW_BOOL) interpreter_error("Function must return boolean array.", n);
      }
      return r.value;
    }
    interpreter_error("Function without return is not allowed.", n);
    Value temp;
    temp.type = ValueType::VOID;
    return temp;
  }
  return Value();
}

void Interpreter::execute(StatementNode* node) {
  if (auto n = dynamic_cast<VariableDeclarationNode*>(node)) {
    if (scopes.back().find(n->name->name) != scopes.back().end()) interpreter_error("Variable " + n->name->name + " already declared.", n);
    Value val;
    if (n->initializer != nullptr) val = evaluate(n->initializer);
    else {
      if (n->type == TokenType::KW_INT) {
        val.intval = 0;
        val.type = ValueType::INT;
      }
      if (n->type == TokenType::KW_FLOAT) {
        val.floatval = 0.0;
        val.type = ValueType::FLOAT;
      }
      if (n->type == TokenType::KW_BOOL) {
        val.boolval = false;
        val.type = ValueType::BOOL;
      }
      if (n->type == TokenType::KW_ARRAY) {
        val.type = ValueType::ARRAY;
        if (evaluate(n->size).type != ValueType::INT) interpreter_error("Array size should be int", n);
        if (evaluate(n->size).intval <= 0) interpreter_error("Array size must be strictly positive.", n);
        val.size = evaluate(n->size).intval;
        val.arrayval = std::vector<Value>(val.size);
        if (n->subtype == TokenType::KW_INT) {
          for (int i = 0; i<val.size; i++) val.arrayval[i].intval = 0, val.arrayval[i].type = ValueType::INT;
        }
        if (n->subtype == TokenType::KW_FLOAT) {
          for (int i = 0; i<val.size; i++) val.arrayval[i].floatval = 0.0, val.arrayval[i].type = ValueType::FLOAT;
        }
        if (n->subtype == TokenType::KW_STRING) {
          for (int i = 0; i<val.size; i++) val.arrayval[i].stringval = "", val.arrayval[i].type = ValueType::STRING;
        }
        if (n->subtype == TokenType::KW_BOOL) {
          for (int i = 0; i<val.size; i++) val.arrayval[i].boolval = false, val.arrayval[i].type = ValueType::BOOL;
        }
      }     
      if (n->type == TokenType::KW_STRING) {
        val.stringval = "";
        val.type = ValueType::STRING;
      }
    }
    if (val.type == ValueType::INT && n->type != TokenType::KW_INT) interpreter_error("Initializer must be of type integer.", n);
    if (val.type == ValueType::FLOAT && n->type != TokenType::KW_FLOAT) interpreter_error("Initializer must be of type float.", n);
    if (val.type == ValueType::BOOL && n->type != TokenType::KW_BOOL) interpreter_error("Initializer must be of type boolean.", n);
    if (val.type == ValueType::STRING && n->type != TokenType::KW_STRING) interpreter_error("Initializer must be of type string.", n);
    if (val.type == ValueType::ARRAY) {
      Value sz = evaluate(n->size);
      if (sz.type != ValueType::INT) interpreter_error("Array size should be int", n);
      if (sz.intval <= 0) interpreter_error("Array size must be strictly positive.", n);
      if (n->type != TokenType::KW_ARRAY) interpreter_error("Initializer must be an array.", n);
      val.size = sz.intval;
      for (auto x: val.arrayval) {
        if (x.type == ValueType::INT && n->subtype != TokenType::KW_INT) interpreter_error("Array elements must be of type integer.", n);
        if (x.type == ValueType::FLOAT && n->subtype != TokenType::KW_FLOAT) interpreter_error("Array elements must be of type float.", n);
        if (x.type == ValueType::BOOL && n->subtype != TokenType::KW_BOOL) interpreter_error("Array elements must be of type boolean.", n);
        if (x.type == ValueType::STRING && n->subtype != TokenType::KW_STRING) interpreter_error("Array elements must be of type string.", n);
      }
    }
    scopes.back()[n->name->name] = val;
  }
  else if (auto n = dynamic_cast<AssignmentNode*>(node)) {
    if (lookupVariable(n->name->name) == scopes.back().end()) interpreter_error("Variable " + n->name->name + " undeclared.", n);
    Value val = evaluate(n->value);
    Value temp = lookupVariable(n->name->name)->second;
    if (val.type != temp.type) interpreter_error("Assigned value type does not match variable type.", n);
    if (val.type == ValueType::ARRAY) {
      if (val.size > lookupVariable(n->name->name)->second.size) interpreter_error("Assigned array exceeds fixed array size.", n);
      if (val.subtype != temp.subtype) interpreter_error("Assigned array subtype is different.", n);
      for (int i = 0; i<val.size; i++) {
        temp.arrayval[i] = val.arrayval[i];
      }
      assignVariable(n->name, temp);
      return;
    }
    assignVariable(n->name, val);
  }
  else if (auto n = dynamic_cast<IndexAssignmentNode*>(node)) {
    auto temp = lookupVariable(n->object->name);
    if (temp == scopes.back().end()) interpreter_error("Variable " + n->object->name + " undeclared.", n);  
    Value ind = evaluate(n->index);
    if (ind.type != ValueType::INT) interpreter_error("Array index must be int.", n);
    Value val = evaluate(n->value);
    if (temp->second.type != ValueType::ARRAY) interpreter_error("Identifier '" + n->object->name + "' must be array.", n);
    if (temp->second.subtype != val.type) interpreter_error("Assigned value type does not match variable type.", n);
    if (temp->second.size <= ind.intval) interpreter_error("Array index out of bounds.", n);
    Value x = temp->second;
    x.arrayval[ind.intval] = val;
    assignVariable(n->object, x);
  }
  else if (auto n = dynamic_cast<ExpressionStatementNode*>(node)) {
    evaluate(n->expression);
  }
  else if (auto n = dynamic_cast<IfStatementNode*>(node)) {
    Value val = evaluate(n->condition);
    if (val.type != ValueType::BOOL) interpreter_error("If condition must be boolean.", n);
    if (val.boolval) {
      Scope guard(scopes);
      for (auto it: n->codeblock) execute(it);
      return;
    }
    for (auto x: n->elseif) {
      Value v = evaluate(x->condition);
      if (v.type != ValueType::BOOL) interpreter_error("Else if condition must be boolean.", n);
      if (v.boolval) {
        Scope guard(scopes);
        for (auto it: x->codeblock) execute(it);
        return;
      }      
    }
    Scope guard(scopes);
    for (auto it: n->elseblock) execute(it);
  }
  else if (auto n = dynamic_cast<WhileLoopNode*>(node)) {
    Value val = evaluate(n->condition);
    if (val.type != ValueType::BOOL) interpreter_error("While condition must be boolean.", n);
    try {
      while (val.boolval) {
        try {
          Scope guard(scopes);
          for (auto it: n->codeblock) execute(it);
          val = evaluate(n->condition);
        }
        catch (ContinueException&) {
          val = evaluate(n->condition);
          continue;
        }
      }
    }
    catch (BreakException&) {}
  }
  else if (auto n = dynamic_cast<ForLoopNode*>(node)) {
    Value val = evaluate(n->iterable);
    if (val.type != ValueType::ARRAY) interpreter_error("For loop iterable must be array.", n);
    try {
      for (auto it: val.arrayval) {
        try {
          Scope guard(scopes);
          scopes.back()[n->iterator->name] = it;
          for (auto stmt: n->codeblock) execute(stmt);
        }
        catch (ContinueException&) {
          continue;
        }
      }
    }
    catch (BreakException&) {}
  }
  else if (auto n = dynamic_cast<ReturnNode*>(node)) {
    ReturnException r;
    r.value = evaluate(n->value);
    throw r;
  }
  else if (auto n = dynamic_cast<BreakNode*>(node)) {
    BreakException b;
    throw b;
  }
  else if (auto n = dynamic_cast<ContinueNode*>(node)) {
    ContinueException c;
    throw c;
  }
  else if (auto n = dynamic_cast<FunctionDeclarationNode*>(node)) {
    if (functions.find(n->name->name) != functions.end()) interpreter_error("Function '" + n->name->name + "' is already declared.", n);
    functions[n->name->name] = n;
    std::set<std::string> s;
    for (auto it: n->parameters) {
      if (s.find(it->name->name) != s.end()) interpreter_error("Duplicate parameters are not allowed.", n);
      s.insert(it->name->name);
    }
    for (auto it: n->body) {
      if (auto x = dynamic_cast<ReturnNode*>(it)) {
        if (n->returntype == TokenType::KW_INT && evaluate(x->value).type != ValueType::INT) interpreter_error("Return type must be integer.", x);
        if (n->returntype == TokenType::KW_FLOAT && evaluate(x->value).type != ValueType::FLOAT) interpreter_error("Return type must be float.", x);
        if (n->returntype == TokenType::KW_BOOL && evaluate(x->value).type != ValueType::BOOL) interpreter_error("Return type must be bool.", x);
        if (n->returntype == TokenType::KW_STRING && evaluate(x->value).type != ValueType::STRING) interpreter_error("Return type must be string.", x);
        if (n->returntype == TokenType::KW_ARRAY && evaluate(x->value).type != ValueType::ARRAY) interpreter_error("Return type must be array.", x);
        if (n->returntype == TokenType::KW_ARRAY) {
          if (n->returnsubtype == TokenType::KW_INT && evaluate(x->value).subtype != ValueType::INT) interpreter_error("Return type must be integer array.", x);
          if (n->returnsubtype == TokenType::KW_FLOAT && evaluate(x->value).subtype != ValueType::FLOAT) interpreter_error("Return type must be float array.", x);
          if (n->returnsubtype == TokenType::KW_BOOL && evaluate(x->value).subtype != ValueType::BOOL) interpreter_error("Return type must be bool array.", x);
          if (n->returnsubtype == TokenType::KW_STRING && evaluate(x->value).subtype != ValueType::STRING) interpreter_error("Return type must be string array.", x);
          // if (n->returnsubtype == TokenType::KW_ARRAY && evaluate(x->value).subtype != ValueType::ARRAY) interpreter_error("Return type must be array.", x);
        }
      }
    }
  }
  else if (auto n = dynamic_cast<IncrementNode*>(node)) {
    if (lookupVariable(n->name->name) == scopes.back().end()) interpreter_error("Variable " + n->name->name + " undeclared.", n);
    Value var = lookupVariable(n->name->name)->second;
    if (var.type == ValueType::INT) var.intval++;
    else if (var.type == ValueType::FLOAT) var.floatval++;
    else interpreter_error("Increment operator requries int or float.", n);
    assignVariable(n->name, var);
  }
  else if (auto n = dynamic_cast<DecrementNode*>(node)) {
    if (lookupVariable(n->name->name) == scopes.back().end()) interpreter_error("Variable " + n->name->name + " undeclared.", n);
    Value var = lookupVariable(n->name->name)->second;
    if (var.type == ValueType::INT) var.intval--;
    else if (var.type == ValueType::FLOAT) var.floatval--;
    else interpreter_error("Decrement operator requries int or float.", n);
    assignVariable(n->name, var);
  }
}

void Interpreter::execute(ProgramNode* program) {
  try {
    scopes.push_back(std::unordered_map<std::string, Value>());
    for (auto stmt: program->statements) execute(stmt);
  }
  catch (BreakException&) {
    interpreter_error("'break' used outside loop.", program);
  }
  catch (ContinueException&) {
    interpreter_error("'continue' used outside loop.", program);
  }
  catch (ReturnException&) {
    interpreter_error("'return' used outside function.", program);
  }
}