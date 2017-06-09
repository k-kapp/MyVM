# MyVM

All source code files must have the extension .vml. Statements are separated by newline characters.

Code organisation is at the moment slightly messy. Not everything that is in the parser C source file, for example, is really part of the parser.

The commands of the VM are shown below. Parameters, where applicable, are shown in parentheses after the statement name:

+ push (integer or variable): pushes a specified integer to the top of the stack
+ pop (x)                   : pops the topmost value off the stack, and assigns its value to the variable x
+ add                       : adds the two integers at the top of the stack together, pops both off, and pushes the result to the top of the stack
+ mul                       : same as add, except multiplication instead of addition is performed
+ jmp (label)               : unconditional jump to the label specified
+ ifgt (label)              : jump to label specified if topmost value of stack is greater than zero
+ iflt (label)              : jump to label specified if topmost value of stack is less than zero
+ ifeq (label)              : jump to label specified if topmost value of stack is equal to zero
+ dup                       : pushes a duplicate of the value at the top of the stack, onto the top of the stack
+ print                     : prints the value at the top of the stack to the screen

Three example source files are given.

+ test_mul.vml: simple illustration of how the push, mul and print commands work
+ unevens.vml: prints uneven numbers between 10 and 0 in decreasing order
+ gcd.vml: finds the greatest common denominator between two numbers (must be specified in source code). In the example here these numbers are 210 and 3450.
