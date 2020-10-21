/* 
This file contains datastructures and function prototypes necessary for implementation of this compiler.
The components of this file are as follows:

Data Structures:
    1. grammer
    2. tokenStream
    3. parseTree
    4. typeExpressionTable
*/

// TODO: Data Structures

// Data Structure: grammar 
typedef struct grammarStruct{
    int data;
}grammar;

// Data Structure: tokenStream 
typedef struct tokenStreamStruct{
    int data;
}tokenStream;

// Data Structure: parseTree 
typedef struct parseTreeStruct{
    int data;
}parseTree;

// Data Structure: typeExpressionTable
typedef struct typeExpressionTableStruct{
    int data;
}typeExpressionTable;

// Function Prototype: readGrammar( “grammar.txt”, grammar G)
readGrammar (char* grammarFilePath,  grammar G);

// Function Prototype: tokeniseSourcecode(  “sourcecode.txt”,  tokenStream  *s)
tokeniseSourcecode (char* sourceCodeFilePath,  tokenStream  *s);

// Function Prototype: createParseTree (parseTree  *t,  tokenStream  *s,  grammar  G)
createParseTree (parseTree  *t,  tokenStream  *s,  grammar  G);

// Function Prototype: traverseParseTree (parseTree *t, typeExpressionTable T)
traverseParseTree (parseTree *t, typeExpressionTable T);

// Function Prototype: printParseTree (parseTree *t)
printParseTree (parseTree *t);

// Function Prototype: printTypeExpressionTable (typeExpressionTable T)
printTypeExpressionTable (typeExpressionTable T);