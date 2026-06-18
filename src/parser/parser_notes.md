We are going to output an abstract syntax tree (AST), taking all the tokens as the inputs.

All of the tokens are put into a huge single tree. The root node is Root, other nodes are arranged in an hierarchy.
There will be a lot of nodes like ExpressionNode, IfStatementNode, etc. We need to handle all the statements that our program finds valid.

The statements/expression my language supports:

int x = 5
float pi = 3.14
bool flag = true
string name = "Pranvesh"
let luffy = "Goat" (type is inferred)
x = 10 
array(int) arr[n]
array(int) arr = [1, 2, 3, 4]
arr = [1, 2, 3]
arr[0] = 10
s[0] = '1' (string)
a++

if (x > 5) {
  // statements
}
else if (x < 5) {
  // statements
}
else {
  // statements
}

for (i in arr) {
  // statements
}

for (i in range(0, n)) {
  // statements
}

while (ind < n) {
  // statements
}

int add(int a, int b) {
  return a + b
}

input(a)
output("bye")

ProgramNode:
  statements: [*StatementNode,..]

ExpressionNode:
  pass

StatementNode:
  pass

IntLiteralNode:
  value:

IdentifierNode:
  name:

FloatLiteralNode:
  value:

StringLiteralNode:
  value:

BoolLiteralNode:
  value:

ArrayLiteralNode:
  elements: [*ExpressionNode,..]

IndexAccessNode:
  object: *ExpressionNode
  index: *ExpressionNode

IndexAssignmentNode:
  object: *ExpressionNode
  index: *ExpressionNode
  value: *ExpressionNode

VariableDeclarationNode:
  type:
  name:
  initializer: *ExpressionNode
  size: *ExpressionNode

AssignmentNode:
  name: *IdentifierNode
  value: *ExpressionNode

IfStatementNode:
  condition: *ExpressionNode
  codeblock: [*StatementNode,..]
  elseif: [*ElseIfNode,..]
  else: [*StatementNode,..]

ElseIfNode:
  condition: *ExpressionNode
  codeblock: [*StatementNode,..]

ForLoopNode:
  iterator: *IndentifierNode
  iterable: *ExpressionNode
  codeblock: [*StatementNode,..]

WhileLoopNode:
  condition: *ExpressionNode
  codeblock: [*StatementNode,..]

FunctionDeclarationNode:
  returntype:
  name:
  parameters: [*ParameterNode,..]
  body: [*StatementNode,..]

ParameterNode:
  type:
  name:

ReturnNode:
  value: *ExpressionNode

FunctionCallNode:
  name:
  arguments: [*ExpressionNode,..]

BinaryExpressionNode:
  operator: 
  left: *ExpressionNode
  right: *ExpressionNode

UnaryExpressionNode:
  operator:
  operand: *ExpressionNode

ExpressionStatementNode:
  expression: *ExpressionNode

Hierarchy:

ExpressionNode:
  IdentifierNode - ok ok
  IntLiteralNode - ok ok 
  FloatLiteralNode - ok ok
  BoolLiteralNode - ok ok 
  StringLiteralNode - ok ok
  UnaryExpressionNode - ok ok
  FunctionCallNode - ok ok
  ArrayLiteralNode - ok ok
  IndexAccessNode - ok ok

StatementNode:
  VariableDeclarationNode - ok ok
  AssignmentNode - ok ok 
  IncrementNode - ok ok
  DecrementNode - ok ok
  IndexAssignmentNode - ok ok
  IfStatementNode - ok ok
  ElseIfNode - ok ok
  WhileLoopNode - ok ok
  ForLoopNode - ok ok
  FunctionDeclarationNode - ok
  ReturnNode - ok ok
  ExpressionStatementNode - ok ok
  BreakNode - ok ok
  ContinueNode - ok ok

ParameterNode - ok ok

We are parsing expressiong recursively from higher precedence to lower precedence (lower calls higher). 

parseExpression
->parseOr
->parseAnd
->parseEquality
->parseComparison
->parseTerm
->parseFactor
->parseUnaryExpression
->parsePrimary

from lowest to highest, the precendence is as follows.

or
ans
== !=
>= > < <=
+ -
* / %
not

so, we do this, if its a * b * c

first we create tree for a * b,
left node is a -> evaluated by unaryexp(), then b is right, by unaryexp().
now (a*b) becomes left node, new tree is created again with c as right.
so:

*:
left: *:
        left: a
        right: b
right: c

is the final tree.

This is how we evaluate the expressions.

# Parser Phase Notes

