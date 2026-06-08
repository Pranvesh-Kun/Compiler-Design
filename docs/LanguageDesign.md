Language name is still not decided.
It should be simple, not too complex and easy to write.

It is a static typed language.
We will be using 0 based indexing.

No semicolons are used to identify the end of sentence.
Uses curly braces for blocks.

We do not need a main function. We do not have an entry point, just start typing normally.
Also multiline strings are allowed.

We will use # to comment.

# this is a comment.

###
this is a multiline
comment.
###

Primitive Types:
int
float
bool (true/false)
string

We do not have a char type, just going to use string.

Variables:

Variables are explicitly typed.

Eg:

int x = 5
string name = "Tony"

Type inference is also available.

let x = 5 (assumes int type)

Once type is determined, it cannot be changed.

If value is not initialized:
int: 0
float: 0.0
string: ""
bool: Error

Arrays:

Fixed size array:

array(int) arr[n]

Dynamic array:

array(int) arr

array(int) arr = [1, 2, 3, 4]

is allowed.

Type inference is also allowed in arrays.

Mixed types is not allowed for arrays.

Array reassignment is allowed:

array(int) arr[5] = [1, 2, 3, 4, 5]
arr = [1, 2, 3] 

is allowed, ans the remaining values are initialized as 0.

but arr = [1, 2, 3, 4, 5, 6] is not allowed and will get error.

Strings have indexing, eg: s[0] will fetch first element.

Input: 
input(x) is the syntax to input a single element.
input(arr) will input the entire array.
input(arr[1]) also works.

Output:
output(x) is the syntax to output x.

If statements:

if (condition) {

}
else if (condition) {

}
else {

}

Loops:

Array iteration:

for (i in arr) {

}

index based:

for (int i = 0; i<n; i++) {

}

While loop:

while (condition) {

}

Functions:

int add(int a, int b) {
  return a+b
}

void functions are supported.
Recursion is also supported.

Operators:

Increment/Decrement:
i++ and i-- does exist, exactly like in c++.

Arithmetic Operators:

+
-
*
/
%

Compound Operators:

+=
-=
*=
/=
%=

Logical Operators:

and, or, not

Comparison Operators:

==
!=
<
>
<=
>=

Example Program:

int n
input(n)

array(int) arr[n]
input(arr)

int sum = 0

for (i in arr) {
  sum += i
}

output(sum)


