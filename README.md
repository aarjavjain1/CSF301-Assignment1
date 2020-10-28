# CSF301-Assignment1

1. `make all` to compile the code into .o files and `compiler`.
2. `./compiler` to run the code, put the source code in `sourcecode.txt` and grammar in `grammar.txt` (should be right associative, and unambiguous).
3. `make clean` to remove all .o files

* The driving code including the `main` function is in `driver.c`. 
* All data structure definitions are in `customcompiler.h`.
* All function definitions are in `customcompiler.c`.
* The grammar is in `grammar.txt` in a format that can be parsed by the program.
* Sample source code for testing the compiler is in `sourcecode.txt`.

## Details about the implementation

### Parse Tree Creation
The `createParseTree` function gets the grammar and tokenised source from other functions, and then calls a recursive function to build the parse tree. The recursion is further optimised using backtracking.

### Type Expression Table Creation

