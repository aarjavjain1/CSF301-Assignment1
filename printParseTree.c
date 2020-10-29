#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "datastructuresandprototypes.h"


int printParseTreeWithDepth(parseTree *t, int currentDepth){
    printf("Printing Tree ---- depth: %d ---- line number: %d parseTreeNode: %s \n", currentDepth, t->lineNumber, t->symbolName);
    for (int i = 0; i< MAX_PARSE_TREE_CHILDREN; i++){
        if (t->children[i])
            printParseTreeWithDepth(t->children[i], currentDepth + 1);
    }
    return 0;
}

// Function Definition: printParseTree (parseTree *t)
int printParseTree(parseTree *t){
    int currentDepth = 0;
    printParseTreeWithDepth(t, currentDepth);
    return 0;
}
