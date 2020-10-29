/*
Group Number: 55
Group Member details:
1. Aarjav Jain -- 2018A7PS0222P
2. Pranav Gupta -- 2018A7PS0190P
3. Harsh Sulakhe -- 2018A7PS0186P

This file contains datastructures and function prototypes necessary.
*/

//////////////////////////////DATA STRUCTURES////////////////////////////////////////////
#include <stdbool.h>

#ifndef DATA_STRUCTURE_SIZES
#define DATA_STRUCTURE_SIZES
    #define MAX_SIZE_OF_GRAMMAR_WORDS 40
    #define NUMBER_OF_GRAMMAR_RULES 55
    #define MAX_SIZE_OF_TOKEN_NAME MAX_SIZE_OF_GRAMMAR_WORDS
    #define MAX_SIZE_OF_INENTIFIER 21
    #define MAX_PARSE_TREE_CHILDREN 15
    #define MAX_SIZE_OF_LEXEME MAX_SIZE_OF_IDENTIFIER
    #define MAX_VARIABLES 1000
    #define MAX_LEN 1000

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
        int parent;
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
        int depth;
        // child pointer
        struct parseTreeStruct *children[MAX_PARSE_TREE_CHILDREN];
        typeExpressionTable* type;
    }parseTree;

#endif


////////////////////////////FUNCTION PROTOTYPES//////////////////////////////////////////

// Function Prototype: readGrammar( “grammar.txt”, grammar G)
int readGrammar (char* grammarFilePath,  grammarNode** G);

// Function Prototype: tokeniseSourcecode(  “sourcecode.txt”,  tokenStream  *s)
tokenStream* tokeniseSourcecode (char* sourceCodeFilePath,  tokenStream  *s);

// Function Prototype: createParseTree (parseTree  *t,  tokenStream  *s,  grammar  G)
parseTree* createParseTree (parseTree  *t,  tokenStream  *s,  grammarNode**  G);

// Function Prototype: traverseParseTree (parseTree *t, typeExpressionTable T)
int traverseParseTree (parseTree **t, typeExpressionTable **T);

// Function Prototype: printParseTree (parseTree *t)
int printParseTree (parseTree *t);

// Function Prototype: printTypeExpressionTable (typeExpressionTable T)
int printTypeExpressionTable (typeExpressionTable *T);


/////////////////////////////UTILITY FUNCTION PROTOTYPES//////////////////////////////////

void print_grammar_rule(grammarNode* root);

////// Utility functions to free memory on heap
void freeParseTreeNodeChildrenAll(parseTree *t);
void freeParseTreeNodeChildren(parseTree* t);
void freeParseTreeMemory(parseTree *rootOfParseTree);
void freeRectDimensionMemory(rect_dimension* rd);
void freeDimensionMemory(dimension* d);
void freeJaggedDimensionMemory(jagged_dimension* jd);
void freeTableExpression(expression* exp);
void freeTypeExpressionTableMemory(typeExpressionTable* tablePointer);
void freeTokenStreamMemory(tokenStream* t);
void freeGrammarRuleMemory(grammarNode* Gi);
void freeGrammarMemory(grammarNode** G);
void freeStackMemory(stack* st);

////// Utility functions for tokeniseSourcecode
char* trimwhitespace(char *str);
bool isVariable(char* str);
bool isNumber(char* str);
int search(char* token);
tokenStream* get_token(char* token,int* line_count);

////// Utility functions for readGrammar
// trimwhitespace(char* str) is utility for both reagGrammar and tokeniseSourcecode

////// Utility functions for createParseTree
int isGrammarWordTerminal(char* str);
stack* stack_top(stack* root);
stack* stack_pop(stack* root);
stack* stack_push(stack* root, char* str);
stack* stack_pushrhs(stack* root, grammarNode* G);
parseTree* parseTreeGetCurrent(parseTree* t);
void populateChildrenGrammarNode(parseTree* current, grammarNode* Gi);
int predictRule(int grammarRuleNum, grammarNode** G, tokenStream** recievedToken, grammarOrderNode **grammarOrderAddress, parseTree* t);
void populateParseTreeDepth(parseTree* t, int currentDepth);

////// Utility functions for traverseParseTree
void recurse(parseTree* t, parseTree* parseArray[], int* parseIndex);
void addDeclaration(parseTree** t, typeExpressionTable **T);
typeExpressionTable* getExpression(parseTree * input, typeExpressionTable* table);
void addAssignment(parseTree** t, typeExpressionTable *T);
int compare(typeExpressionTable* a, typeExpressionTable* b, int op);
int max(int a, int b);
int min(int a, int b);

////// Utilify functions for printParseTree
// No utility functions

////// Utility functions for printTypeExpressionTable
// No utility functions