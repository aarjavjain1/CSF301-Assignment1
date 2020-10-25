/*
This file implements all the necessary functions for the compiler to work.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "customcompiler.h"
#define MAX_LEN 1000
// Function Definition: readGrammar( “grammar.txt”, grammar G)
int readGrammar (char* grammarFilePath,  grammarNode** G){
    printf ("Inside readGrammar.\n");
    FILE* fp;
    char* token;
    fp = fopen(grammarFilePath, "r");
    if (fp == NULL) {
      perror("Failed: ");
      return 1;
    }

    char buffer[MAX_LEN];
    // -1 to allow room for NULL terminator for really long string
    int line_num = 0;
    while (fgets(buffer, MAX_LEN - 1, fp))
    {
        // Remove trailing newline
        buffer[strcspn(buffer, "\n")] = 0;
        token = strtok(buffer, " ");
        grammarNode * temp = (grammarNode*)malloc(sizeof(grammarNode));
        grammarNode * head = temp;
        strcpy(temp->grammarWord,token);
        token = strtok(NULL, " ");
       /* walk through other tokens */
        while( token != NULL) {
            if (strcmp(token,"->") == 0){
                token = strtok(NULL," ");
                continue;
            }
            // printf( "%s\n", token );
            temp->next = (grammarNode*)malloc(sizeof(grammarNode));
            temp = temp->next;
            strcpy(temp->grammarWord,token);
            // printf("HERE");
            token = strtok(NULL, " ");
        }
        // printf("%s\n", buffer);
        G[line_num++] = head;
        // printf("GRAMMARWORD %s\n",G[line_num++]->grammarWord);
    }
    fclose(fp);
    // for(int i = 0; i< NUMBER_OF_GRAMMAR_RULES; i++){
    //     // printf("%d",i);
    //     grammarNode* t = G[i];
    //     while (t!= NULL){
    //       printf("%s\t",t->grammarWord);
    //       t = t->next;
    //     }
    //     printf("\n");
    // }
    return 0;
}

// Function Definition: tokeniseSourcecode(  “sourcecode.txt”,  tokenStream  *s)
int tokeniseSourcecode (char* sourceCodeFilePath,  tokenStream  *s){
    printf ("Inside tokeniseSourcecode.\n");
    FILE* fp;
    char* token;
    fp = fopen(sourceCodeFilePath, "r");
    if (fp == NULL) {
      perror("Failed: ");
      return 1;
    }

    char buffer[MAX_LEN];
    // -1 to allow room for NULL terminator for really long string
    int line_num = 0;
    while (fgets(buffer, MAX_LEN - 1, fp))
    {
        // Remove trailing newline
        buffer[strcspn(buffer, "\n")] = 0;
        token = strtok(buffer, " ");
        tokenStream * temp = (tokenStream*)malloc(sizeof(tokenStream));
        tokenStream * head = temp;
        strcpy(temp->tokenName,token);
        temp->lineNumber = line_num;
        // strcpy(temp->lexeme,stringtoenum(token));
        token = strtok(NULL, " ");
       /* walk through other tokens */
        while( token != NULL) {
            // printf( "%s\n", token );
            temp->next = (tokenStream*)malloc(sizeof(tokenStream));
            temp = temp->next;
            strcpy(temp->tokenName,token);
            // strcpy(temp->lexeme,stringtoenum(token));
            temp->lineNumber = line_num;
            // printf("HERE");
            token = strtok(NULL, " ");
        }
        // printf("%s\n", buffer);
        // s[line_num++] = head;
        // printf("GRAMMARWORD %s\n",G[line_num++]->grammarWord);
        line_num++;
    }
    fclose(fp);
    // for(int i = 0; i< NUMBER_OF_GRAMMAR_RULES; i++){
    //     // printf("%d",i);
    //     grammarNode* t = G[i];
    //     while (t!= NULL){
    //       printf("%s\t",t->grammarWord);
    //       t = t->next;
    //     }
    //     printf("\n");
    // }
    return 0;
}

// Function Definition: createParseTree (parseTree  *t,  tokenStream  *s,  grammar  G)
int createParseTree (parseTree  *t,  tokenStream  *s,  grammarNode*  G){
    // initiate stack
    // fill start in stack
    // while loop 
    //  check if terminal
    //      pop from stack and add to parse tree
    // check if non terminal
    //      pop from stack and add to parse tree and push rhs of grammar rule to stack
    //

    stack st;
    stack_push(st, PROGRAM);
    

    return 0;
}

// Function Definition: traverseParseTree (parseTree *t, typeExpressionTable T)
int traverseParseTree (parseTree *t, typeExpressionTable T){
    printf ("Inside traverseParseTree.\n");
    return 0;
}

// Function Definition: printParseTree (parseTree *t)
int printParseTree (parseTree *t){
    printf ("Inside printPareseTree.\n");
    return 0;
}

// Function Definition: printTypeExpressionTable (typeExpressionTable T)
int printTypeExpressionTable (typeExpressionTable T){
    printf ("Inside printTypeExpressionTable.\n");
    return 0;
}
