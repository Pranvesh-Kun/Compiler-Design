#include "lexer.h"
#include <set>
#include <vector>
#include <iostream>

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
  row = 1;
  col = 1;
  ind = 0;
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
  int cnt = 0;
  bool digit = false;
  for (auto c: buffer) {
    if (isdigit(c)) {
      digit = true;
      continue;
    }
    if (c == '.') cnt++;
    else return false;
  }
  if (cnt != 1 || !digit) return false;
  return true;
}

void Lexer::lexical_error() {
  std::cout << "Lexical Error at line " << row << ", column " << col-buffer.size() << ":\nInvalid Token: '" << buffer << "'\n";
  abort();
}

void Lexer::assign_token() {
  if (buffer.empty()) return;
  Token tk;
  tk.line_num = row;
  tk.col_num = col-buffer.size();
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
    else {
      lexical_error();
    }
  }
  tokens.push_back(tk);
}

std::vector<char> paren = {'{', '}', '(', ')', '[', ']'};

bool Lexer::need_break() {
  if (buffer.empty()) return false;
  if (source[ind] == ' ' || source[ind] == '\n') return true;
  if (isSymbol(source[ind]) && isalnum(buffer.back())) return true;
  if (isSymbol(buffer.back()) && isalnum(source[ind])) return true;
  for (auto p: paren) {
    if (p == source[ind] || p == buffer.back()) return true;
  }
  return false;
}

void Lexer::add_string() {
  Token str;
  str.type = TokenType::STR_LITERAL;
  str.line_num = row;
  int sz = (int)buffer.size();
  for (auto c: buffer) {
    if (c == '"' || c == '\'' || c == '\n' || c == '\t') sz++;
  }
  str.col_num = col-sz;
  str.text = buffer;
  tokens.push_back(str);
}

void Lexer::_EOF() {
  Token eof;
  eof.type = TokenType::END_OF_FILE;
  eof.col_num = col;
  eof.line_num = row;
  eof.text = "EOF";
  tokens.push_back(eof);
}

void Lexer::escape_error() {
  std::cout << "Lexical Error at line " << row << ", column " << col << ":\nUnknown escape sequence.\n";
  abort();
}

std::vector<Token> Lexer::tokenize() {
  bool in_sl_comment = false;
  bool in_ml_comment = false;
  bool in_string = false;
  while (ind < (int)source.size()) {
    if (source[ind] == '\\' && in_string) {
      if (ind + 1 < (int)source.size() && (source[ind+1] == '"' || source[ind+1] == '\\')) {
        ind++;
        col++;
        buffer.push_back(source[ind]);
        ind++;
        col++;
        continue;
      }
      else if (ind + 1 < (int)source.size()) {
        if (source[ind+1] == 'n') {
          ind+=2;
          col+=2;
          buffer.push_back('\n');
          continue;
        }
        if (source[ind+1] == 't') {
          ind+=2;
          col+=2;
          buffer.push_back('\t');
          continue;
        }
        else {
          escape_error();
        }
      }
    }
    if (source[ind] == '"') {
      if (in_string) {
        in_string = false;
        add_string();
        buffer.clear();
        ind++;
        col++;
      }
      else {
        assign_token();
        buffer.clear();
        in_string = true;
        ind++;
        col++;
      }
      continue;
    }
    if (in_string) {
      if (source[ind] == '\n') lexical_error();
      buffer.push_back(source[ind]);
      ind++;
      col++;
      continue;
    }
    if (source[ind] == '#') {
      if (!buffer.empty()) {
        buffer.push_back(source[ind]);
        col++;
        lexical_error();
      }
      if ((ind+1 < (int)source.size() && source[ind+1] == '#') && (ind+2 < (int)source.size() && source[ind+2] == '#')) {
        in_ml_comment = !in_ml_comment;
        ind += 2;
        col += 2;
        continue;
      }
      else in_sl_comment = true;
    }
    else if (need_break()) {
      if (!buffer.empty()) {
        assign_token();
        buffer.clear();
      }
    }
    if (source[ind] != ' ' && source[ind] != '\n' && !in_sl_comment && !in_ml_comment) {
      buffer.push_back(source[ind]);
    }
    if (source[ind] == '\n') {
      row++;
      col = 1;
      if (in_sl_comment) in_sl_comment = false;
    }
    else {
      col++;
    }
    ind++;
  }
  if (in_ml_comment) {
    std::cout << "Lexical Error at line " << row << ", column " << col-buffer.size()-1 << ":\nUnterminated multiline comment." << "\n";
    abort();
  }
  if (in_string) {
    std::cout << "Lexical Error at line " << row << ", column " << col-buffer.size()-1 << ":\nUnterminated string literal." << "\n";
    abort();
  }

  assign_token();
  _EOF();
  return tokens;
}

std::string Lexer::TokenToString(Token tk) {
  return tokenmap[tk.type];
}

void Lexer::printtokens() {
  for (auto token: tokens) {
    std::cout << TokenToString(token) << "\n";
    // std::cout << token.text << "\n";
  }
}