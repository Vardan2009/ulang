# uLang
This is a programming language I made with a goal of making the interpreter as small as I can.
It is written in C and the binary size is only `~21kb` The source code is 140 lines (5.43kb).
The language includes stuff like variables, output, conditional branching and labels

## Syntax
```
~ this is a comment, each line is a seperate command
~
LBL MAIN           ~ label
~
SET X 10            ~ set X to 10
SET Y 0             ~ set Y to 0
MOV X Y             ~ assign variable var1 = var2
ADD X Y             ~ add variable variable by another var1 += var2
INC X               ~ increment variable 1
DEC Y               ~ decrement variable 1
OUT D X             ~ print variable with format (D)ecimal
OUT C Y             ~ print variable with format (C)har
~
ENLEQ X Y           ~ (E)xecute (N)ext (L)ine if (EQ)ual          var1 == var2
ENLNEQ X Y          ~ (E)xecute (N)ext (L)ine if (N)ot (EQ)ual    var1 != var2 
ENLGT X Y           ~ (E)xecute (N)ext (L)ine if (G)reater (T)han var1 > var2
ENLLT X Y           ~ (E)xecute (N)ext (L)ine if (L)ess (T)han    var1 < var2
JMP MAIN            ~ jump to label 1
```

## Fibonacci Sequence
Here is the code for printing the first numbers of the fibonacci sequence in ulang
```
~ Initialize variables
SET A 0          ~ A will store the first Fibonacci number
SET B 1          ~ B will store the second Fibonacci number
SET N 32         ~ N will store the seperator character
SET C 10         ~ C will store the counter for the sequence length (first 10 numbers)
SET I 1          ~ I will store the current index (starting from 1 for counting purposes)
~
~ Print the first Fibonacci number
OUT D A
OUT C N
~
~ Print the second Fibonacci number
OUT D B
OUT C N
~
~ Define the label LOOP
LBL LOOP
~
~ Increment the index
INC I
~
~ Calculate the next Fibonacci number
MOV D A          ~ D = A
MOV A B          ~ A = B
ADD B D          ~ B = B + D
~
~ Print the next Fibonacci number
OUT D B
~ Print the seperator if the current index is less than 10
ENLLT I C
OUT C N
~
~ Continue the loop if the current index is less than 10
ENLLT I C
JMP LOOP
~
~ End of program
~
```