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
        printf ("%20s\t%10s\t%10s\t%5s\t%15s\t%5s\t%50s\n", "Symbol", "isTerminal", "Lexeme", "Line Number", "Grammar Rule Number", "Depth", "Type Expression");
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
    printf("%5d\t", getLineNumber(t));
    if (!(t->isLeaf)) {
        printf("%11s %3d\t", "Rule Number", t->grammarRuleUsed->grammarRuleNum);
    } else {
        printf("%15s\t", "-");
    }
    printf("%5d\t", t->depth);
    if (!(t->isLeaf) && t->type) {
        printTypeExpression(t->type);
        printf("\t");
    } else {
        printf ("%50s\t", "-");
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