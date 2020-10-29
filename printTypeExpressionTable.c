#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "datastructuresandprototypes.h"

// Function Definition: printTypeExpressionTable (typeExpressionTable T)
int printTypeExpressionTable (typeExpressionTable *T){
    // printf ("Inside printTypeExpressionTable.\n");
    // printf("Field1\t\tField2\t\tField3\t\tField4\n");
    typeExpressionTable* l = T;
    // while(l){
        // printf("%s\t", l->name);
        // if (l->type == primitive)
        //     printf("Primitive\t");
        // else if (l->type == rect_array)
        //     printf("Rectangular Array\t");
        // else if (l->type == jagged_array)
        //     printf("Jagged Array\t");
        // if (l->array_type == stat)
        //     printf("Static\t");
        // else if (l->array_type == dyn)
        //     printf("Dynamic\t");
        // else if (l->array_type == NA)
        //     printf("Not Applicable\t");
        if (l->type == primitive){
            printf("<type = %s>", l->exp->a->basicElementType);
        }
        else if (l->type == rect_array){
            printf("<type=rectangularArray, dimensions=%d, ", l->exp->b->dimensions);
            rect_dimension* temp = l->exp->b->d;
            int count = 0;
            while(temp){
                count++;
                printf("range_R%d=(%s,%s), ", count, temp->low, temp->high);
                temp = temp->next;
            }
            printf("basicElementType=INTEGER>");
        }
        else if (l->type == jagged_array){
            printf("<type=jaggedArray, dimensions=%d, range_R1=(%d,%d), range_R2=(", l->exp->c->dimensions, l->exp->c->low, l->exp->c->high);
            jagged_dimension* temp = l->exp->c->d;
            int count = 1;
            while(temp){
                count++;
                printf("%d", temp->size);
                if (temp->inner_size){
                    printf("[");
                    dimension* in = temp->inner_size;
                    while(in){
                        printf("%d", in->size);
                        if (in->next) printf(",");
                        in = in->next;
                    }
                    printf("]");
                }
                if (temp->next) printf(",");
                    temp = temp->next;
            }
            printf("), basicElementType=INTEGER>");
        }
        // printf("\n");
    return 0;
}
