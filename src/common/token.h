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

struct Token {
  TokenType type;
  std::string text;
  int line_num;
  int col_num;
};