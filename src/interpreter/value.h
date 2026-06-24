#pragma once
#include <string>
#include <vector>

enum class ValueType {
  INT,
  FLOAT,
  BOOL,
  STRING,
  ARRAY,
  VOID
};

class Value {
  public:
    ValueType type;
    ValueType subtype;
    int size;
    int intval;
    float floatval;
    std::string stringval;
    bool boolval;
    std::vector<Value> arrayval;
};