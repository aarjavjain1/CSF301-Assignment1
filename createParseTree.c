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

int isGrammarWordTerminal(char* str) {
    if (str[0] >= 'A' && str[0] <= 'Z')
        return 1;
    else
        return 0;
}

stack* stack_top(stack* root) {
    return root;
}

stack* stack_pop(stack* root) {
    if (root == NULL)
        return NULL;
    stack* temp = root;
    root = root->next;
    free(temp->str);
    free(temp);
    return root;
}

stack* stack_push(stack* root, char* str) {
    stack* stackNode = (stack*)malloc(sizeof(stack));
    stackNode->str = (char*)malloc(sizeof(char) * (strlen(str) + 1));
    strcpy(stackNode->str, str);
    stackNode->terminal = isGrammarWordTerminal(str);
    stackNode->next = root;
    // root = stackNode;
    // printf ("end of push to stack: %s\n", str);
    return stackNode;
}

stack* stack_pushrhs(stack* root, grammarNode* G) {
    if (G == NULL)
        return root;
    else {
        // stack_push(root, G->grammarWord);
        root = stack_pushrhs(root, G->next);
        root = stack_push(root, G->grammarWord);
    }
    return root;
}

parseTree* parseTreeGetCurrent(parseTree* t) {
    if (t == NULL)
        return NULL;
    parseTree* retpointer;
    int hasChild = 0;
    // printf("exporing %d, ", t->isLeaf);
    if (t->children[0] != NULL) {
        hasChild = 1;
    }
    // printf("haschild: %d\n", hasChild);
    if (hasChild == 0 && t->isLeaf == 0)
        return t;

    for (int i = 0; i < MAX_PARSE_TREE_CHILDREN; i++) {
        if (t->children[i] == NULL)
            break;
        // printf("inside for child i: %d\n", i);
        retpointer = parseTreeGetCurrent(t->children[i]);
        if (retpointer != NULL)
            return retpointer;
    }
    // printf ("returning NULL\n");
    return NULL;
}

void populateChildrenGrammarNode(parseTree* current, grammarNode* Gi) {
    if (current == NULL)
        // printf("populateChildren Got null current with rule: %s", Gi->grammarWord);
    int childToPopulate = 0;
    while (Gi->next != NULL) {
        Gi = Gi->next;
        int terminal = isGrammarWordTerminal(Gi->grammarWord);
        current->children[childToPopulate] = (parseTree*)malloc(sizeof(parseTree));
        current->children[childToPopulate]->isTerminal = terminal;
        current->children[childToPopulate]->grammarRuleUsed = NULL;
        current->children[childToPopulate]->lexeme = NULL;
        current->children[childToPopulate]->tokenName = NULL;
        current->children[childToPopulate]->type = NULL;
        current->children[childToPopulate]->isLeaf = 0;
        current->children[childToPopulate]->symbolName = NULL;
        current->children[childToPopulate]->lineNumber = 0;
        for (int i = 0; i < MAX_PARSE_TREE_CHILDREN; i++)
            current->children[childToPopulate]->children[i] = NULL;
        // printf("populated %s", Gi->grammarWord);
        childToPopulate++;
        // other fields for this node in the parse tree will be filled in createParseTree if terminal on stack is detected
    }
}

void populateParseTreeDepth(parseTree* t, int currentDepth) {
    if (t == NULL)
        return;
    t->depth = currentDepth;
    for (int i = 0; i < MAX_PARSE_TREE_CHILDREN; i++) {
        populateParseTreeDepth(t->children[i], currentDepth + 1);
    }
}

