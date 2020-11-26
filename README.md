# CSF301-Assignment1
A program designed to parse source code for a custom language (can be seen in `t1.txt` to `t6.txt`). The syntax and semantics of the source code are analysed for correctness using parse tree creation and type expression table formation. Any errors in the source code are displayed with line numbers.

Instructions to use the program:

1. `make all` to compile the code into `output`.
2. `./output inputcode.txt` to run the code, put the source code in `inputcode.txt` (name can be changed) and grammar in `grammar.txt` (should be right associative, and unambiguous).
3. `make clean` to remove all `.o` files.

* The driving code including the `main` function is in `driver.c`. 
* All data structure definitions are in `customcompiler.h`.
* The grammar is in `grammar.txt` in a format that can be parsed by the program.
* Sample source code for testing the compiler is in `t1.txt` to `t6.txt`.

