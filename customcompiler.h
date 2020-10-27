/*
This file contains datastructures and function prototypes necessary for implementation of this compiler.
The components of this file are as follows:

Data Structures:
    1. grammer
    2. tokenStream
    3. parseTree
    4. typeExpressionTable
*/

//////////////////////////////DATA STRUCTURES////////////////////////////////////////////

#ifndef DATA_STRUCTURE_SIZES
#define DATA_STRUCTURE_SIZES
    #define MAX_SIZE_OF_GRAMMAR_WORDS 40
    #define NUMBER_OF_GRAMMAR_RULES 63
    #define MAX_SIZE_OF_TOKEN_NAME MAX_SIZE_OF_GRAMMAR_WORDS
    #define MAX_SIZE_OF_INENTIFIER 21
    #define MAX_PARSE_TREE_CHILDREN 15
    #define MAX_SIZE_OF_LEXEME MAX_SIZE_OF_IDENTIFIER
#endif

#ifndef DATA_STRUCTURES
#define DATA_STRUCTURES
    // TODO: Data Structures

    // Data Structure: grammar
    // Here we define grammarNode. The grammer is an array of these grammarNodes, as declared in driver.c
    typedef struct grammarNodeStruct{
        char *grammarWord;
        struct grammarNodeStruct* next;
    }grammarNode;

    // Data Structure: tokenStream
    // TODO: Modify tokenName to be an enumeration
    typedef struct tokenStreamStruct{
        char *tokenName;
        char *lexeme;
        int lineNumber;
        struct tokenStreamStruct *next;
    }tokenStream;

    // Data Structure: parseTree
    typedef struct parseTreeStruct{
        // data items
        char *lexeme;
        char *tokenName;
        int isTerminal;
        int lineNumber;
        grammarNode* grammarRuleUsed;
        char *symbolName;
        int isLeaf;
        // child pointer
        struct parseTreeStruct *children[MAX_PARSE_TREE_CHILDREN];
    }parseTree;

    // Data Structure: typeExpressionTable
    typedef struct typeExpressionTableStruct{
        int data;

    }typeExpressionTable;

    typedef struct StackNode {
        char* str;
        int terminal;
        struct StackNode* next;
    } stack;

    typedef struct grammarOrderNodeStruct {
        int grammarRuleNum;
        struct grammarOrderNodeStruct *next;
    }grammarOrderNode;
#endif


////////////////////////////FUNCTION PROTOTYPES//////////////////////////////////////////

// Function Prototype: readGrammar( “grammar.txt”, grammar G)
int readGrammar (char* grammarFilePath,  grammarNode** G);

// Function Prototype: tokeniseSourcecode(  “sourcecode.txt”,  tokenStream  *s)
tokenStream* tokeniseSourcecode (char* sourceCodeFilePath,  tokenStream  *s);

// Function Prototype: createParseTree (parseTree  *t,  tokenStream  *s,  grammar  G)
int createParseTree (parseTree  *t,  tokenStream  *s,  grammarNode**  G);

// Function Prototype: traverseParseTree (parseTree *t, typeExpressionTable T)
int traverseParseTree (parseTree *t, typeExpressionTable T);

// Function Prototype: printParseTree (parseTree *t)
int printParseTree (parseTree *t);

// Function Prototype: printTypeExpressionTable (typeExpressionTable T)
int printTypeExpressionTable (typeExpressionTable T);

void print_grammar_rule(grammarNode* root);