## Goal

Convert token stream from lexer into an Abstract Syntax Tree (AST).

---

## Parser Structure

### Program

```text
Program
 ├─ Statement
 ├─ Statement
 └─ Statement
```

```cpp
parseProgram()
```

Loops until EOF and repeatedly calls:

```cpp
parseStatement()
```

---

## Statement Parsing

### Variable Declarations

Supported:

```text
int x = 5
float pi = 3.14
bool flag = true
string s = "hello"

int x
float y
string z

let x = 5

array(int) arr[10]
array(int) arr = [1,2,3]
```

Function:

```cpp
parseVariableDeclaration()
```

Special handling:

* let requires initializer
* arrays store:

  * type
  * subtype
  * optional size
  * optional initializer

---

### Assignments

Supported:

```text
x = 5

x += 5
x -= 5
x *= 5
x /= 5
x %= 5
```

Function:

```cpp
parseAssignment()
```

Compound assignments are desugared into:

```text
x += y

↓

x = x + y
```

using BinaryExpressionNode.

---

### Index Assignment

Supported:

```text
arr[0] = 10
```

Function:

```cpp
parseIndexAssignment()
```

---

### Increment / Decrement

Supported:

```text
x++
x--
```

Implemented as statements:

```cpp
IncrementNode
DecrementNode
```

Functions:

```cpp
parseIncrement()
parseDecrement()
```

---

### If Statements

Supported:

```text
if (...) {
}

else if (...) {
}

else {
}
```

Functions:

```cpp
parseIfStatement()
parseElseIf()
```

Stores:

* condition
* codeblock
* elseif list
* else block

---

### While Loops

Supported:

```text
while (...) {
}
```

Function:

```cpp
parseWhileLoop()
```

---

### For Loops

Supported:

```text
for (i in arr) {
}
```

Function:

```cpp
parseForLoop()
```

Stores:

* iterator
* iterable expression
* codeblock

---

### Functions

Supported:

```text
int add(int a, int b) {
}

void test() {
}
```

Functions:

```cpp
parseFunctionDeclaration()
parseParameter()
```

Parameters support:

```text
int
float
bool
string
array(type)
```

---

### Return

Supported:

```text
return expr
```

Function:

```cpp
parseReturn()
```

---

### Break / Continue

Supported:

```text
break
continue
```

Functions:

```cpp
parseBreak()
parseContinue()
```

---

### Expression Statements

Supported:

```text
foo()
input(x)
output(x)
```

Function:

```cpp
parseExpressionStatement()
```

---

## Expression Parsing

Implemented using recursive descent + precedence climbing.

### Precedence Order

Lowest → Highest

```text
or
and
== !=
< > <= >=
+ -
* / %
not
primary
```

---

### Call Chain

```cpp
parseExpression()
  -> parseOr()
  -> parseAnd()
  -> parseEquality()
  -> parseComparison()
  -> parseTerm()
  -> parseFactor()
  -> parseUnaryExpression()
  -> parsePrimary()
```

---

## Primary Expressions

Supported:

```text
123
3.14
true
false
"hello"

x

foo(...)

arr[0]

[1,2,3]

(expr)
```

Functions:

```cpp
parseIntLiteral()
parseFloatLiteral()
parseBoolLiteral()
parseStringLiteral()
parseIdentifier()
parseFunctionCall()
parseIndexAccess()
parseArrayLiteral()
parsePrimary()
```

---

## Unary Expressions

Supported:

```text
not x
not not x
```

Function:

```cpp
parseUnaryExpression()
```

AST:

```text
UnaryExpressionNode
```

---

## Binary Expressions

Built inside:

```cpp
parseFactor()
parseTerm()
parseComparison()
parseEquality()
parseAnd()
parseOr()
```

Pattern:

```cpp
left = lower_precedence()

while(operator_found) {
    node->left = left
    node->right = lower_precedence()

    left = node
}

return left
```

Key idea:

```text
left = expression parsed so far
```

This naturally enforces operator precedence.

---

## Error Handling

Implemented:

```cpp
parser_error(expected_token)
parser_error_statement()
parser_error_expression()
```

Includes:

* line number
* column number
* expected token
* actual token

---

## Important Lessons Learned

1. Parser checks syntax only.
2. Type checking belongs to semantic analysis.
3. Expression parser returns ExpressionNode*.
4. BinaryExpressionNode is built inside precedence functions.
5. UnaryExpressionNode recursively parses unary expressions.
6. Operator precedence is enforced by parser structure.
7. AST nodes represent structure, not execution.