int predictRule(int grammarRuleNum, grammarNode** G, tokenStream** recievedToken, grammarOrderNode** grammarOrderAddress, parseTree* t) {
    stack* st = NULL;
    st = stack_push(st, "DOLLAR");
    st->next = NULL;
    st = stack_pushrhs(st, G[grammarRuleNum]->next);
    tokenStream* currentToken = (*recievedToken);

    populateChildrenGrammarNode(t, G[grammarRuleNum]);
    parseTree* current = parseTreeGetCurrent(t);

    while (strcmp(stack_top(st)->str, "DOLLAR")) {
        if ((stack_top(st)->terminal == 1) && !strcmp(stack_top(st)->str, currentToken->tokenName)) {
            st = stack_pop(st);
            current->lexeme = currentToken->lexeme;
            current->lineNumber = currentToken->lineNumber;
            current->tokenName = currentToken->tokenName;
            current->symbolName = currentToken->tokenName;
            current->isLeaf = 1;
            for (int i = 0; i < MAX_PARSE_TREE_CHILDREN; i++)
                current->children[i] = NULL;
            current = parseTreeGetCurrent(t);
            currentToken = currentToken->next;
        } else if (stack_top(st)->terminal == 0) {
            int ruleSelectedFlag = 0;
            for (int i = 0; i < NUMBER_OF_GRAMMAR_RULES; i++) {
                if (!strcmp(G[i]->grammarWord, stack_top(st)->str) && !ruleSelectedFlag) {
                    tokenStream* sentToken = currentToken;
                    int predict = predictRule(i, G, &sentToken, grammarOrderAddress, current);
                    if (predict == 1) {
                        currentToken = sentToken;
                        ruleSelectedFlag = 1;
                        st = stack_pop(st);
                        current->grammarRuleUsed = G[i];
                        current->symbolName = G[i]->grammarWord;
                        current = parseTreeGetCurrent(t);
                    } else {
                        freeParseTreeNodeChildren(current);
                    }
                }
            }
            if (!ruleSelectedFlag) {
                return 0;
            }
        } else {
            freeStackMemory(st);
            return 0;
        }
    }
    freeStackMemory(st);
    (*recievedToken) = currentToken;
    return 1;
}

// Function Definition: createParseTree (parseTree  *t,  tokenStream  *s,  grammar  G)
parseTree* createParseTree(parseTree* t, tokenStream* s, grammarNode** G) {
    // pseudocode if we have parsing table (M[X][a]): https://www.tutorialspoint.com/compiler_design/compiler_design_top_down_parser.htm

    grammarOrderNode* grammarOrder = NULL;

    stack* st = NULL;
    st = stack_push(st, "DOLLAR");
    st = stack_push(st, "main_program");
    t = malloc(sizeof(parseTree));
    parseTree* current = t;
    for (int i = 0; i < MAX_PARSE_TREE_CHILDREN; i++)
        current->children[i] = NULL;
    current->isTerminal = false;
    current->lexeme = NULL;
    current->type = NULL;
    current->lineNumber = 0;
    current->symbolName = malloc(sizeof(char) * (strlen("main_program") + 1));
    strcpy(current->symbolName, "main_program");
    current->tokenName = NULL;
    current->isLeaf = 0;
    tokenStream* currentToken = s;
    int debugCounter = 0;
    while (strcmp(stack_top(st)->str, "DOLLAR")) {
        if ((stack_top(st)->terminal == 1) && !strcmp(stack_top(st)->str, currentToken->tokenName)) {
            st = stack_pop(st);
            current->lexeme = currentToken->lexeme;
            current->lineNumber = currentToken->lineNumber;
            current->tokenName = currentToken->tokenName;
            current->symbolName = currentToken->tokenName;
            current->isLeaf = 1;
            for (int i = 0; i < MAX_PARSE_TREE_CHILDREN; i++)
                current->children[i] = NULL;
            current = parseTreeGetCurrent(t);
            currentToken = currentToken->next;
        } else if (stack_top(st)->terminal == 0) {
            int ruleSelectedFlag = 0;
            for (int i = 0; i < NUMBER_OF_GRAMMAR_RULES; i++) {
                if (!strcmp(G[i]->grammarWord, stack_top(st)->str) && !ruleSelectedFlag) {
                    tokenStream* sentToken = currentToken;
                    int predict = predictRule(i, G, &sentToken, &grammarOrder, current);
                    if (predict == 1) {
                        currentToken = sentToken;
                        ruleSelectedFlag = 1;
                        st = stack_pop(st);
                        current->grammarRuleUsed = G[i];
                        current->symbolName = G[i]->grammarWord;
                        // if (current->children[0] == NULL)
                        //     printf("KHALi!!!\n");
                        current = parseTreeGetCurrent(t);
                    } else {
                        freeParseTreeNodeChildren(current);
                        for (int i = 0; i < MAX_PARSE_TREE_CHILDREN; i++)
                            current->children[i] = NULL;
                    }
                }
            }
            if (!ruleSelectedFlag) {
                // printf("st ka top: %s, token: %s\n", st->str, currentToken->tokenName);
                printf("No rule Selected, Some error present\n");
                return NULL;
            }
        } else {
            // printf("Terminal at top of stack, but doesn't match, some problem exists\n");
            // printf("st ka top: %s, token: %s\n", st->str, currentToken->tokenName);
            return NULL;
        }
    }
    freeStackMemory(st);

    populateParseTreeDepth(t, 0);

    printf("Parse tree created.\n");
    return t;
}
