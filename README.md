# Cypher

Cypher is a custom interpreted programming language built entirely from scratch in C++.

This project implements a complete language execution pipeline:

```txt
Cypher Source Code → Lexer → Tokens → Parser → AST → Interpreter → Program Output
```

The Cypher language supports static typing, type inference, typed arrays, functions, recursion, lexical scoping, control flow constructs, and runtime semantic validation.

The implementation includes:

* Lexical analysis and tokenization
* Recursive Descent Parsing
* Abstract Syntax Tree (AST) construction
* Operator precedence parsing
* Symbol table management
* Lexical scope resolution
* Function call handling and recursion
* Runtime semantic validation
* Exception-based control flow (`return`, `break`, `continue`)
* RAII-based scope management

The Cypher interpreter executes programs directly from the AST and supports user-defined functions, typed arrays, nested scopes, control flow constructs, recursion, and detailed runtime error reporting.


## Table of Contents

- [Data Types](#data-types)
- [Variables](#variables)
- [Arrays](#arrays)
- [Operators](#operators)
- [Scopes](#scopes)
- [Conditionals](#conditionals)
- [Loops](#loops)
- [Functions](#functions)
- [Recursion](#recursion)
- [Input / Output](#input--output)
- [Error Handling](#error-handling)
- [Implementation](#implementation)
- [Debugging Tools](#debugging-tools)
- [Building & Running](#building--running)
- [Status](#status)

---

## Example Program

```
int fact(int n) {
    if (n == 0) {
        return 1
    }

    return n * fact(n - 1)
}

output(fact(5))
```
**Output**
```
120
```

---

## Data Types

| Type         | Description            |
|--------------|------------------------|
| `int`        | Integer values         |
| `float`      | Floating point values  |
| `bool`       | Boolean values         |
| `string`     | Text strings           |
| `array(T)`   | Typed arrays           |

```
int x = 5
float pi = 3.14
bool flag = false
string name = "Pranvesh"

array(int) nums[5] = [1, 2, 3, 4, 5]
```

---

## Variables

### Explicit Typing

```
int x = 5
float pi = 3.14
bool flag = false
string name = "hello"
```

### Type Inference

Use the `let` keyword to infer the type from the initializer:

```
let x = 100
let pi = 3.14
let flag = true
```

### Default Initialization

If no initializer is provided, variables receive a default value automatically:

| Type     | Default |
|----------|---------|
| `int`    | `0`     |
| `float`  | `0.0`   |
| `bool`   | `false` |
| `string` | `""`    |

```
int x
float y
bool flag
string name
```

### Assignment

```
x = 10
x = 254891
```

---

## Arrays

Arrays are strongly typed.

```
array(int) arr[5] = [1, 2, 3, 4, 5]
array(float) nums[2] = [3.14, 5.0]
array(string) names[3] = ["Pranvesh", "says", "hi"]
array(bool) flags[3] = [false, true, false]
```

### Size Inference

```
array(int) arr = [1, 3, 6]
```

The array size is automatically inferred from the initializer.

### Default Initialization

```
array(int) nums[5]
```

Each element receives the default value of its subtype.

### Partial Reassignment

```
array(int) egg[5] = [1, 2, 3, 4, 5]
egg = [5, 6, 7]
// Result: egg = [5, 6, 7, 4, 5]
```

Unfilled positions retain their previous values.

### Indexing

```
output(arr[0])
arr[2] = 100
```

Bounds checking and index type validation are fully supported.

---

## Operators

### Arithmetic

| Operator | Description    |
|----------|----------------|
| `+`      | Addition       |
| `-`      | Subtraction    |
| `*`      | Multiplication |
| `/`      | Division       |
| `%`      | Modulo         |

### Comparison

| Operator | Description            |
|----------|------------------------|
| `==`     | Equal                  |
| `!=`     | Not Equal              |
| `>`      | Greater Than           |
| `>=`     | Greater Than or Equal  |
| `<`      | Less Than              |
| `<=`     | Less Than or Equal     |

### Logical

| Operator | Description  |
|----------|--------------|
| `and`    | Logical AND  |
| `or`     | Logical OR   |
| `not`    | Logical NOT  |

### Increment / Decrement

Supported for `int` and `float`:

```
x++
x--
y++
y--
```

### Operator Precedence

Highest to lowest:

| Level | Operators        |
|-------|------------------|
| 1     | `not`            |
| 2     | `*` `/` `%`      |
| 3     | `+` `-`          |
| 4     | `>` `>=` `<` `<=`|
| 5     | `==` `!=`        |
| 6     | `and`            |
| 7     | `or`             |

Parentheses can be used to override precedence.

---

## Scopes

The language implements proper lexical scoping. Variables declared inside a block exist only within that block.

```
int x = 5

if (true) {
    int x = 10
}

output(x)  // 5
```

Scope management is implemented using the **RAII** pattern — each block automatically creates a new scope and destroys it on exit.

Scopes are supported for: `if`, `else if`, `else`, `while`, `for`, and function calls.

---

## Conditionals

```
if (condition) {
    // body
}
else if (condition) {
    // body
}
else {
    // body
}
```

**Example:**

```
if (x > 5) {
    x = 10
}
else if (x == 5) {
    output("equal")
}
else {
    output("smaller")
}
```

Conditions must evaluate to a `bool`.

---

## Loops

### While

```
while (condition) {
    // body
}
```

```
while (x < 5) {
    output(x)
    x++
}
```

### For

```
for (iterator in iterable) {
    // body
}
```

```
array(int) arr[5] = [1, 2, 3, 4, 5]

for (i in arr) {
    output(i)
}
```

### Break and Continue

```
while (x < 5) {
    if (x == 1) {
        continue
    }
    x++
}
```

```
for (i in arr) {
    if (i == 67) {
        break
    }
    output(i)
}
```

---

## Functions

```
returntype function_name(parameters) {
    // body
    return value
}
```

**Example:**

```
int add(int a, int b) {
    return a + b
}

add(5, 6)
```

Functions support:

- Typed parameters and return values
- Array parameters and array return values
- Recursion
- Local scopes

### Void Functions
Void functions are supported. A void function must return 0.
```
void greet() {
    return 0
}
```

---

## Recursion

Recursion is fully supported.

```
int fact(int n) {
    if (n == 0) {
        return 1
    }
    return n * fact(n - 1)
}
```

---

## Input / Output

```
input(x)    // reads a value from stdin into x
output(x)   // prints x to stdout
```

---


## Error Handling

The interpreter performs extensive runtime semantic validation. All detected errors terminate execution immediately with a descriptive message.

Supported checks include:

- Undeclared variables
- Duplicate variable / function / parameter declarations
- Type mismatch during assignment
- Invalid array subtype assignments
- Invalid function arguments and argument counts
- Invalid return types and array return subtypes
- Array index out of bounds and invalid indexing
- Invalid arithmetic, comparison, and logical operations
- Non-boolean conditions in `if` / `while`
- Invalid iterables in `for` loops
- `break` / `continue` outside loops
- `return` outside functions

---

## Implementation

> Built to understand how programming languages work internally — from raw source code to execution.

### Lexer

Performs lexical analysis, converting raw source into a token stream.

**Responsibilities:**
- Tokenization
- Keyword and identifier recognition
- Number and string parsing
- Operator parsing
- Delimiter handling
- Comment handling

**Internally uses:**
- String processing
- Hash maps
- Character classification
- State-based scanning

### Parser

Uses **Recursive Descent Parsing** to build the AST.

**Responsibilities:**
- Syntax validation
- AST construction
- Operator precedence handling
- Function / loop / conditional parsing

Operator precedence is enforced through a hierarchy of parsing functions.

### Abstract Syntax Tree (AST)

The parser constructs an Abstract Syntax Tree (AST), which represents the hierarchical structure of the program independently of execution.

Key node types:

- `ProgramNode`
- `VariableDeclarationNode`
- `AssignmentNode`
- `FunctionDeclarationNode` / `FunctionCallNode`
- `BinaryExpressionNode` / `UnaryExpressionNode`
- `IfStatementNode`
- `WhileLoopNode` / `ForLoopNode`
- `ReturnNode`

### Interpreter

Walks the AST and executes the program.
Execution is performed using a tree-walk interpreter that recursively traverses AST nodes and evaluates them at runtime.

**Responsibilities:**
- Variable and scope management
- Function execution
- Recursion
- Array operations
- Semantic validation
- Runtime error detection

**Internally uses:**
- Hash maps
- Vectors
- Trees
- Exceptions
- RAII-based scope management
- Dynamic dispatch via RTTI and `dynamic_cast`

---

## Debugging Tools

### Token Printer

Displays all tokens generated by the lexer. Useful for debugging lexical analysis.

### AST Printer

Recursively traverses and displays the full syntax tree in a hierarchical format. Useful for verifying parser behavior and AST construction.

---

## Building & Running

**Build:**

```bash
g++ main.cpp src/**/*.cpp -std=c++17 -o cypher
```

**Run:**

Place source code in `tests/source.txt`, then:

```bash
./cypher
```

---

## Project Statistics

- Language: C++
- Approximate implementation size: ~1800 lines of C++ code.
- Parsing Technique: Recursive Descent Parsing
- Execution Model: Tree-Walk Interpreter
- Data Structures Used:
  - Hash Maps
  - Vectors
  - Abstract Syntax Trees
  - Scope Stacks
- Exception Handling Used For:
  - return
  - break
  - continue

## Status

**Cypher 1.0**

Implemented:

- Language Design
- Lexer
- Parser
- AST
- Interpreter
- Functions & Recursion
- Arrays
- Scopes
- Runtime Semantic Validation
- Error Handling
