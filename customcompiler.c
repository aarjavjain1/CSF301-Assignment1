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

char *trimwhitespace(char *str)
{
  char *end;

  // Trim leading space
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  // Write new null terminator character
  end[1] = '\0';

  return str;
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
    else if (!strcmp(token, "size"))
        strcpy(new->tokenName, "GSIZE");
    else if (isVariable(token))
        strcpy(new->tokenName,"VAR");
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
            case '(':
            {
                new->tokenName = (char *) malloc (sizeof(char) * strlen("LSIMPLE"));
                new->lexeme = (char *) malloc (sizeof(char) * strlen("("));
                strcpy(new->tokenName, "LSIMPLE");
                strcpy(new->lexeme, "(");
                new->lineNumber = *line_count;
                return new;
                break;
            }
            case ')':
            {
                new->tokenName = (char *) malloc (sizeof(char) * strlen("RSIMPLE"));
                new->lexeme = (char *) malloc (sizeof(char) * strlen(")"));
                strcpy(new->tokenName, "RSIMPLE");
                strcpy(new->lexeme, ")");
                new->lineNumber = *line_count;
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

int isEmpty(stack* root)
{
    return !root;
}

stack* stack_top(stack* root)
{
    if (isEmpty(root))
        return NULL;
    return root;
}

stack* stack_pop(stack* root)
{
    if (isEmpty(root))
      return NULL;
    stack* temp = root;
    root = root->next;
    free(temp);
    return root;
}

int isGrammarWordTerminal(char* str){
  if (str[0] >= 'A' && str[0]<= 'Z')
    return 1;
  else
    return 0;
}

stack* newNode(char* str)
{
    stack* stackNode = (stack*)malloc(sizeof(stack));
    stackNode->str = (char*)(malloc(sizeof(char)*(strlen(str)+1)));
    strcpy(stackNode->str, str);
    stackNode->terminal = isGrammarWordTerminal(str);
    stackNode->next = NULL;
    return stackNode;
}

stack* stack_push(stack* root, char* str){
    stack* stackNode = newNode(str);
    stackNode->next = root;
    // root = stackNode;
    // printf ("end of push to stack: %s\n", str);
    return stackNode;
}

stack* stack_pushrhs(stack* root, grammarNode* G)
{
    if (G == NULL)
        return root;
    else{
        // stack_push(root, G->grammarWord);
        root = stack_pushrhs(root,G->next);
        root = stack_push(root, G->grammarWord);
    }
    return root;
}

void print_stack(stack* root){
    while (root){
        printf ("%s -> ", root->str);
        root = root->next;
    }
    printf("\n");
}

void print_grammar_rule(grammarNode* root){
    while (root){
        printf ("%s -> ", root->grammarWord);
        root = root->next;
    }
    printf("\n");
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

    char *buffer = (char*)malloc(sizeof(char)*MAX_LEN);

    int line_num = 0;
    while (fgets(buffer, MAX_LEN - 1, fp))
    {
        // Remove trailing newline
        buffer[strcspn(buffer, "\n")] = 0;
        buffer = trimwhitespace(buffer);

        token = strtok(buffer, " ");
        // printf("lol\n");
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
tokenStream* tokeniseSourcecode (char* sourceCodeFilePath,  tokenStream  *s){
    // printf ("Inside tokeniseSourcecode.\n");
    FILE* fp;
    char* token;
    fp = fopen(sourceCodeFilePath, "r");
    // if (fp == NULL) {
    //   perror("Failed: ");
    //   return 1;
    // }

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
    // while(test != NULL){
    //   printf("%s\n",test->tokenName );
    //   test = test->next;
    // }
    return head;
}

int predictRule(int grammarRuleNum, grammarNode** G, tokenStream* currentToken){
    stack* st;
    st = stack_push(st, "DOLLAR");
    // printf ("after dollar push, st->str: %s\n", st->str);
    st = stack_pushrhs(st, G[grammarRuleNum]->next);

    tokenStream* temp = currentToken;
    while (strcmp(stack_top(st)->str, "DOLLAR")){
        // printf("inside recursive while st: %s, lex:%s\n", st->str, temp->lexeme);
        if ((stack_top(st)->terminal == 1) && !strcmp(stack_top(st)->str, temp->tokenName)){
            // printf("terminal detected : %s\n", st->str);
            st = stack_pop(st);
            // print_stack(st);
            temp = temp->next;
            // if (st->next == NULL)
                // printf("mast DOLLAR dikha\n");
        }
        else if (stack_top(st)->terminal == 0){
            // printf("non terminal detected : %s\n", st->str);
            // loop over grammar, for possible rules, check possility using backtracking function.
            int ruleSelectedFlag = 0;
            for (int i = 0; i < NUMBER_OF_GRAMMAR_RULES; i++){
                if (!strcmp(G[i]->grammarWord, stack_top(st)->str) && !ruleSelectedFlag){
                    {
                        // printf ("sending grammar rule (predict): ");
                        // print_grammar_rule(G[i]);
                    }
                    if (predictRule(i, G, temp)){
                        // printf("predicted rule: \n");
                        // printf("%s -> %s\n", G[i]->grammarWord, G[i]->next->grammarWord);
                        ruleSelectedFlag = 1;
                        st = stack_pop(st);
                        st = stack_pushrhs(st, G[i]->next);
                        // printf("stack after pushrhs: ");print_stack(st);
                    }
                }
            }
            if (!ruleSelectedFlag){
                // printf("no rule found\n");
                return 0;
            }
        }
        else{
            // printf("returning 0 on st: %s, token: %s\n", st->str, temp->tokenName);
            return 0;
        }
    }
    // printf("returning 1 as prediction on G[i]:");
    // print_grammar_rule(G[grammarRuleNum]);
    return 1;
}

parseTree* parseTreeGetCurrent(parseTree* t){
    parseTree* retpointer;
    int hasChild = 0;
    printf("exporing %d, ", t->isLeaf);
    if (t->children[0] != NULL){
        hasChild = 1;
    }
    printf("haschild: %d\n", hasChild);
    if (hasChild==0 && t->isLeaf == 0)
        return t;

    for (int i = 0; i<MAX_PARSE_TREE_CHILDREN; i++){
        if (t->children[i] == NULL)
            break;
        printf("inside for child i: %d\n", i);
        retpointer = parseTreeGetCurrent(t->children[i]);
        if (retpointer != NULL)
            return retpointer;
    }
    printf ("returning NULL\n");
    return NULL;
}


void populateChildrenGrammarNode(parseTree* current, grammarNode* Gi){
    int childToPopulate = 0;
    while (Gi->next!=NULL){
        Gi = Gi->next;
        int terminal = isGrammarWordTerminal(Gi->grammarWord);
        current->children[childToPopulate] = (parseTree*)malloc(sizeof(parseTree));
        current->children[childToPopulate]->isTerminal = terminal;
        current->children[childToPopulate]->grammarRuleUsed = NULL;
        current->children[childToPopulate]->isLeaf = 0;
        printf("populated %s", Gi->grammarWord);
        childToPopulate++;
        // other fields for this node in the parse tree will be filled in createParseTree if terminal on stack is detected
    }
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

    stack* st = NULL;
    st = stack_push(st, "DOLLAR");
    st = stack_push(st, "main_program");
    // printf ("Dollar and start pushed to stack\n");
    // tokenStream* test =s;
    // while(test != NULL){
    //   printf("%s\n",test->tokenName );
    //   test = test->next;
    // }
    t = malloc(sizeof(parseTree));
    parseTree* current = t;
    for (int i = 0; i < MAX_PARSE_TREE_CHILDREN; i++)
        current->children[i] = NULL;
    current->isTerminal = false;
    current->lexeme = NULL;
    current->lineNumber = 0;
    current->symbolName = "main_program";
    current->tokenName = NULL;
    current->isLeaf = 0;
    tokenStream* currentToken = s;
    // printf ("stack top se pehle\n");
    // printf ("outside while stack top: %s\n", st->str);
    int debugCounter = 0;
    while (strcmp(stack_top(st)->str, "DOLLAR")){
        {
            // printf ("%d main stack at big while:", debugCounter);
            // printf("stack in big while: ");print_stack(st);
            // printf("current: %llu\n", current);
        }
        if ((stack_top(st)->terminal == 1) && !strcmp(stack_top(st)->str, currentToken->tokenName)){
            printf("accepted terminal: %s\n", currentToken->tokenName);
            st = stack_pop(st);
            current->lexeme = currentToken->lexeme;
            current->lineNumber = currentToken->lineNumber;
            current->tokenName = currentToken->tokenName;
            current->symbolName = currentToken->tokenName;
            current->isLeaf = 1;
            for (int i = 0; i < MAX_PARSE_TREE_CHILDREN; i++)
                current->children[i] = NULL;
            current = parseTreeGetCurrent(t);
            if (current == NULL)
                printf("terminal ke baad diqqat\n");
            currentToken = currentToken->next;
        }
        else if (stack_top(st)->terminal == 0){
            // loop over grammar, for possible rules, check possility using backtracking function.
            int ruleSelectedFlag = 0;
            for (int i = 0; i < NUMBER_OF_GRAMMAR_RULES; i++){
                // printf("grammar rule for if : %s\n", G[i]->grammarWord);
                if (!strcmp(G[i]->grammarWord, stack_top(st)->str) && !ruleSelectedFlag){
                    // {
                    //     printf ("seding grammar rule: ");
                    //     print_grammar_rule(G[i]);
                    // }
                    if (predictRule(i, G, currentToken)){
                        // print_grammar_rule(G[i]);
                        printf("got a rule yayayayayayay\n");
                        ruleSelectedFlag = 1;
                        st = stack_pop(st);
                        st = stack_pushrhs(st, G[i]->next);
                        current->grammarRuleUsed = G[i];
                        current->symbolName = G[i]->grammarWord;
                        printf("stack after rule insertion: ");print_stack(st);
                        populateChildrenGrammarNode(current, G[i]);
                        if (current->children[0] == NULL)
                            printf("KHALi!!!\n");
                        current = parseTreeGetCurrent(t);
                        if (current == NULL)
                            printf("sachmein KHALi!!!\n");
                    }
                }
            }
            // if (!ruleSelectedFlag)
            //     printf ("No rule Selected, Some error present\n");
        }
        else{
            printf("Terminal at top of stack, but doesn't match, some problem exists\n");
            printf("st ka top: %s, token: %s", st->str, currentToken->tokenName);
        }
    }
    printParseTree(t);
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
    printf("Printing Tree ----  parseTreeNode: %s\n", t->symbolName);
    for (int i = 0; i< MAX_PARSE_TREE_CHILDREN; i++){
        if (t->children[i])
            printParseTree(t->children[i]);
    }
    return 0;
}

// Function Definition: printTypeExpressionTable (typeExpressionTable T)
int printTypeExpressionTable (typeExpressionTable T){
    printf ("Inside printTypeExpressionTable.\n");
    return 0;
}
