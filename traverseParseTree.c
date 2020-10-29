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

parseTree* parseArray[MAX_VARIABLES];
static int parseIndex = 0;

int getLineNumber(parseTree *t){
    parseTree* temp = t;
    while(temp && temp->children[0]) temp = temp->children[0];
    return temp->lineNumber;
}

int max(int a, int b){
    if (a > b) return a;
    else return b;
}

int min(int a, int b) {
    if (a > b)
        return b;
    else
        return a;
}

void recurse(parseTree* t) {
    if (t && t->isLeaf) {
        parseArray[parseIndex] = t;
        parseIndex++;
    } else {
        for (int i = 0; i < MAX_PARSE_TREE_CHILDREN; i++)
            if (t->children[i])
                recurse(t->children[i]);
    }
}

void addDeclaration(parseTree** t, typeExpressionTable** T) {
    // printf("Printing Declaration Tree ----  parseTreeNode: %s -> ", t->symbolName);
    for (int i = 0; i < MAX_VARIABLES; i++) {
        parseArray[i] = NULL;
    }
    parseIndex = 0;
    recurse(*t);
    // for (int i = 0; i < parseIndex; i++) {
    //     printf("%s ", parseArray[i]->symbolName);
    // }
    // printf("\n");
    int low_var = 200000000, high_var = -200000000, colon = -1;
    for (int i = 0; i < parseIndex; i++) {
        // printf("%s ", parseArray[i]->symbolName);
        if (!strcmp(parseArray[i]->symbolName, "VAR") && colon == -1) {
            low_var = min(low_var, i);
            high_var = max(high_var, i);
        }
        if (colon == -1 && !strcmp(parseArray[i]->symbolName, "COLON")) {
            colon = i;
        }
    }
    // printf("%d %d %d\n", low_var, high_var, colon);
    if (!strcmp(parseArray[colon + 1]->symbolName, "INTEGER") || !strcmp(parseArray[colon + 1]->symbolName, "BOOLEAN") || !strcmp(parseArray[colon + 1]->symbolName, "REAL")) {
        // printf("Mai to ithhe paaji\n");
        for (int i = low_var; i <= high_var; i++) {
            typeExpressionTable* temp = (typeExpressionTable*)malloc(sizeof(typeExpressionTable));
            temp->name = (char*)malloc(1 + sizeof(char) * strlen(parseArray[i]->lexeme));
            strcpy(temp->name, parseArray[i]->lexeme);
            temp->type = primitive;
            temp->array_type = NA;
            temp->exp = (expression*)malloc(sizeof(expression));
            temp->exp->a = (prim*)malloc(sizeof(prim));
            temp->exp->a->basicElementType = (char*)malloc(1 + sizeof(char) * strlen(parseArray[colon + 1]->symbolName));
            temp->next = NULL;
            strcpy(temp->exp->a->basicElementType, parseArray[colon + 1]->symbolName);
            if (*T == NULL)
                *T = temp;
            else {
                typeExpressionTable* l = *T;
                while (l->next) l = l->next;
                l->next = temp;
            }
        }
    } else if (!strcmp(parseArray[colon + 1]->symbolName, "ARRAY")) {
        for (int i = low_var; i <= high_var; i++) {
            typeExpressionTable* temp = (typeExpressionTable*)malloc(sizeof(typeExpressionTable));
            temp->name = (char*)malloc(1 + sizeof(char) * strlen(parseArray[i]->lexeme));
            strcpy(temp->name, parseArray[i]->lexeme);
            temp->type = rect_array;
            temp->next = NULL;
            temp->exp = (expression*)malloc(sizeof(expression));
            temp->exp->b = (rect*)malloc(sizeof(rect));
            temp->exp->b->basicElementType = (char*)malloc(sizeof("INTEGER") + 1);
            temp->exp->b->d = NULL;
            strcpy(temp->exp->b->basicElementType, "INTEGER");
            bool lstat = true;
            int dim = 0;
            for (int i = colon + 1; i < parseIndex; i++) {
                if (!strcmp(parseArray[i]->symbolName, "LSQUARE")) {
                    rect_dimension* rd = (rect_dimension*)malloc(sizeof(rect_dimension));
                    dim++;
                    // if ((!strcmp(parseArray[i+1]->symbolName, "VAR") || !strcmp(parseArray[i+1]->symbolName, "NUMBER")) && (!strcmp(parseArray[i+3]->symbolName, "VAR") || !strcmp(parseArray[i+3]->symbolName, "NUMBER"))) {
                    // if (!strcmp(parseArray[i+1], "VAR") || !strcmp(parseArray[i+3], "VAR")) stat = false;
                    if (!strcmp(parseArray[i + 1]->symbolName, "VAR"))
                        lstat = false;
                    rd->low = (char*)malloc(1 + sizeof(char) * strlen(parseArray[i + 1]->lexeme));
                    strcpy(rd->low, parseArray[i + 1]->lexeme);

                    if (!strcmp(parseArray[i + 3]->symbolName, "VAR"))
                        lstat = false;
                    rd->high = (char*)malloc(1 + sizeof(char) * strlen(parseArray[i + 3]->lexeme));
                    strcpy(rd->high, parseArray[i + 3]->lexeme);
                    rd->next = NULL;
                    if (temp->exp->b->d == NULL)
                        temp->exp->b->d = rd;
                    else {
                        rect_dimension* l = temp->exp->b->d;
                        while (l->next) l = l->next;
                        l->next = rd;
                    }
                    // }
                }
            }
            if (lstat)
                temp->array_type = stat;
            else
                temp->array_type = dyn;
            temp->exp->b->dimensions = dim;
            if (*T == NULL)
                *T = temp;
            else {
                typeExpressionTable* l = *T;
                while (l->next) l = l->next;
                l->next = temp;
            }
        }
    } else if (!strcmp(parseArray[colon + 1]->symbolName, "JAGGED")) {
        int colon_loc = colon;
        for (int i = low_var; i <= high_var; i++, colon = colon_loc) {
            typeExpressionTable* temp = (typeExpressionTable*)malloc(sizeof(typeExpressionTable));
            temp->name = (char*)malloc(1 + sizeof(char) * strlen(parseArray[i]->lexeme));
            strcpy(temp->name, parseArray[i]->lexeme);
            temp->type = jagged_array;
            temp->array_type = NA;
            temp->next = NULL;
            temp->exp = (expression*)malloc(sizeof(expression));
            temp->exp->c = (jagged*)malloc(sizeof(jagged));
            temp->exp->c->basicElementType = (char*)malloc(sizeof("INTEGER") + 1);
            strcpy(temp->exp->c->basicElementType, "INTEGER");
            temp->exp->c->d = NULL;
            int low, high, dim;
            // printf("Yaha pahunch gayi mai1\n");
            if (!strcmp(parseArray[colon + 2]->symbolName, "ARRAY") &&
                !strcmp(parseArray[colon + 3]->symbolName, "LSQUARE") &&
                !strcmp(parseArray[colon + 4]->symbolName, "NUMBER") &&
                !strcmp(parseArray[colon + 5]->symbolName, "DOT") &&
                !strcmp(parseArray[colon + 6]->symbolName, "NUMBER") &&
                !strcmp(parseArray[colon + 7]->symbolName, "RSQUARE") &&
                !strcmp(parseArray[colon + 8]->symbolName, "LSQUARE") &&
                !strcmp(parseArray[colon + 9]->symbolName, "RSQUARE")) {
                if (!strcmp(parseArray[colon + 10]->symbolName, "LSQUARE") && !strcmp(parseArray[colon + 11]->symbolName, "RSQUARE"))
                    temp->exp->c->dimensions = 3;
                else
                    temp->exp->c->dimensions = 2;
                // printf("Yaha pahunch gayi mai2\n");
                temp->exp->c->low = atoi(parseArray[colon + 4]->lexeme);
                temp->exp->c->high = atoi(parseArray[colon + 6]->lexeme);
                low = atoi(parseArray[colon + 4]->lexeme);
                high = atoi(parseArray[colon + 6]->lexeme);
                dim = temp->exp->c->dimensions;
            } else {  // should not occur (not accepted by grammar)
                printf("Type Error Occurred\n");
                return;
            }

            for (; colon < parseIndex; colon++) {
                if (strcmp(parseArray[colon]->symbolName, "R1")) {  // if not R1
                    continue;
                } else {
                    jagged_dimension* jd = (jagged_dimension*)malloc(sizeof(jagged_dimension));
                    jd->parent = atoi(parseArray[colon+2]->lexeme);
                    if (jd->parent < low) {
                        printf("\nType Expression Error, Line Number: %3d, Statement Type: Declaration, ", getLineNumber(*t));
                        printf("Variables:");
                        for(int z = low_var; z <= high_var; z++){
                            printf(" %s", parseArray[z]->lexeme);
                        }
                        printf(", Depth in Parse Tree: %d", parseArray[colon]->depth);
                        printf(", Message: %d is lesser than %d, lower dimension of dimension 2\n\n", jd->parent, low); 
                        return;
                    }
                    if (jd->parent > high) {
                        printf("\nType Expression Error, Line Number: %3d, Statement Type: Declaration, ", getLineNumber(*t));
                        printf("Variables:");
                        for(int z = low_var; z <= high_var; z++){
                            printf(" %s", parseArray[z]->lexeme);
                        }
                        printf(", Depth in Parse Tree: %d", parseArray[colon]->depth);
                        printf(", Message: %d is higher than %d, higher dimension of dimension 2\n\n", jd->parent, high); 
                        return;
                    }
                    jd->size = atoi(parseArray[colon + 6]->lexeme);
                    jd->inner_size = NULL;
                    jd->next = NULL;
                    int loop = jd->size;
                    colon += 10;
                    if (dim == 2){
                        while(--loop){
                            if (strcmp(parseArray[colon]->symbolName, "NUMBER") || strcmp(parseArray[colon+1]->symbolName, "SEMICOLON")) {
                                printf("\nType Expression Error, Line Number: %3d, Statement Type: Declaration, ", getLineNumber(*t));
                                printf("Variables:");
                                for(int z = low_var; z <= high_var; z++){
                                    printf(" %s", parseArray[z]->lexeme);
                                }
                                printf(", Depth in Parse Tree: %d", parseArray[colon]->depth);
                                if (!strcmp(parseArray[colon+1]->symbolName, "RCURLY"))
                                    printf(", Message: Size of 2nd dimension and number of elements mismatch\n\n");
                                else
                                    printf(", Message: Invaild 2 dimensional declaration, only one element allowed\n\n");
                                return;
                            }
                            colon += 2;
                        }
                        if (strcmp(parseArray[colon]->symbolName, "NUMBER") || strcmp(parseArray[colon+1]->symbolName, "RCURLY")) {
                            printf("\nType Expression Error, Line Number: %3d, Statement Type: Declaration, ", getLineNumber(*t));
                            printf("Variables:");
                            for(int z = low_var; z <= high_var; z++){
                                printf(" %s", parseArray[z]->lexeme);
                            }
                        printf(", Depth in Parse Tree: %d", parseArray[colon]->depth);
                        printf(", Message: Size of 2nd dimension and number of elements mismatch\n\n");
                            return;
                        }
                        jd->inner_size = NULL;
                    } else if (dim == 3) {
                        while (--loop) {
                            int inner_size = 0;
                            for (; strcmp(parseArray[colon]->symbolName, "SEMICOLON"); colon++) {
                                if (!strcmp(parseArray[colon]->symbolName, "RCURLY")) {
                                    printf("\nType Expression Error, Line Number: %3d, Statement Type: Declaration, ", getLineNumber(*t));
                                    printf("Variables:");
                                    for(int z = low_var; z <= high_var; z++){
                                        printf(" %s", parseArray[z]->lexeme);
                                    }
                                    printf(", Depth in Parse Tree: %d", parseArray[colon]->depth);
                                    printf(", Message: Size of 2nd dimension and number of elements mismatch\n\n"); 
                                    return;
                                }
                                if (!strcmp(parseArray[colon]->symbolName, "NUMBER"))
                                    inner_size++;
                                else {
                                    printf("\nType Expression Error, Line Number: %3d, Statement Type: Declaration, ", getLineNumber(*t));
                                    printf("Variables:");
                                    for(int z = low_var; z <= high_var; z++){
                                        printf(" %s", parseArray[z]->lexeme);
                                    }
                                    printf(", Depth in Parse Tree: %d", parseArray[colon]->depth);
                                    printf(", Message: Invalid 3D declaration\n\n");
                                    return;
                                }
                            }
                            colon++;
                            dimension* d = (dimension*)malloc(sizeof(dimension));
                            d->size = inner_size;
                            d->next = NULL;
                            if (jd->inner_size == NULL)
                                jd->inner_size = d;
                            else {
                                dimension* l = jd->inner_size;
                                while (l->next) l = l->next;
                                l->next = d;
                            }
                        }
                        int inner_size = 0;
                        for (; strcmp(parseArray[colon]->symbolName, "RCURLY"); colon++) {
                            if (!strcmp(parseArray[colon]->symbolName, "NUMBER"))
                                inner_size++;
                            else {
                                printf("\nType Expression Error, Line Number: %3d, Statement Type: Declaration, ", getLineNumber(*t));
                                printf("Variables:");
                                for(int z = low_var; z <= high_var; z++){
                                    printf(" %s", parseArray[z]->lexeme);
                                }
                                printf(", Depth in Parse Tree: %d", parseArray[colon]->depth);
                                printf(", Message: Size of 2nd dimension and number of elements mismatch\n\n");
                                return;
                            }
                        }
                        dimension* d = (dimension*)malloc(sizeof(dimension));
                        d->size = inner_size;
                        d->next = NULL;
                        if (jd->inner_size == NULL)
                            jd->inner_size = d;
                        else {
                            dimension* l = jd->inner_size;
                            while (l->next) l = l->next;
                            l->next = d;
                        }
                    }
                    else {
                        printf("\nType Expression Error, Line Number: %3d, Statement Type: Declaration, ", getLineNumber(*t));
                        printf("Variables:");
                        for(int z = low_var; z <= high_var; z++){
                            printf(" %s", parseArray[z]->lexeme);
                        }
                        printf(", Depth in Parse Tree: %d", parseArray[colon]->depth);
                        printf(", Message: Jagged Array out of bounds\n\n");
                        return;
                    }
                    if (temp->exp->c->d == NULL)
                        temp->exp->c->d = jd;
                    else {
                        jagged_dimension* l = temp->exp->c->d;
                        while (l->next) l = l->next;
                        l->next = jd;
                    }
                }
            }
            temp->exp->b->dimensions = dim;
            if (*T == NULL)
                *T = temp;
            else {
                typeExpressionTable* l = *T;
                while (l->next) l = l->next;
                l->next = temp;
            }
        }
    }
    else {
        printf("\nType Expression Error, Line Number: %3d, Statement Type: Declaration, ", getLineNumber(*t));
        printf(", Depth in Parse Tree: %d", parseArray[colon]->depth);
        printf(", Message: Traversal Error Occurred\n\n");
        return;
    }
    return;
}

