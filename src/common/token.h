#include <string>
#include <map>
#pragma once

enum class TokenType {
  KW_INT, // keyword int
  KW_FLOAT, // keyword float
  KW_IF, // keyword if
  KW_ELSE, // keyword else
  KW_FOR, // keyword for
  KW_WHILE, // keyword while
  KW_LET, // keyword let
  KW_RETURN, // keyword return
  KW_CONTINUE, // keyword continue
  KW_BREAK, // keyword break
  KW_IN, // keyword in
  KW_STRING, // keyword string
  KW_BOOL, // keyword bool
  KW_ARRAY, // keyword array
  KW_VOID, // keyword void
  KW_INPUT, // keyword input
  KW_OUTPUT, // keyword output
  KW_AND, // keyword and
  KW_OR, // keyword or
  KW_NOT, // keyword not
  IDENTIFIER, // identifier
  INT_LITERAL, // integer
  FLOAT_LITERAL, // float
  STR_LITERAL, // string
  BOOL_LITERAL_TRUE, // true
  BOOL_LITERAL_FALSE, // false
  ASSIGN, // =
  EQUALS, // ==
  NOT_EQUALS, // !=
  LESS_EQUALS, // <=
  GRT_EQUALS, // >=
  LESS_THAN, // <
  GRT_THAN, // >
  PLUS, // +
  MINUS, // -
  ASTERISK, // *
  SLASH, // /
  MOD, // %
  PLUS_EQUAL, // +=
  MINUS_EQUAL, // -=
  ASTERISK_EQUAL, // *=
  SLASH_EQUAL, // /=
  MOD_EQUAL, // %=
  INCREMENT, // ++
  DECREMENT, // --
  L_PAREN, // (
  R_PAREN, // )
  L_CURLY, // { 
  R_CURLY, // }
  L_SQUARE, // [
  R_SQUARE, // ]
  COMMA, // ,
  END_OF_FILE, // end of file
};


inline std::map<TokenType, std::string> tokenmap = {
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

struct Token {
  TokenType type;
  std::string text;
  int line_num;
  int col_num;
};