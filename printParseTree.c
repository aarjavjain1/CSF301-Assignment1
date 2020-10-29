/*
Group Number: 55
Group Member details:
1. Aarjav Jain -- 2018A7PS0222P
2. Pranav Gupta -- 2018A7PS0190P
3. Harsh Sulakhe -- 2018A7PS0186P
*/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "datastructuresandprototypes.h"

// Function Definition: printParseTree (parseTree *t)
int printParseTree(parseTree *t) {
    if (t == NULL)
        return 1;
    if (t->depth == 0)
        printf ("Symbol\tisTerminal\tType Expression\tlexeme\tLine Number\tGrammar Rule Number\tDepth\n");
    printf("%s \t", t->symbolName);
    if (t->isTerminal){
        printf("true \t");
    } else {
        printf("false \t");
    }
    if (!(t->isLeaf)) {
        printTypeExpression(t->type);
        printf("\t");
    } else {
        printf ("- \t");
    } 
    if (t->isLeaf) {
        printf("%s \t", t->lexeme);
    } else {
        printf("- \t");
    }
    if (t->isLeaf) {
        printf("%d \t", t->lineNumber);
    } else {
        printf("- \t");
    }
    if (!(t->isLeaf)) {
        printf("Rule %d \t", t->grammarRuleUsed->grammarRuleNum);
    } else {
        printf("- \t");
    }
    printf("%d \t", t->depth);
    printf("\n");
    //line, grammar, depth
    
    // print children
    for (int i = 0; i < MAX_PARSE_TREE_CHILDREN; i++) {
        if (t->children[i])
            printParseTree(t->children[i]);
    }
    return 0;
}