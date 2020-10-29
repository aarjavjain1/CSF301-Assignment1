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
    printf("Printing Tree ---- depth: %d ---- line number: %d parseTreeNode: %s \n", t->depth, t->lineNumber, t->symbolName);
    for (int i = 0; i < MAX_PARSE_TREE_CHILDREN; i++) {
        if (t->children[i])
            printParseTree(t->children[i]);
    }
    return 0;
}