typeExpressionTable* getExpression(parseTree* input, typeExpressionTable* table) {
    // printf("Inside getExpression\n\n\n\n");
    if (!strcmp(input->symbolName, "lhs")) {
        if (!strcmp(input->children[0]->symbolName, "VAR")) {
            if (input->children[1] == NULL) {
                typeExpressionTable* temp = table;
                while (temp && strcmp(temp->name, input->children[0]->lexeme)) {
                    temp = temp->next;
                }
                return temp;
            } else if (!strcmp(input->children[1]->symbolName, "LSQUARE")) {
                //do this
                int numNums = 0;
                // while(input->children[2]->children[numNums]!=NULL) numNums++;
                parseTree* rec = input->children[2];
                while (rec) {
                    numNums++;
                    rec = rec->children[1];
                }
                rec = input->children[2];
                typeExpressionTable* temp = table;
                while (temp && strcmp(temp->name, input->children[0]->lexeme)) {
                    temp = temp->next;
                }
                if (temp->type == jagged_array) {
                    if (numNums != temp->exp->c->dimensions)
                        return NULL;
                    jagged* dim_details = temp->exp->c;
                    int dimensions = temp->exp->c->dimensions;
                    if (dimensions == 2) {
                        int low_1 = dim_details->low;
                        int high_1 = dim_details->high;
                        int s1 = atoi(rec->children[0]->lexeme);
                        int s2 = atoi(rec->children[1]->children[0]->lexeme);
                        if (s1 < low_1 || s1 > high_1)
                            return NULL;
                        jagged_dimension* inner_details = dim_details->d;
                        while (inner_details && s1 != inner_details->parent) {
                            inner_details = inner_details->next;
                            // s1--;
                        }
                        int high_2 = inner_details->size;
                        if (s2 >= high_2)
                            return NULL;

                        typeExpressionTable* temp_ = (typeExpressionTable*)malloc(sizeof(typeExpressionTable));
                        temp_->name = (char*)malloc(sizeof(char) * strlen("non_terminal"));
                        strcpy(temp_->name, "non_terminal");
                        temp_->type = primitive;
                        temp_->next = NULL;
                        temp_->exp = (expression*)malloc(sizeof(expression));
                        temp_->exp->a = (prim*)malloc(sizeof(prim));
                        temp_->exp->a->basicElementType = (char*)malloc(sizeof(char) * strlen("INTEGER"));
                        strcpy(temp_->exp->a->basicElementType, "INTEGER");
                        return temp_;
                    } else if (dimensions == 3) {
                        int low_1 = dim_details->low;
                        int high_1 = dim_details->high;
                        int s1 = atoi(rec->children[0]->lexeme);
                        int s2 = atoi(rec->children[1]->children[0]->lexeme);
                        int s3 = atoi(rec->children[1]->children[1]->children[0]->lexeme);
                        if (s1 < low_1 || s1 > high_1)
                            return NULL;
                        jagged_dimension* inner_details = dim_details->d;
                        while (s1 > low_1) {
                            inner_details = inner_details->next;
                            s1--;
                        }
                        int high_2 = inner_details->size;
                        if (s2 >= high_2)
                            return NULL;
                        dimension* inner_inner_details = inner_details->inner_size;
                        int high_3 = 1;
                        while (inner_inner_details->next != NULL) {
                            high_3++;
                            inner_inner_details = inner_inner_details->next;
                        }
                        if (s3 >= high_3)
                            return NULL;

                        typeExpressionTable* temp_ = (typeExpressionTable*)malloc(sizeof(typeExpressionTable));
                        temp_->name = (char*)malloc(sizeof(char) * strlen("non_terminal"));
                        strcpy(temp_->name, "non_terminal");
                        temp_->next = NULL;
                        temp_->type = primitive;
                        temp_->exp = (expression*)malloc(sizeof(expression));
                        temp_->exp->a = (prim*)malloc(sizeof(prim));
                        temp_->exp->a->basicElementType = (char*)malloc(sizeof(char) * strlen("INTEGER"));
                        strcpy(temp_->exp->a->basicElementType, "INTEGER");
                        return temp_;
                    }
                } else if (temp->type == rect_array) {
                    if (temp->array_type == dyn) {
                        if (numNums != temp->exp->b->dimensions)
                            return NULL;
                        else {
                            typeExpressionTable* temp_ = (typeExpressionTable*)malloc(sizeof(typeExpressionTable));
                            temp_->name = (char*)malloc(sizeof(char) * strlen("non_terminal"));
                            strcpy(temp_->name, "non_terminal");
                            temp_->next = NULL;
                            temp_->type = primitive;
                            temp_->exp = (expression*)malloc(sizeof(expression));
                            temp_->exp->a = (prim*)malloc(sizeof(prim));
                            temp_->exp->a->basicElementType = (char*)malloc(sizeof(char) * strlen("INTEGER"));
                            strcpy(temp_->exp->a->basicElementType, "INTEGER");
                            return temp_;
                        }
                    }
                    if (numNums != temp->exp->b->dimensions)
                        return NULL;
                    rect_dimension* temp_rect = temp->exp->b->d;
                    while (temp_rect != NULL) {
                        int dim_ = atoi(rec->children[0]->symbolName);
                        int high = atoi(temp_rect->high);
                        int low = atoi(temp_rect->low);
                        if (dim_ < low || dim_ > high)
                            return NULL;
                        temp_rect = temp_rect->next;
                        rec = rec->children[1];
                    }
                }

            } else {
                printf("This should not have occured lhs !->var lsq || var \n");
                return NULL;
            }
        } else {
            printf("This should not have occured lhs !-> var\n");
            return NULL;
        }
    } else if (!strcmp(input->symbolName, "factor")) {
        if (!strcmp(input->children[0]->symbolName, "VAR")) {
            //do this (same as above)
            if (input->children[1] == NULL) {
                typeExpressionTable* temp = table;
                while (temp && strcmp(temp->name, input->children[0]->lexeme)) {
                    temp = temp->next;
                }
                return temp;
            } else if (!strcmp(input->children[1]->symbolName, "LSQUARE")) {
                //do this
                int numNums = 0;
                // while(input->children[2]->children[numNums]!=NULL) numNums++;
                parseTree* rec = input->children[2];
                while (rec) {
                    numNums++;
                    rec = rec->children[1];
                }
                rec = input->children[2];
                typeExpressionTable* temp = table;
                while (temp && strcmp(temp->name, input->children[0]->lexeme)) {
                    temp = temp->next;
                }
                if (temp->type == jagged_array) {
                    if (numNums != temp->exp->c->dimensions)
                        return NULL;
                    jagged* dim_details = temp->exp->c;
                    int dimensions = temp->exp->c->dimensions;
                    if (dimensions == 2) {
                        int low_1 = dim_details->low;
                        int high_1 = dim_details->high;
                        int s1 = atoi(rec->children[0]->lexeme);
                        int s2 = atoi(rec->children[1]->children[0]->lexeme);
                        if (s1 < low_1 || s1 > high_1)
                            return NULL;
                        jagged_dimension* inner_details = dim_details->d;
                        while (s1 > low_1) {
                            inner_details = inner_details->next;
                            s1--;
                        }
                        int high_2 = inner_details->size;
                        if (s2 >= high_2)
                            return NULL;

                        typeExpressionTable* temp_ = (typeExpressionTable*)malloc(sizeof(typeExpressionTable));
                        temp_->name = (char*)malloc(sizeof(char) * strlen("non_terminal"));
                        strcpy(temp_->name, "non_terminal");
                        temp_->next = NULL;
                        temp_->type = primitive;
                        temp_->exp = (expression*)malloc(sizeof(expression));
                        temp_->exp->a = (prim*)malloc(sizeof(prim));
                        temp_->exp->a->basicElementType = (char*)malloc(sizeof(char) * strlen("INTEGER"));
                        strcpy(temp_->exp->a->basicElementType, "INTEGER");
                        return temp_;
                    } else if (dimensions == 3) {
                        int low_1 = dim_details->low;
                        int high_1 = dim_details->high;
                        int s1 = atoi(rec->children[0]->lexeme);
                        int s2 = atoi(rec->children[1]->children[0]->lexeme);
                        int s3 = atoi(rec->children[1]->children[1]->children[0]->lexeme);
                        if (s1 < low_1 || s1 > high_1)
                            return NULL;
                        jagged_dimension* inner_details = dim_details->d;
                        while (s1 > low_1) {
                            inner_details = inner_details->next;
                            s1--;
                        }
                        int high_2 = inner_details->size;
                        if (s2 >= high_2)
                            return NULL;
                        dimension* inner_inner_details = inner_details->inner_size;
                        int high_3 = 1;
                        while (inner_inner_details->next != NULL) {
                            high_3++;
                            inner_inner_details = inner_inner_details->next;
                        }
                        if (s3 >= high_3)
                            return NULL;

                        typeExpressionTable* temp_ = (typeExpressionTable*)malloc(sizeof(typeExpressionTable));
                        temp_->name = (char*)malloc(sizeof(char) * strlen("non_terminal"));
                        strcpy(temp_->name, "non_terminal");
                        temp_->next = NULL;
                        temp_->type = primitive;
                        temp_->exp = (expression*)malloc(sizeof(expression));
                        temp_->exp->a = (prim*)malloc(sizeof(prim));
                        temp_->exp->a->basicElementType = (char*)malloc(sizeof(char) * strlen("INTEGER"));
                        strcpy(temp_->exp->a->basicElementType, "INTEGER");
                        return temp_;
                    }
                } else if (temp->type == rect_array) {
                    if (temp->array_type == dyn) {
                        if (numNums != temp->exp->b->dimensions)
                            return NULL;
                        else {
                            typeExpressionTable* temp_ = (typeExpressionTable*)malloc(sizeof(typeExpressionTable));
                            temp_->name = (char*)malloc(sizeof(char) * strlen("non_terminal"));
                            strcpy(temp_->name, "non_terminal");
                            temp_->next = NULL;
                            temp_->type = primitive;
                            temp_->exp = (expression*)malloc(sizeof(expression));
                            temp_->exp->a = (prim*)malloc(sizeof(prim));
                            temp_->exp->a->basicElementType = (char*)malloc(sizeof(char) * strlen("INTEGER"));
                            strcpy(temp_->exp->a->basicElementType, "INTEGER");
                            return temp_;
                        }
                    }
                    if (numNums != temp->exp->b->dimensions)
                        return NULL;
                    rect_dimension* temp_rect = temp->exp->b->d;
                    while (temp_rect != NULL) {
                        int dim_ = atoi(rec->children[0]->symbolName);
                        int high = atoi(temp_rect->high);
                        int low = atoi(temp_rect->low);
                        if (dim_ < low || dim_ > high)
                            return NULL;
                        temp_rect = temp_rect->next;
                        rec = rec->children[1];
                    }
                }
            }
        } else if (!strcmp(input->children[0]->symbolName, "id")) {
            // printf("Inside factor id\n\n\n\n");
            if (!strcmp(input->children[0]->children[0]->symbolName, "VAR")) {
                typeExpressionTable* temp = table;
                while (temp && strcmp(temp->name, input->children[0]->children[0]->lexeme)) {
                    temp = temp->next;
                }
                return temp;
            } else if (!strcmp(input->children[0]->children[0]->symbolName, "NUMBER")) {
                typeExpressionTable* temp = (typeExpressionTable*)malloc(sizeof(typeExpressionTable));
                temp->name = (char*)malloc(sizeof(char) * strlen("non_terminal"));
                strcpy(temp->name, "non_terminal");
                temp->next = NULL;
                temp->type = primitive;
                temp->exp = (expression*)malloc(sizeof(expression));
                temp->exp->a = (prim*)malloc(sizeof(prim));
                temp->exp->a->basicElementType = (char*)malloc(sizeof(char) * strlen("INTEGER"));
                strcpy(temp->exp->a->basicElementType, "INTEGER");
                return temp;
            } else {
                printf("This should not have occured id!->VAR || NUMBER");
                return NULL;
            }
        } else {
            printf("This should not have occured factor !-> VAR || id\n");
            return NULL;
        }
    }
}

