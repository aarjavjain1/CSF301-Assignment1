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
        printf ("%20s\t%10s\t%10s\t%15s\t  %10s\t\t%5s\t%10s\n", "Symbol", "isTerminal", "Lexeme", "Line Number", "Grammar Rule", "Depth", "Type Expression");
    printf("%20s\t", t->symbolName);
    if (t->isTerminal){
        printf("%10s\t","True");
    } else {
        printf("%10s\t","False");
    }
    if (t->lexeme) {
        printf("%10s\t", t->lexeme);
    } else {
        printf("%10s\t", "-");
    }
    // if (t->isLeaf) {
    //     printf("%d \t", t->lineNumber);
    // } else {
    //     printf("- \t");
    // }
    printf("%10d\t\t", getLineNumber(t));
    if (!(t->isLeaf)) {
        printf("%3d\t\t", t->grammarRuleUsed->grammarRuleNum);
    } else {
        printf("%3s\t\t", "-");
    }
    printf("%5d\t", t->depth);
    if (!(t->isLeaf) && t->type) {
        printTypeExpression(t->type);
        printf("\t");
    } else {
        printf ("%10s\t", "-");
    }
    printf("\n");
    //line, grammar, depth
    // print children
    for (int i = 0; i < MAX_PARSE_TREE_CHILDREN; i++) {
        if (t->children[i])
            printParseTree(t->children[i]);
    }
    return 0;
}