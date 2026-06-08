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

std::map<TokenType, std::string> tokenmap = {
    {TokenType::KW_INT, "KW_INT"},
    {TokenType::KW_FLOAT, "KW_FLOAT"},
    {TokenType::KW_IF, "KW_IF"},
    {TokenType::KW_ELSE, "KW_ELSE"},
    {TokenType::KW_FOR, "KW_FOR"},
    {TokenType::KW_WHILE, "KW_WHILE"},
    {TokenType::KW_LET, "KW_LET"},
    {TokenType::KW_RETURN, "KW_RETURN"},
    {TokenType::KW_CONTINUE, "KW_CONTINUE"},
    {TokenType::KW_BREAK, "KW_BREAK"},
    {TokenType::KW_IN, "KW_IN"},
    {TokenType::KW_STRING, "KW_STRING"},
    {TokenType::KW_BOOL, "KW_BOOL"},
    {TokenType::KW_ARRAY, "KW_ARRAY"},
    {TokenType::KW_VOID, "KW_VOID"},
    {TokenType::KW_INPUT, "KW_INPUT"},
    {TokenType::KW_OUTPUT, "KW_OUTPUT"},
    {TokenType::KW_AND, "KW_AND"},
    {TokenType::KW_OR, "KW_OR"},
    {TokenType::KW_NOT, "KW_NOT"},

    {TokenType::IDENTIFIER, "IDENTIFIER"},
    {TokenType::INT_LITERAL, "INT_LITERAL"},
    {TokenType::FLOAT_LITERAL, "FLOAT_LITERAL"},
    {TokenType::STR_LITERAL, "STR_LITERAL"},
    {TokenType::BOOL_LITERAL_TRUE, "BOOL_LITERAL_TRUE"},
    {TokenType::BOOL_LITERAL_FALSE, "BOOL_LITERAL_FALSE"},

    {TokenType::ASSIGN, "ASSIGN"},
    {TokenType::EQUALS, "EQUALS"},
    {TokenType::NOT_EQUALS, "NOT_EQUALS"},
    {TokenType::LESS_EQUALS, "LESS_EQUALS"},
    {TokenType::GRT_EQUALS, "GRT_EQUALS"},
    {TokenType::LESS_THAN, "LESS_THAN"},
    {TokenType::GRT_THAN, "GRT_THAN"},

    {TokenType::PLUS, "PLUS"},
    {TokenType::MINUS, "MINUS"},
    {TokenType::ASTERISK, "ASTERISK"},
    {TokenType::SLASH, "SLASH"},
    {TokenType::MOD, "MOD"},

    {TokenType::PLUS_EQUAL, "PLUS_EQUAL"},
    {TokenType::MINUS_EQUAL, "MINUS_EQUAL"},
    {TokenType::ASTERISK_EQUAL, "ASTERISK_EQUAL"},
    {TokenType::SLASH_EQUAL, "SLASH_EQUAL"},
    {TokenType::MOD_EQUAL, "MOD_EQUAL"},

    {TokenType::INCREMENT, "INCREMENT"},
    {TokenType::DECREMENT, "DECREMENT"},

    {TokenType::L_PAREN, "L_PAREN"},
    {TokenType::R_PAREN, "R_PAREN"},
    {TokenType::L_CURLY, "L_CURLY"},
    {TokenType::R_CURLY, "R_CURLY"},
    {TokenType::L_SQUARE, "L_SQUARE"},
    {TokenType::R_SQUARE, "R_SQUARE"},

    {TokenType::COMMA, "COMMA"},
    {TokenType::END_OF_FILE, "END_OF_FILE"}
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
  for (auto c: buffer) {
    if (isdigit(c)) continue;
    if (c == '.') cnt++;
    else return false;
  }
  if (cnt != 1) return false;
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

bool Lexer::need_break() {
  if (buffer.empty()) return false;
  if (source[ind] == ' ' || source[ind] == '\n') return true;
  if (isSymbol(source[ind]) && isalnum(buffer.back())) return true;
  if (isSymbol(buffer.back()) && isalnum(source[ind])) return true;
  return false;
}

std::vector<Token> Lexer::tokenize() {
  while (ind < (int)source.size()) {
    if (need_break()) {
      if (!buffer.empty()) {
        assign_token();
        buffer.clear();
      }
    }
    if (source[ind] != ' ' && source[ind] != '\n') {
      buffer.push_back(source[ind]);
    }
    if (source[ind] == '\n') {
      row++;
      col = 1;
    }
    else {
      col++;
    }
    ind++;
  }
  assign_token();
  return tokens;
}

std::string Lexer::TokenToString(Token tk) {
  return tokenmap[tk.type];
}

void Lexer::printtokens() {
  for (auto token: tokens) {
    std::cout << TokenToString(token) << "\n";
  }
}