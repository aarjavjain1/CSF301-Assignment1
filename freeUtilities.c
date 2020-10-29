#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "datastructuresandprototypes.h"


void freeParseTreeNodeChildrenAll(parseTree *t){
    if (t == NULL)
        return;
    for (int i = 0; i < MAX_PARSE_TREE_CHILDREN; i++){
            freeParseTreeNodeChildrenAll(t->children[i]);
    }
    free(t);
}

void freeParseTreeNodeChildren(parseTree* t){
    if (t == NULL)
        return;
    for (int i = 0; i < MAX_PARSE_TREE_CHILDREN; i++){
        freeParseTreeNodeChildrenAll(t->children[i]);
    }
    for (int i = 0; i < MAX_PARSE_TREE_CHILDREN; i++)
        t->children[i] = NULL;
}

void freeParseTreeMemory(parseTree *rootOfParseTree){
    if (rootOfParseTree == NULL)
        return;
    for (int i = 0; i < MAX_PARSE_TREE_CHILDREN; i++){
            freeParseTreeNodeChildrenAll(rootOfParseTree->children[i]);
    }
    free(rootOfParseTree->symbolName);
    free(rootOfParseTree);
}

void freeRectDimensionMemory(rect_dimension* rd){
    if (rd == NULL)
        return;
    if (rd->low)
        free(rd->low);
    if (rd->high)
        free(rd->high);
    freeRectDimensionMemory(rd->next);
    free(rd);
}

void freeDimensionMemory(dimension* d){
    if (d == NULL)
        return;
    freeDimensionMemory(d->next);
    free(d);
}

void freeJaggedDimensionMemory(jagged_dimension* jd){
    if (jd == NULL)
        return;
    freeDimensionMemory(jd->inner_size);
    freeJaggedDimensionMemory(jd->next);
    free(jd);
}

void freeTableExpression(expression* exp){
    if (exp == NULL)
        return;
    if (exp->a)
        if (exp->a->basicElementType)
            free(exp->a->basicElementType);
    if (exp->b){
        if (exp->b->basicElementType)
            free (exp->b->basicElementType);
        freeRectDimensionMemory(exp->b->d);
        free(exp->b);
    }
    if (exp->c){
        if (exp->c->basicElementType)
            free (exp->c->basicElementType);
        freeJaggedDimensionMemory(exp->c->d);
        free (exp->c);
    }
}

void freeTypeExpressionTableMemory(typeExpressionTable* tablePointer){
    if (tablePointer == NULL)
        return;
    if (tablePointer->name) free(tablePointer->name);
    freeTableExpression(tablePointer->exp);
    freeTypeExpressionTableMemory(tablePointer->next);
    free(tablePointer);
}

void freeTokenStreamMemory(tokenStream* t){
    if (t == NULL)
        return;
    tokenStream* next = t->next;
    free(t->lexeme);
    free(t->tokenName);
    free(t);
    freeTokenStreamMemory(next);
}

void freeGrammarRuleMemory(grammarNode* Gi){
    if (Gi == NULL)
        return;
    grammarNode* next = Gi->next;
    free(Gi->grammarWord);
    free(Gi);
    freeGrammarRuleMemory(next);
}

void freeGrammarMemory(grammarNode** G){
    for (int i = 0; i < NUMBER_OF_GRAMMAR_RULES; i++){
        freeGrammarRuleMemory(G[i]);
    }
}

void freeStackMemory(stack* st){
    // printf("inside freeStackMemory\n");
    if (st == NULL)
        return;
    stack* next = st->next;
    // printf("freeing st node: %s\n", st->str);
    free(st->str);
    free(st);
    freeStackMemory(next);
}
