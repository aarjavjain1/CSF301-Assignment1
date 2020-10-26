/*
This file implements all the necessary functions for the compiler to work.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "customcompiler.h"
#define MAX_LEN 1000
// Function Definition: readGrammar( “grammar.txt”, grammar G)

bool isVariable(char* str){
    for (int yu=0; yu <(int)strlen(str); ++yu){
        if (yu == 0){
            if (isalpha(str[yu]) || str[yu] == '_') continue;
            else return false;
        } else {
            if (isalnum(str[yu]) || str[yu] == '_') continue;
            else return false;
        }
    }
    return true;
}


bool isNumber(char* str){
    // printf("isNumber %s\n",str);
    for (int yu=0; yu <(int)strlen(str); ++yu){
        if (!isdigit(str[yu]))
            return false;
    }
    return true;
}

int search(char* token)
{
  FILE* f = fopen("keywords.txt","r");
  char buffer[50];
  while(EOF!=fscanf(f,"%[^\n]\n",buffer)){
    if(!strcmp(buffer,token))
      return 1;
  }
  fclose(f);
  return 0;
}

tokenStream* get_token(char* token,int* line_count)
{
    tokenStream *new = (tokenStream *) malloc(sizeof(tokenStream));
    new->tokenName = (char *) malloc (sizeof(char) * 100);
    if (search(token) == 1){
      // printf("NICHT\n");
      char *temp_ = (char *) malloc(strlen(token) + 1);
      for (int yu=0; yu < (int) strlen(token); ++yu)
          temp_[yu] = toupper(token[yu]);
      //printf("token: %s\n", buff);
      strcpy(new->tokenName, temp_);
    }
    else if (!strcmp(token, "R1"))
        strcpy(new->tokenName, "R1");
    else if (isVariable(token))
        strcpy(new->tokenName,"var");
    else if(isNumber(token))
        strcpy(new->tokenName,"NUMBER");
    else if(!strcmp(token,"&&&"))
        strcpy(new->tokenName,"OP_AND");
    else if(!strcmp(token,"|||"))
        strcpy(new->tokenName,"OP_OR");
    else if(!strcmp(token,".."))
        strcpy(new->tokenName,"DOT");
    else if(strlen(token)==1)
    {
        switch (token[0])
        {
            case '+':
            {
                new->tokenName = (char *) malloc (sizeof(char) * strlen("OP_PLUS"));
                new->lexeme = (char *) malloc (sizeof(char) * strlen("+"));
                strcpy(new->tokenName, "OP_PLUS");
                strcpy(new->lexeme, "+");
                new->lineNumber = *line_count;
                new->next = NULL;
                return new;
                break;
            }
            case '-':
            {
                new->tokenName = (char *) malloc (sizeof(char) * strlen("OP_MINUS"));
                new->lexeme = (char *) malloc (sizeof(char) * strlen("-"));
                strcpy(new->tokenName, "OP_MINUS");
                strcpy(new->lexeme, "-");
                new->lineNumber = *line_count;
                new->next = NULL;
                return new;
                break;
            }
            case '/':
            {
                new->tokenName = (char *) malloc (sizeof(char) * strlen("OP_DIV"));
                new->lexeme = (char *) malloc (sizeof(char) * strlen("/"));
                strcpy(new->tokenName, "OP_DIV");
                strcpy(new->lexeme, "/");
                new->lineNumber = *line_count;
                new->next = NULL;
                return new;
                break;
            }
            case '[':
            {
                new->tokenName = (char *) malloc (sizeof(char) * strlen("LSQUARE"));
                new->lexeme = (char *) malloc (sizeof(char) * strlen("["));
                strcpy(new->tokenName, "LSQUARE");
                strcpy(new->lexeme, "[");
                new->lineNumber = *line_count;
                new->next = NULL;
                return new;
                break;
            }
            case ']':
            {
                new->tokenName = (char *) malloc (sizeof(char) * strlen("RQSQUARE"));
                new->lexeme = (char *) malloc (sizeof(char) * strlen("]"));
                strcpy(new->tokenName, "RSQUARE");
                strcpy(new->lexeme, "]");
                new->lineNumber = *line_count;
                new->next = NULL;
                return new;
                break;
            }
            case '{':
            {
                new->tokenName = (char *) malloc (sizeof(char) * strlen("LCURLY"));
                new->lexeme = (char *) malloc (sizeof(char) * strlen("{"));
                strcpy(new->tokenName, "LCURLY");
                strcpy(new->lexeme, "{");
                new->lineNumber = *line_count;
                new->next = NULL;
                return new;
                break;
            }
            case '}':
            {
                new->tokenName = (char *) malloc (sizeof(char) * strlen("RCURLY"));
                new->lexeme = (char *) malloc (sizeof(char) * strlen("}"));
                strcpy(new->tokenName, "RCURLY");
                strcpy(new->lexeme, "}");
                new->lineNumber = *line_count;
                new->next = NULL;
                return new;
                break;
            }
            case ';':
            {
                new->tokenName = (char *) malloc (sizeof(char) * strlen("SEMICOLON"));
                new->lexeme = (char *) malloc (sizeof(char) * strlen(";"));
                strcpy(new->tokenName, "SEMICOLON");
                strcpy(new->lexeme, ";");
                new->lineNumber = *line_count;
                new->next = NULL;
                return new;
                break;
            }
            case ',':
            {
                new->tokenName = (char *) malloc (sizeof(char) * strlen("COMMA"));
                new->lexeme = (char *) malloc (sizeof(char) * strlen(","));
                strcpy(new->tokenName, "COMMA");
                strcpy(new->lexeme, ",");
                new->lineNumber = *line_count;
                new->next = NULL;
                return new;
                break;
            }
            case '=':
            {
                new->tokenName = (char *) malloc (sizeof(char) * strlen("EQ"));
                new->lexeme = (char *) malloc (sizeof(char) * strlen("="));
                strcpy(new->tokenName, "EQ");
                strcpy(new->lexeme, "=");
                new->lineNumber = *line_count;
                new->next = NULL;
                return new;
                break;
            }
            case ':':
            {
                new->tokenName = (char *) malloc (sizeof(char) * strlen("COLON"));
                new->lexeme = (char *) malloc (sizeof(char) * strlen(":"));
                strcpy(new->tokenName, "COLON");
                strcpy(new->lexeme, ":");
                new->lineNumber = *line_count;
                new->next = NULL;
                return new;
                break;
            }

            case '*':
            {
                new->tokenName = (char *) malloc (sizeof(char) * strlen("OP_MULT"));
                new->lexeme = (char *) malloc (sizeof(char) * strlen("*"));
                strcpy(new->tokenName, "OP_MULT");
                strcpy(new->lexeme, "*");
                new->lineNumber = *line_count;
                new->next = NULL;
                return new;
                break;
            }
            case '\t':
            case '\r':
            case ' ':
                break;
            default:
                printf("Lexical Error at line #%d: unidentifiable string: %s.\n", *line_count, token);
                new->tokenName = "DOLLAR";
        }
    }
    new->lexeme = (char *) malloc (sizeof(char) * (strlen(token) + 1));

    strcpy(new->lexeme, token);
    new->lineNumber = *line_count;
    new->next = NULL;
    return new;
}



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
    int line_num = 0;
    while (fgets(buffer, MAX_LEN - 1, fp))
    {
        // Remove trailing newline
        buffer[strcspn(buffer, "\n")] = 0;
        token = strtok(buffer, " ");
        grammarNode * temp = (grammarNode*)malloc(sizeof(grammarNode));
        grammarNode * head = temp;
        temp->grammarWord = (char *)malloc(sizeof(char)*strlen(token));
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
            temp->grammarWord = (char *)malloc(sizeof(char)*strlen(token));
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
    tokenStream* temp = NULL;
    tokenStream* head = NULL;
    tokenStream* prev = NULL;

    while (fgets(buffer, MAX_LEN - 1, fp))
    {
        // Remove trailing newline
        buffer[strcspn(buffer, "\n")] = 0;
        token = strtok(buffer, " ");
       /* walk through other tokens */
        while( token != NULL) {
            temp = get_token(token,&line_num);
            if(!head)
              head = temp;
            if(prev)
              prev->next = temp;
            prev = temp;
            temp = temp->next;
            token = strtok(NULL, " ");
        }
        line_num++;
    }
    fclose(fp);
    s = head;
    tokenStream* test = head;
    while(test != NULL){
      printf("%s\n",test->tokenName );
      test = test->next;
    }
    return 0;
}

