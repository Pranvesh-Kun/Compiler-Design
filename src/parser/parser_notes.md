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
  IdentifierNode
  IntLiteralNode
  FloatLiteralNode
  BoolLiteralNode
  StringLiteralNode
  BinaryExpressionNode
  UnaryExpressionNode
  FunctionCallNode
  ArrayLiteralNode
  IndexAccessNode

StatementNode:
  VariableDeclarationNode
  AssignmentNode
  IndexAssignmentNode
  IfStatementNode
  ElseIfNode
  WhileLoopNode
  ForLoopNode
  FunctionDeclarationNode
  ReturnNode
  ExpressionStatementNode

ParameterNode




