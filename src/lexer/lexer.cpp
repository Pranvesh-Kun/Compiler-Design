#include "lexer.h"
#include <set>
#include <vector>

std::map<std::string, TokenType> symbolmap = {
    {"=", TokenType::ASSIGN},
    {"==", TokenType::EQUALS},
    {"!=", TokenType::NOT_EQUALS},
    {"<=", TokenType::LESS_EQUALS},
    {">=", TokenType::GRT_EQUALS},
    {"<", TokenType::LESS_THAN},
    {">", TokenType::GRT_THAN},

    {"+", TokenType::PLUS},
    {"-", TokenType::MINUS},
    {"*", TokenType::ASTERISK},
    {"/", TokenType::SLASH},
    {"%", TokenType::MOD},

    {"+=", TokenType::PLUS_EQUAL},
    {"-=", TokenType::MINUS_EQUAL},
    {"*=", TokenType::ASTERISK_EQUAL},
    {"/=", TokenType::SLASH_EQUAL},
    {"%=", TokenType::MOD_EQUAL},

    {"++", TokenType::INCREMENT},
    {"--", TokenType::DECREMENT},

    {"(", TokenType::L_PAREN},
    {")", TokenType::R_PAREN},

    {"{", TokenType::L_CURLY},
    {"}", TokenType::R_CURLY},

    {"[", TokenType::L_SQUARE},
    {"]", TokenType::R_SQUARE},

    {",", TokenType::COMMA}
};

std::map<std::string, TokenType> keywordmap = {
  {"int", TokenType::KW_INT},
  {"float", TokenType::KW_FLOAT},
  {"if", TokenType::KW_IF},
  {"else", TokenType::KW_ELSE},
  {"for", TokenType::KW_FOR},
  {"while", TokenType::KW_WHILE},
  {"let", TokenType::KW_LET},
  {"return", TokenType::KW_RETURN},
  {"continue", TokenType::KW_CONTINUE},
  {"break", TokenType::KW_BREAK},
  {"in", TokenType::KW_IN},
  {"string", TokenType::KW_STRING},
  {"bool", TokenType::KW_BOOL},
  {"array", TokenType::KW_ARRAY},
  {"void", TokenType::KW_VOID},
  {"input", TokenType::KW_INPUT},
  {"output", TokenType::KW_OUTPUT},
  {"and", TokenType::KW_AND},
  {"or", TokenType::KW_OR},
  {"not", TokenType::KW_NOT},
  {"true", TokenType::BOOL_LITERAL_TRUE},
  {"false", TokenType::BOOL_LITERAL_FALSE}
};

Lexer::Lexer(std::string s) {
  source = s;
  row = 0;
  col = 0;
  ind = 0;
  std::vector<Token> tokens;
}

bool Lexer::isKeyword() {
  if (keywordmap.find(buffer) != keywordmap.end()) return true;
  return false;
}

bool Lexer::isSymbol(char c) {
  if (symbolmap.find(std::string(1, c)) != symbolmap.end()) return true;
  return false;
}

bool Lexer::isIdentifier() {
  if (!(isalpha(buffer[0]) || buffer[0] == '_')) return false;
  for (auto c: buffer) {
    if (isalnum(c) || c == '_') continue;
    return false;
  }
  return true;
}

bool Lexer::isInteger() {
  for (auto c: buffer) {
    if (isdigit(c)) continue;
    return false;
  }
  return true;
}

bool Lexer::isFloat() {

}

void Lexer::assign_token() {
  Token tk;
  tk.line_num = row;
  tk.col_num = col;
  tk.text = buffer;
  if (isKeyword()) {
    tk.type = keywordmap[buffer];
  }
  else {
    // can be operator, literal or identifier.
    if (symbolmap.find(buffer) != symbolmap.end()) {
      tk.type = symbolmap[buffer];
    }
    else if (isIdentifier()) {
      tk.type = TokenType::IDENTIFIER;
    }
    else if (isInteger()) {
      tk.type = TokenType::INT_LITERAL;
    }
    else if (isFloat()) {
      tk.type = TokenType::FLOAT_LITERAL;
    }
  }
  tokens.push_back(tk);
}

bool Lexer::need_break() {

}

std::vector<Token> Lexer::tokenize() {
  while (ind < (int)source.size()) {
    if (need_break()) {
      if (buffer.empty()) {
        ind++;
        continue;
      }
      assign_token();
      buffer.clear();
    }
    else {
      buffer.push_back(source[ind]);
    }
    ind++;
  }
  assign_token();
  return tokens;
}