// Function Definition: createParseTree (parseTree  *t,  tokenStream  *s,  grammar  G)
int createParseTree (parseTree  *t,  tokenStream  *s,  grammarNode**  G){
    // initiate stack
    // fill start in stack
    // while loop 
    //  check if terminal
    //      pop from stack and add to parse tree
    // check if non terminal
    //      pop from stack and add to parse tree and push rhs of grammar rule to stack
    //
    // pseudocode if we have parsing table (M[X][a]): https://www.tutorialspoint.com/compiler_design/compiler_design_top_down_parser.htm
    
    stack st;
    stack_push(st, "DOLLAR");
    stack_push(st, "start");
    t = malloc(sizeof(parseTree));
    parseTree* current = t;
    for (int i = 0; i < MAX_PARSE_TREE_CHILDREN; i++)
        current->children[i] = NULL;
    current->isTerminal = false;
    current->lexeme = NULL;
    current->lineNumber = 1;
    current->symbolName = "start";
    current->tokenName = NULL;
    tokenStream* currentToken = s;

    while (!strcmp(stack_top(st), "DOLLAR")){
        if ((stack_top(st).terminal == 1) && !strcmp(stack_top(st), currentToken->lexeme)){
            stack_pop(st);
            current = parseTreeInsert(current, currentToken);
            current = parseTreeGetCurrent(t);
            currentToken = currentToken->next;
        }
        else if (stack_top(st).terminal == 0){
            // loop over grammar, for possible rules, check possility using backtracking function.
            int ruleSelectedFlag = 0;
            for (int i = 0; i < NUMBER_OF_GRAMMAR_RULES; i++){
                if (!strcmp(G[i]->grammarWord, stack_top(st))){
                    if (predictRule(G[i], currentToken)){
                        ruleSelectedFlag = 1;
                        stack_pop(st);
                        stack_pushrhs(st, G[i]);
                        populateChildrenGrammarNode(current, G[i]);
                        current = current->children[0];
                    }
                }
            }
            if (!ruleSelectedFlag)
                printf ("No rule Selected, Some error present\n");
        }
        else
            printf("Terminal at top of stack, but doesn't match, some problem exists\n");
    }

    printf ("Parse tree created.\n");

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
