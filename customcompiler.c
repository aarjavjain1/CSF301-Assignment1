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
    // new->tokenName = (char *) malloc (sizeof(char) * 100);
    if (search(token) == 1){
      // printf("NICHT\n");
      char *temp_ = (char *) malloc(sizeof(char)*(strlen(token)+1));
      for (int yu=0; yu < (int) strlen(token); ++yu)
          temp_[yu] = toupper(token[yu]);
    temp_[(int)strlen(token)] = '\0';
      //printf("token: %s\n", buff);
      new->tokenName = (char*) malloc(sizeof(char) * (strlen(temp_)+1));
      strcpy(new->tokenName, temp_);
    }
    else if (!strcmp(token, "R1")){
        new->tokenName = (char*) malloc(sizeof(char) * (strlen("R1")+1));
        strcpy(new->tokenName, "R1");
    }
    else if (!strcmp(token, "size")){
        new->tokenName = (char*) malloc(sizeof(char) * (strlen("GSIZE")+1));
        strcpy(new->tokenName, "GSIZE");
    }
    else if (isVariable(token)){
        new->tokenName = (char*) malloc(sizeof(char) * (strlen("VAR")+1));
        strcpy(new->tokenName,"VAR");
    }
    else if(isNumber(token)){
        new->tokenName = (char*) malloc(sizeof(char) * (strlen("NUMBER")+1));
        strcpy(new->tokenName,"NUMBER");
    }
    else if(!strcmp(token,"&&&")){
        new->tokenName = (char*) malloc(sizeof(char) * (strlen("OP_AND")+1));
        strcpy(new->tokenName,"OP_AND");
    }
    else if(!strcmp(token,"|||")){
        new->tokenName = (char*) malloc(sizeof(char) * (strlen("OP_OR")+1));
        strcpy(new->tokenName,"OP_OR");
    }
    else if(!strcmp(token,"..")){
        new->tokenName = (char*) malloc(sizeof(char) * (strlen("DOT")+1));
        strcpy(new->tokenName,"DOT");
    }
    else if(strlen(token)==1)
    {
        switch (token[0])
        {
            case '+':
            {
                new->tokenName = (char *) malloc (sizeof(char) * (strlen("OP_PLUS")+1));
                new->lexeme = (char *) malloc (sizeof(char) * (strlen("+")+1));
                strcpy(new->tokenName, "OP_PLUS");
                strcpy(new->lexeme, "+");
                new->lineNumber = *line_count;
                new->next = NULL;
                return new;
                break;
            }
            case '-':
            {
                new->tokenName = (char *) malloc (sizeof(char) * (strlen("OP_MINUS") + 1));
                new->lexeme = (char *) malloc (sizeof(char) * (strlen("-")+1));
                strcpy(new->tokenName, "OP_MINUS");
                strcpy(new->lexeme, "-");
                new->lineNumber = *line_count;
                new->next = NULL;
                return new;
                break;
            }
            case '/':
            {
                new->tokenName = (char *) malloc (sizeof(char) * (strlen("OP_DIV")+1));
                new->lexeme = (char *) malloc (sizeof(char) * (strlen("/")+1));
                strcpy(new->tokenName, "OP_DIV");
                strcpy(new->lexeme, "/");
                new->lineNumber = *line_count;
                new->next = NULL;
                return new;
                break;
            }
            case '[':
            {
                new->tokenName = (char *) malloc (sizeof(char) * (strlen("LSQUARE")+1));
                new->lexeme = (char *) malloc (sizeof(char) * (strlen("[")+1));
                strcpy(new->tokenName, "LSQUARE");
                strcpy(new->lexeme, "[");
                new->lineNumber = *line_count;
                new->next = NULL;
                return new;
                break;
            }
            case ']':
            {
                new->tokenName = (char *) malloc (sizeof(char) * (strlen("RQSQUARE")+1));
                new->lexeme = (char *) malloc (sizeof(char) * (strlen("]")+1));
                strcpy(new->tokenName, "RSQUARE");
                strcpy(new->lexeme, "]");
                new->lineNumber = *line_count;
                new->next = NULL;
                return new;
                break;
            }
            case '{':
            {
                new->tokenName = (char *) malloc (sizeof(char) * (strlen("LCURLY")+1));
                new->lexeme = (char *) malloc (sizeof(char) * (strlen("{")+1));
                strcpy(new->tokenName, "LCURLY");
                strcpy(new->lexeme, "{");
                new->lineNumber = *line_count;
                new->next = NULL;
                return new;
                break;
            }
            case '}':
            {
                new->tokenName = (char *) malloc (sizeof(char) * (strlen("RCURLY")+1));
                new->lexeme = (char *) malloc (sizeof(char) * (strlen("}")+1));
                strcpy(new->tokenName, "RCURLY");
                strcpy(new->lexeme, "}");
                new->lineNumber = *line_count;
                new->next = NULL;
                return new;
                break;
            }
            case '(':
            {
                new->tokenName = (char *) malloc (sizeof(char) * (strlen("LSIMPLE")+1));
                new->lexeme = (char *) malloc (sizeof(char) * (strlen("(")+1));
                strcpy(new->tokenName, "LSIMPLE");
                strcpy(new->lexeme, "(");
                new->lineNumber = *line_count;
                new->next = NULL;
                return new;
                break;
            }
            case ')':
            {
                new->tokenName = (char *) malloc (sizeof(char) * (strlen("RSIMPLE")+1));
                new->lexeme = (char *) malloc (sizeof(char) * (strlen(")")+1));
                strcpy(new->tokenName, "RSIMPLE");
                strcpy(new->lexeme, ")");
                new->lineNumber = *line_count;
                new->next = NULL;
                return new;
                break;
            }
            case ';':
            {
                new->tokenName = (char *) malloc (sizeof(char) * (strlen("SEMICOLON")+1));
                new->lexeme = (char *) malloc (sizeof(char) * (strlen(";")+1));
                strcpy(new->tokenName, "SEMICOLON");
                strcpy(new->lexeme, ";");
                new->lineNumber = *line_count;
                new->next = NULL;
                return new;
                break;
            }
            case ',':
            {
                new->tokenName = (char *) malloc (sizeof(char) * (strlen("COMMA")+1));
                new->lexeme = (char *) malloc (sizeof(char) * (strlen(",")+1));
                strcpy(new->tokenName, "COMMA");
                strcpy(new->lexeme, ",");
                new->lineNumber = *line_count;
                new->next = NULL;
                return new;
                break;
            }
            case '=':
            {
                new->tokenName = (char *) malloc (sizeof(char) * (strlen("EQUALS")+1));
                new->lexeme = (char *) malloc (sizeof(char) * (strlen("=")+1));
                strcpy(new->tokenName, "EQUALS");
                strcpy(new->lexeme, "=");
                new->lineNumber = *line_count;
                new->next = NULL;
                return new;
                break;
            }
            case ':':
            {
                new->tokenName = (char *) malloc (sizeof(char) * (strlen("COLON")+1));
                new->lexeme = (char *) malloc (sizeof(char) * (strlen(":")+1));
                strcpy(new->tokenName, "COLON");
                strcpy(new->lexeme, ":");
                new->lineNumber = *line_count;
                new->next = NULL;
                return new;
                break;
            }

            case '*':
            {
                new->tokenName = (char *) malloc (sizeof(char) * (strlen("OP_MULT")+1));
                new->lexeme = (char *) malloc (sizeof(char) * (strlen("*")+1));
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
    new->lexeme = (char *) malloc (sizeof(char) * (strlen(token)+1));

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
    return root;
}

stack* stack_pop(stack* root)
{
    if (root == NULL)
      return NULL;
    stack* temp = root;
    root = root->next;
    free(temp->str);
    free(temp);
    return root;
}

int isGrammarWordTerminal(char* str){
  if (str[0] >= 'A' && str[0]<= 'Z')
    return 1;
  else
    return 0;
}

// stack* newNode(char* str)
// {
//     return NULL;
// }

stack* stack_push(stack* root, char* str){
    stack* stackNode = (stack*)malloc(sizeof(stack));
    stackNode->str = (char*) malloc(sizeof(char) * (strlen(str) + 1));
    strcpy(stackNode->str, str);
    stackNode->terminal = isGrammarWordTerminal(str);
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
        temp->grammarWord = (char *)malloc(sizeof(char)*(strlen(token) + 1));
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
            temp->next = NULL;
            temp->grammarWord = (char *)malloc(sizeof(char)*(strlen(token)+1));
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

grammarOrderNode* grammarOrderAdd(int ruleNum, grammarOrderNode *grammarOrder){
    if (grammarOrder == NULL){
        grammarOrder = (grammarOrderNode*) malloc(sizeof(grammarOrderNode));
        grammarOrder->grammarRuleNum = ruleNum;
        grammarOrder->next = NULL;
        return grammarOrder;
    }
    grammarOrderNode *temp = grammarOrder;
    while (temp->next != NULL)
        temp = temp->next;
    temp->next = (grammarOrderNode*) malloc(sizeof(grammarOrderNode));
    temp->grammarRuleNum = ruleNum;
    temp->next->next = NULL;
    return grammarOrder;
}

grammarOrderNode* grammarOrderRemoveLast(grammarOrderNode *grammarOrder){
    grammarOrderNode *temp = grammarOrder;
    if (temp == NULL)
        return temp;
    if (temp->next == NULL){
        free(temp);
        return NULL;
    }
    while (temp->next->next != NULL)
        temp = temp->next;
    free(temp->next);
    temp->next = NULL;
    return grammarOrder;
}

//remove after debugging
void printParseTreeNode(parseTree* node){
    // printf ("Current stats ----");
    // if (node == NULL)
    //     printf("NULL hai\n");
    // else {
    //     printf (" %lu \n", node);
    // }
    // printf("isterminal : %d --\n", node->isTerminal);
    // printf("symbolName : %s --\n", node->symbolName);
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

    char *buffer = (char* ) malloc(sizeof(char) * MAX_LEN);
    // -1 to allow room for NULL terminator for really long string
    int line_num = 0;
    tokenStream* temp = NULL;
    tokenStream* head = NULL;
    tokenStream* prev = NULL;

    while (fgets(buffer, MAX_LEN - 1, fp))
    {
        // Remove trailing newline
        buffer[strcspn(buffer, "\n")] = 0;
        buffer = trimwhitespace(buffer);
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

parseTree* parseTreeGetCurrent(parseTree* t){
    if (t == NULL)
        return NULL;
    parseTree* retpointer;
    int hasChild = 0;
    // printf("exporing %d, ", t->isLeaf);
    if (t->children[0] != NULL){
        hasChild = 1;
    }
    // printf("haschild: %d\n", hasChild);
    if (hasChild==0 && t->isLeaf == 0)
        return t;

    for (int i = 0; i<MAX_PARSE_TREE_CHILDREN; i++){
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

void populateChildrenGrammarNode(parseTree* current, grammarNode* Gi){
    if (current == NULL)
        printf("populateChildren Got null current with rule: %s", Gi->grammarWord);
    int childToPopulate = 0;
    while (Gi->next!=NULL){
        Gi = Gi->next;
        int terminal = isGrammarWordTerminal(Gi->grammarWord);
        current->children[childToPopulate] = (parseTree*)malloc(sizeof(parseTree));
        current->children[childToPopulate]->isTerminal = terminal;
        current->children[childToPopulate]->grammarRuleUsed = NULL;
        current->children[childToPopulate]->isLeaf = 0;
        current->children[childToPopulate]->symbolName = NULL;
        for (int i = 0; i<MAX_PARSE_TREE_CHILDREN; i++)
            current->children[childToPopulate]->children[i] = NULL;
        // printf("populated %s", Gi->grammarWord);
        childToPopulate++;
        // other fields for this node in the parse tree will be filled in createParseTree if terminal on stack is detected
    }
}

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

int predictRule(int grammarRuleNum, grammarNode** G, tokenStream** recievedToken, grammarOrderNode **grammarOrderAddress, parseTree* t){
    stack* st = NULL;
    st = stack_push(st, "DOLLAR");
    st->next = NULL;
    st = stack_pushrhs(st, G[grammarRuleNum]->next);
    tokenStream* currentToken = (*recievedToken);

    populateChildrenGrammarNode(t, G[grammarRuleNum]);
    parseTree* current = parseTreeGetCurrent(t);

    while (strcmp(stack_top(st)->str, "DOLLAR")){
        if ((stack_top(st)->terminal == 1) && !strcmp(stack_top(st)->str, currentToken->tokenName)){
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
        }
        else if (stack_top(st)->terminal == 0){
            int ruleSelectedFlag = 0;
                for (int i = 0; i < NUMBER_OF_GRAMMAR_RULES; i++){
                    if (!strcmp(G[i]->grammarWord, stack_top(st)->str) && !ruleSelectedFlag){
                        tokenStream* sentToken = currentToken;
                        int predict = predictRule(i, G, &sentToken, grammarOrderAddress, current);
                        if (predict == 1){
                            currentToken = sentToken;
                            ruleSelectedFlag = 1;
                            st = stack_pop(st);
                            current->grammarRuleUsed = G[i];
                            current->symbolName = G[i]->grammarWord;
                            current = parseTreeGetCurrent(t);
                        }
                        else{
                            freeParseTreeNodeChildren(current);
                        }
                    }
                }
                if (!ruleSelectedFlag){
                    return 0;
                }
        }
        else{
            freeStackMemory(st);
            return 0;
        }
    }
    freeStackMemory(st);
    (*recievedToken) = currentToken;
    return 1;
}

// Function Definition: createParseTree (parseTree  *t,  tokenStream  *s,  grammar  G)
int createParseTree (parseTree  *t,  tokenStream  *s,  grammarNode**  G){
    // pseudocode if we have parsing table (M[X][a]): https://www.tutorialspoint.com/compiler_design/compiler_design_top_down_parser.htm

    grammarOrderNode *grammarOrder = NULL;

    stack* st = NULL;
    st = stack_push(st, "DOLLAR");
    st = stack_push(st, "main_program");
    t = malloc(sizeof(parseTree));
    parseTree* current = t;
    for (int i = 0; i < MAX_PARSE_TREE_CHILDREN; i++)
        current->children[i] = NULL;
    current->isTerminal = false;
    current->lexeme = NULL;
    current->lineNumber = 0;
    current->symbolName = malloc(sizeof(char) * (strlen("main_program")+1));
    strcpy(current->symbolName, "main_program");
    current->tokenName = NULL;
    current->isLeaf = 0;
    tokenStream* currentToken = s;
    int debugCounter = 0;
    while (strcmp(stack_top(st)->str, "DOLLAR")){
        if ((stack_top(st)->terminal == 1) && !strcmp(stack_top(st)->str, currentToken->tokenName)){
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
        }
        else if (stack_top(st)->terminal == 0){
            int ruleSelectedFlag = 0;
                for (int i = 0; i < NUMBER_OF_GRAMMAR_RULES; i++){
                    if (!strcmp(G[i]->grammarWord, stack_top(st)->str) && !ruleSelectedFlag){
                        // {
                            // printf ("seding grammar rule: ");
                            // print_grammar_rule(G[i]);
                            // printParseTreeNode(current);
                        // }
                        tokenStream* sentToken = currentToken;
                        int predict = predictRule(i, G, &sentToken, &grammarOrder, current);
                        if (predict == 1){
                            currentToken = sentToken;
                            ruleSelectedFlag = 1;
                            st = stack_pop(st);
                            current->grammarRuleUsed = G[i];
                            current->symbolName = G[i]->grammarWord;
                            // if (current->children[0] == NULL)
                            //     printf("KHALi!!!\n");
                            current = parseTreeGetCurrent(t);
                        }
                        else{
                            freeParseTreeNodeChildren(current);
                            for (int i = 0; i < MAX_PARSE_TREE_CHILDREN; i++)
                                current->children[i] = NULL;
                        }
                    }
                }
                if (!ruleSelectedFlag){
                    printf ("No rule Selected, Some error present\n");
                    break;
                }
        }
        else{
            printf("Terminal at top of stack, but doesn't match, some problem exists\n");
            printf("st ka top: %s, token: %s\n", st->str, currentToken->tokenName);
            break;
        }
    }
    freeStackMemory(st);
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
