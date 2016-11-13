# MyVM

All source code files must have the extension .vml. 

The commands of the VM are as follows:

+ push (integer or variable): pushes a specified integer to the top of the stack
+ pop (x): pops the topmost value off the stack, and assigns its value to the variable x
+ add: adds the two integers at the top of the stack together, pops both off, and pushes the result to the top of the stack
+ mul: same as add, except multiplication instead of addition is performed
+ jmp (label): unconditional jump to the label specified
+ ifgt (label): jump to label specified if topmost value of stack is greater than zero
+ iflt (label): jump to label specified if topmost value of stack is less than zero
+ ifeq (label): jump to label specified if topmost value of stack is equal to zero
+ dup: pushes a duplicate of the value at the top of the stack, onto the top of the stack
+ print: prints the value at the top of the stack to the screen