int compare(typeExpressionTable* a, typeExpressionTable* b, int op) {
    if (op == 1 && a->type == primitive && !strcmp(a->exp->a->basicElementType, "BOOLEAN") &&
        b->type == primitive && !strcmp(b->exp->a->basicElementType, "BOOLEAN")) {
        return 1;
    } else if (op == 2) {
        if ((a->type == primitive && !strcmp(a->exp->a->basicElementType, "INTEGER") &&
             b->type == primitive && !strcmp(b->exp->a->basicElementType, "INTEGER")) ||
            (a->type == primitive && !strcmp(a->exp->a->basicElementType, "REAL") &&
             b->type == primitive && !strcmp(b->exp->a->basicElementType, "REAL")))
            return 1;
        else if (a->type == rect_array && b->type == rect_array) {
            bool ans = true;
            if (a->exp->b->dimensions != b->exp->b->dimensions)
                ans = false;
            else {
                rect_dimension* t1 = a->exp->b->d;
                rect_dimension* t2 = b->exp->b->d;
                while (t1 && t2) {
                    if (strcmp(t1->low, t2->low) || strcmp(t1->high, t2->high)) ans = false;
                    t1 = t1->next;
                    t2 = t2->next;
                }
            }
            return ans;
        } else if (a->type == jagged_array && b->type == jagged_array) {
            bool ans = true;
            if (a->exp->c->dimensions != b->exp->c->dimensions || a->exp->c->low != b->exp->c->low || a->exp->c->high != b->exp->c->high)
                ans = false;
            else {
                jagged_dimension* t1 = a->exp->c->d;
                jagged_dimension* t2 = b->exp->c->d;
                while (t1 && t2) {
                    if (t1->parent != t2->parent || t1->size != t2->size) ans = false;
                    dimension* dt1 = t1->inner_size;
                    dimension* dt2 = t2->inner_size;
                    while (dt1 && dt2) {
                        if (dt1->size != dt2->size) ans = false;
                        dt1 = dt1->next;
                        dt2 = dt2->next;
                    }
                    t1 = t1->next;
                    t2 = t2->next;
                }
            }
            return ans;
        } else
            return 0;
    } else if (op == 3) {
        if ((a->type == primitive && !strcmp(a->exp->a->basicElementType, "INTEGER") &&
             b->type == primitive && !strcmp(b->exp->a->basicElementType, "INTEGER")) ||
            (a->type == primitive && !strcmp(a->exp->a->basicElementType, "REAL") &&
             b->type == primitive && !strcmp(b->exp->a->basicElementType, "REAL"))) {
            return 1;
        } else
            return 0;
    } else {
        return 0;
    }
}

