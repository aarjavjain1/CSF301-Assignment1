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
    #define MAX_VARIABLES 1000
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

    typedef struct StackNode {
        char* str;
        int terminal;
        struct StackNode* next;
    } stack;

    typedef struct grammarOrderNodeStruct {
        int grammarRuleNum;
        struct grammarOrderNodeStruct *next;
    }grammarOrderNode;
    
    // Type Expression Table DS

    typedef enum {primitive, rect_array, jagged_array} Type;
    typedef enum {stat, dyn, NA} Array_Type;

    typedef struct dim {
        int size;
        struct dim* next;
    } dimension;

    typedef struct j_dim {
        int size;
        dimension* inner_size;
        struct j_dim *next;
    } jagged_dimension;

    typedef struct jagg{
        char *basicElementType;
        int dimensions;
        int low;
        int high;
        jagged_dimension *d;
    } jagged;

    typedef struct rect_dim {
        char* low;
        char* high;
        struct rect_dim *next;
    } rect_dimension;

    typedef struct rectangular{
        char *basicElementType;
        int dimensions;
        rect_dimension *d;
    } rect;

    typedef struct primitive{
        char *basicElementType;
    } prim;

    typedef union expr{
            prim *a;
            rect *b;
            jagged *c;
    } expression;

    typedef struct typeExpTable {
        char *name;
        Type type;
        Array_Type array_type;
        expression* exp;
        struct typeExpTable* next;
    } typeExpressionTable;

#endif


////////////////////////////FUNCTION PROTOTYPES//////////////////////////////////////////

// Function Prototype: readGrammar( “grammar.txt”, grammar G)
int readGrammar (char* grammarFilePath,  grammarNode** G);

// Function Prototype: tokeniseSourcecode(  “sourcecode.txt”,  tokenStream  *s)
tokenStream* tokeniseSourcecode (char* sourceCodeFilePath,  tokenStream  *s);

// Function Prototype: createParseTree (parseTree  *t,  tokenStream  *s,  grammar  G)
parseTree* createParseTree (parseTree  *t,  tokenStream  *s,  grammarNode**  G);

// Function Prototype: traverseParseTree (parseTree *t, typeExpressionTable T)
int traverseParseTree (parseTree *t, typeExpressionTable *T);

// Function Prototype: printParseTree (parseTree *t)
int printParseTree (parseTree *t);

// Function Prototype: printTypeExpressionTable (typeExpressionTable T)
int printTypeExpressionTable (typeExpressionTable *T);

void print_grammar_rule(grammarNode* root);