void addAssignment(parseTree** t, typeExpressionTable* T) {
    // printf("Inside addAssignment\n%s\n%s\n\n", (*t)->symbolName, (*t)->lexeme);
    // printParseTree(*t);
    int op = -1;
    if (!strcmp((*t)->symbolName, "assignment")) {
        (*t)->children[0]->type = getExpression((*t)->children[0], T);
        addAssignment(&(*t)->children[2], T);
        op = 4;
        //compare
    } else if (!strcmp((*t)->symbolName, "factor") || !strcmp((*t)->symbolName, "lhs")) {
        (*t)->type = getExpression(*t, T);
        return;
    } else {
        if ((*t)->children[1] == NULL) {
            addAssignment(&(*t)->children[0], T);
            (*t)->type = (*t)->children[0]->type;
            return;
        }
        if (!strcmp((*t)->children[1]->symbolName, "OP_OR") || !strcmp((*t)->children[1]->symbolName, "OP_AND"))
            op = 1;  //boolean
        else if (!strcmp((*t)->children[1]->children[0]->symbolName, "OP_PLUS") || !strcmp((*t)->children[1]->children[0]->symbolName, "OP_MINUS") || !strcmp((*t)->children[1]->children[0]->symbolName, "OP_MULT"))
            op = 2;  //integer
        else if (!strcmp((*t)->children[1]->children[0]->symbolName, "OP_DIV"))
            op = 3;  //division
        if (!strcmp((*t)->children[0]->symbolName, "factor")) {
            (*t)->children[0]->type = getExpression((*t)->children[0], T);
        } else {
            addAssignment(&(*t)->children[0], T);
        }
        if (!strcmp((*t)->children[2]->symbolName, "factor")) {
            (*t)->children[2]->type = getExpression((*t)->children[2], T);
        } else {
            addAssignment(&(*t)->children[2], T);
        }
    }
    if ((*t)->children[0]->type == NULL || (*t)->children[2]->type == NULL) {
        (*t)->type = NULL;
        // printf("Error aa gayi %s %d\n", (*t)->symbolName, (*t)->lineNumber);
    } else {
        if (op == 1) {
            // if ((*t)->children[0]->type->type == primitive && !strcmp((*t)->children[0]->type->exp->a->basicElementType,"BOOLEAN") &&
            //     (*t)->children[2]->type->type == primitive && !strcmp((*t)->children[2]->type->exp->a->basicElementType,"BOOLEAN")) {
            if (compare((*t)->children[0]->type, (*t)->children[2]->type, op)) {
                (*t)->type = (*t)->children[0]->type;
            } else {
                (*t)->type = NULL;
                printf("\nType Expression Error, Line Number: %3d, Statement Type: Assignment, ", getLineNumber(*t));
                printf("Operator: %3s, ", (*t)->children[1]->lexeme);
                printf("Lexeme of First Operand: %5s, Type of First Operand: ", (*t)->children[0]->type->name);
                printTypeExpressionTable((*t)->children[0]->type);
                printf(", Lexeme of Second Operand: %5s, Type of Second Operand: ", (*t)->children[2]->type->name);
                printTypeExpressionTable((*t)->children[2]->type);
                printf(", Depth in Parse Tree: %d", (*t)->depth);
                printf(", Message: Boolean Operation Type Mismatch\n\n"); 
                // printf("Error aa gayi 1 %s %d\n", (*t)->symbolName, (*t)->lineNumber);
            }
        } else if (op == 2) {
            if (compare((*t)->children[0]->type, (*t)->children[2]->type, op)) {
                (*t)->type = (*t)->children[0]->type;
            } else {
                (*t)->type = NULL;
                // printTypeExpressionTable((*t)->children[0]->type);
                // printTypeExpressionTable((*t)->children[2]->type);
                printf("\nType Expression Error, Line Number: %3d, Statement Type: Assignment, ", getLineNumber(*t));
                printf("Operator: %3s, ", (*t)->children[1]->children[0]->lexeme);
                printf("Lexeme of First Operand: %5s, Type of First Operand: ", (*t)->children[0]->type->name);
                printTypeExpressionTable((*t)->children[0]->type);
                printf(", Lexeme of Second Operand: %5s, Type of Second Operand: ", (*t)->children[2]->type->name);
                printTypeExpressionTable((*t)->children[2]->type);
                printf(", Depth in Parse Tree: %d", (*t)->depth);
                if (!strcmp((*t)->children[1]->children[0]->symbolName, "OP_PLUS"))
                    printf(", Message: Addition Operation Type Mismatch\n\n");
                else if (!strcmp((*t)->children[1]->children[0]->symbolName, "OP_MINUS"))
                    printf(", Message: Subtraction Operation Type Mismatch\n\n");
                else if (!strcmp((*t)->children[1]->children[0]->symbolName, "OP_MULT"))
                    printf(", Message: Multiplication Operation Type Mismatch\n\n");
            }
        } else if (op == 3) {
            if (compare((*t)->children[0]->type, (*t)->children[2]->type, op)) {
                typeExpressionTable* temp = (typeExpressionTable*)malloc(sizeof(typeExpressionTable));
                temp->name = (char*)malloc(1 + sizeof(char) * strlen("non-terminal"));
                strcpy(temp->name, "non-terminal");
                temp->type = primitive;
                temp->array_type = NA;
                temp->exp = (expression*)malloc(sizeof(expression));
                temp->exp->a = (prim*)malloc(sizeof(prim));
                temp->exp->a->basicElementType = (char*)malloc(1 + sizeof(char) * strlen("REAL"));
                temp->next = NULL;
                strcpy(temp->exp->a->basicElementType, "REAL");
                (*t)->type = temp;
            } else {
                (*t)->type = NULL;
                
                printf("\nType Expression Error, Line Number: %3d, Statement Type: Assignment, ", getLineNumber(*t));
                printf("Operator: %3s, ", (*t)->children[1]->children[0]->lexeme);
                printf("Lexeme of First Operand: %5s, Type of First Operand: ", (*t)->children[0]->type->name);
                printTypeExpressionTable((*t)->children[0]->type);
                printf(", Lexeme of Second Operand: %5s, Type of Second Operand: ", (*t)->children[2]->type->name);
                printTypeExpressionTable((*t)->children[2]->type);
                printf(", Depth in Parse Tree: %d", (*t)->depth);
                printf(", Message: Division Operation Type Mismatch\n\n"); 
            }
        } else if (op == 4) {
            if (compare((*t)->children[0]->type, (*t)->children[2]->type, 1) || compare((*t)->children[0]->type, (*t)->children[2]->type, 2))
                (*t)->type = (*t)->children[0]->type;
            else {
                (*t)->type = NULL;
                // printTypeExpressionTable((*t)->children[0]->type);
                // printTypeExpressionTable((*t)->children[2]->type);
                printf("\nType Expression Error, Line Number: %3d, Statement Type: Assignment, ", getLineNumber(*t));
                printf("Operator: %3s, ", (*t)->children[1]->lexeme);
                printf("Lexeme of First Operand: %5s, Type of First Operand: ", (*t)->children[0]->type->name);
                printTypeExpressionTable((*t)->children[0]->type);
                printf(", Lexeme of Second Operand: %5s, Type of Second Operand: ", (*t)->children[2]->type->name);
                printTypeExpressionTable((*t)->children[2]->type);
                printf(", Depth in Parse Tree: %d", (*t)->depth);
                printf(", Message: Assignment Type Mismatch\n\n"); 
            }
        }
    }
}

// Function Definition: traverseParseTree (parseTree *t, typeExpressionTable T)
int traverseParseTree(parseTree** t, typeExpressionTable** T) {
    // printf ("Inside traverseParseTree.\n");
    if (t != NULL && !strcmp((*t)->symbolName, "declaration")) {
        addDeclaration(t, T);
        return 0;
    }
    if (t != NULL && !strcmp((*t)->symbolName, "assignment")) {
        addAssignment(t, *T);
        return 0;
    }
    for (int i = 0; i < MAX_PARSE_TREE_CHILDREN; i++) {
        if ((*t)->children[i])
            traverseParseTree(&((*t)->children[i]), T);
    }
    return 0;
}
