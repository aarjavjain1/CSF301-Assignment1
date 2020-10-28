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
    int line_num = 1;
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
        current->children[childToPopulate]->lexeme = NULL;
        current->children[childToPopulate]->tokenName = NULL;
        current->children[childToPopulate]->isLeaf = 0;
        current->children[childToPopulate]->symbolName = NULL;
        for (int i = 0; i<MAX_PARSE_TREE_CHILDREN; i++)
            current->children[childToPopulate]->children[i] = NULL;
        // printf("populated %s", Gi->grammarWord);
        childToPopulate++;
        // other fields for this node in the parse tree will be filled in createParseTree if terminal on stack is detected
    }
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
parseTree* createParseTree (parseTree  *t,  tokenStream  *s,  grammarNode**  G){
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
                    printf("st ka top: %s, token: %s\n", st->str, currentToken->tokenName);
                    printf ("No rule Selected, Some error present\n");
                    return NULL;
                }
        }
        else{
            printf("Terminal at top of stack, but doesn't match, some problem exists\n");
            printf("st ka top: %s, token: %s\n", st->str, currentToken->tokenName);
            return NULL;
        }
    }
    freeStackMemory(st);
    // printParseTree(t);
    printf ("Parse tree created.\n");
    return t;
}

// Function Definition: printParseTree (parseTree *t)
int printParseTree (parseTree *t){
    printf("Printing Tree ----  parseTreeNode: %s \n", t->symbolName);
    // for (int i = 0; i< MAX_PARSE_TREE_CHILDREN; i++){
    //     if (t->children[i])
    //         printf("%s\t", t->children[i]->symbolName);
    // }
    // printf("\n");
    for (int i = 0; i< MAX_PARSE_TREE_CHILDREN; i++){
        if (t->children[i])
            printParseTree(t->children[i]);
    }
    return 0;
}

parseTree* parseArray[MAX_VARIABLES];
static int parseIndex = 0;

int max(int a, int b){
    if (a > b) return a;
    else return b;
}

int min(int a, int b){
    if (a > b) return b;
    else return a;
}

void recurse(parseTree *t){
    if (t && t->isLeaf){
        parseArray[parseIndex] = t;
        parseIndex++;
    } else {
        for (int i = 0; i< MAX_PARSE_TREE_CHILDREN; i++)
            if (t->children[i])
                recurse(t->children[i]);
    }
}

void addDeclaration(parseTree** t, typeExpressionTable **T){
    // printf("Printing Declaration Tree ----  parseTreeNode: %s -> ", t->symbolName);
    for(int i = 0; i < MAX_VARIABLES; i++){
        parseArray[i] = NULL;
    }
    parseIndex = 0;
    recurse(*t);
    // for (int i = 0; i < parseIndex; i++) {
    //     printf("%s ", parseArray[i]->symbolName);
    // }
    printf("\n");
    int low_var = 200000000, high_var = -200000000, colon = -1;
    for (int i = 0; i < parseIndex; i++) {
        // printf("%s ", parseArray[i]->symbolName);
        if (!strcmp(parseArray[i]->symbolName, "VAR") && colon == -1){
            low_var = min(low_var, i);
            high_var = max(high_var, i);
        }
        if (colon == -1 && !strcmp(parseArray[i]->symbolName, "COLON")){
            colon = i;
        }
    }
    // printf("%d %d %d\n", low_var, high_var, colon);
    if (!strcmp(parseArray[colon+1]->symbolName, "INTEGER") || !strcmp(parseArray[colon+1]->symbolName, "BOOLEAN") || !strcmp(parseArray[colon+1]->symbolName, "REAL")){
        // printf("Mai to ithhe paaji\n");
        for (int i = low_var; i <= high_var; i++){
            typeExpressionTable* temp = (typeExpressionTable*)malloc(sizeof(typeExpressionTable));
            temp->name = (char*)malloc(strlen(parseArray[i]->lexeme));
            strcpy(temp->name, parseArray[i]->lexeme);
            temp->type = primitive;
            temp->array_type = NA;
            temp->exp = (expression*)malloc(sizeof(expression));
            temp->exp->a = (prim*)malloc(sizeof(prim));
            temp->exp->a->basicElementType = (char*)malloc(strlen(parseArray[colon+1]->symbolName));
            temp->next = NULL;
            strcpy(temp->exp->a->basicElementType,parseArray[colon+1]->symbolName);
            if (*T == NULL) *T = temp;
            else {
                typeExpressionTable* l = *T;
                while(l->next) l = l->next;
                l->next = temp;
            }
        }
    }
    else if (!strcmp(parseArray[colon+1]->symbolName, "ARRAY")){
        for (int i = low_var; i <= high_var; i++){
            typeExpressionTable* temp = (typeExpressionTable*)malloc(sizeof(typeExpressionTable));
            temp->name = (char*)malloc(strlen(parseArray[i]->lexeme));
            strcpy(temp->name, parseArray[i]->lexeme);
            temp->type = rect_array;
            temp->next = NULL;
            temp->exp = (expression*)malloc(sizeof(expression));
            temp->exp->b = (rect*)malloc(sizeof(rect));
            temp->exp->b->basicElementType = (char*)malloc(sizeof("INTEGER")+1);
            temp->exp->b->d = NULL;
            strcpy(temp->exp->b->basicElementType, "INTEGER");
            bool lstat = true;
            int dim = 0;
            for (int i = colon+1; i < parseIndex; i++){
                if (!strcmp(parseArray[i]->symbolName, "LSQUARE")){
                    rect_dimension* rd = (rect_dimension*)malloc(sizeof(rect_dimension));
                    dim++;
                    // if ((!strcmp(parseArray[i+1]->symbolName, "VAR") || !strcmp(parseArray[i+1]->symbolName, "NUMBER")) && (!strcmp(parseArray[i+3]->symbolName, "VAR") || !strcmp(parseArray[i+3]->symbolName, "NUMBER"))) {
                        // if (!strcmp(parseArray[i+1], "VAR") || !strcmp(parseArray[i+3], "VAR")) stat = false;
                    if (!strcmp(parseArray[i+1]->symbolName, "VAR"))
                        lstat = false;
                    rd->low = (char*)malloc(strlen(parseArray[i+1]->lexeme));
                    strcpy(rd->low, parseArray[i+1]->lexeme);

                    if (!strcmp(parseArray[i+3]->symbolName, "VAR"))
                        lstat = false;
                    rd->high = (char*)malloc(strlen(parseArray[i+3]->lexeme));
                    strcpy(rd->high, parseArray[i+3]->lexeme);
                    rd->next = NULL;
                    if (temp->exp->b->d == NULL) temp->exp->b->d = rd;
                    else {
                        rect_dimension* l = temp->exp->b->d;
                        while(l->next) l = l->next;
                        l->next = rd;
                    }
                    // }
                }
            }
            if (lstat) temp->array_type = stat;
            else temp->array_type = dyn;
            temp->exp->b->dimensions = dim;
            if (*T == NULL) *T = temp;
            else {
                typeExpressionTable* l = *T;
                while(l->next) l = l->next;
                l->next = temp;
            }
        }
    }
    else if (!strcmp(parseArray[colon+1]->symbolName, "JAGGED")){
        int colon_loc = colon;
        for (int i = low_var; i <= high_var; i++, colon=colon_loc){
            typeExpressionTable* temp = (typeExpressionTable*)malloc(sizeof(typeExpressionTable));
            temp->name = (char*)malloc(strlen(parseArray[i]->lexeme));
            strcpy(temp->name, parseArray[i]->lexeme);
            temp->type = jagged_array;
            temp->array_type = NA;
            temp->next = NULL;
            temp->exp = (expression*)malloc(sizeof(expression));
            temp->exp->c = (jagged*)malloc(sizeof(jagged));
            temp->exp->c->basicElementType = (char*)malloc(sizeof("INTEGER")+1);
            strcpy(temp->exp->c->basicElementType, "INTEGER");
            temp->exp->c->d = NULL;
            int low, high, dim;
            // printf("Yaha pahunch gayi mai1\n");
            if (!strcmp(parseArray[colon+2]->symbolName, "ARRAY") &&
                !strcmp(parseArray[colon+3]->symbolName, "LSQUARE") &&
                !strcmp(parseArray[colon+4]->symbolName, "NUMBER") &&
                !strcmp(parseArray[colon+5]->symbolName, "DOT") &&
                !strcmp(parseArray[colon+6]->symbolName, "NUMBER") &&
                !strcmp(parseArray[colon+7]->symbolName, "RSQUARE") &&
                !strcmp(parseArray[colon+8]->symbolName, "LSQUARE") &&
                !strcmp(parseArray[colon+9]->symbolName, "RSQUARE")) {
                    if (!strcmp(parseArray[colon+10]->symbolName, "LSQUARE") && !strcmp(parseArray[colon+11]->symbolName, "RSQUARE")) temp->exp->c->dimensions = 3;
                    else temp->exp->c->dimensions = 2;
            // printf("Yaha pahunch gayi mai2\n");
                    temp->exp->c->low = atoi(parseArray[colon+4]->lexeme);
                    temp->exp->c->high = atoi(parseArray[colon+6]->lexeme);
                    low = atoi(parseArray[colon+4]->lexeme);
                    high = atoi(parseArray[colon+6]->lexeme);
                    dim = temp->exp->c->dimensions;
            } else { // should not occur (not accepted by grammar)
                printf("Type Error Occurred\n");
                return;
            }

            for (; colon < parseIndex; colon++) {
                if (strcmp(parseArray[colon]->symbolName, "R1")) { // if not R1
                    continue;
                } else {
                    jagged_dimension* jd = (jagged_dimension*)malloc(sizeof(jagged_dimension));
                    jd->parent = atoi(parseArray[colon+2]->lexeme);
                    if (jd->parent < low || jd->parent > high) {
                        printf("Jagged Array Index out of Bounds\n");
                        return;
                    }
                    jd->size = atoi(parseArray[colon+6]->lexeme);
                    jd->next = NULL;
                    int loop = jd->size;
                    colon += 10;
                    if (dim == 2){
                        while(--loop){
                            if (strcmp(parseArray[colon]->symbolName, "NUMBER") || strcmp(parseArray[colon+1]->symbolName, "SEMICOLON")) {
                                printf("Type Error Occurred\n");
                                return;
                            }
                            colon += 2;
                        }
                        if (strcmp(parseArray[colon]->symbolName, "NUMBER") || strcmp(parseArray[colon+1]->symbolName, "RCURLY")) {
                            printf("Type Error Occurred\n");
                            return;
                        }
                        jd->inner_size = NULL;
                    }
                    else if (dim == 3){
                        while(--loop){
                            int inner_size = 0;
                            for (; strcmp(parseArray[colon]->symbolName, "SEMICOLON"); colon++){
                                if (!strcmp(parseArray[colon]->symbolName, "RCURLY")) {
                                    printf("Type Error Occurred123\n");
                                    return;
                                }
                                if (!strcmp(parseArray[colon]->symbolName, "NUMBER")) inner_size++;
                                else {
                                    printf("Type Error Occurred456\n");
                                    return;
                                }
                            }
                            colon++;
                            dimension* d = (dimension*)malloc(sizeof(dimension));
                            d->size = inner_size;
                            d->next = NULL;
                            if (jd->inner_size == NULL) jd->inner_size = d;
                            else {
                                dimension* l = jd->inner_size;
                                while(l->next) l = l->next;
                                l->next = d;
                            }
                        }
                        int inner_size = 0;
                        for (; strcmp(parseArray[colon]->symbolName, "RCURLY"); colon++){
                            if (!strcmp(parseArray[colon]->symbolName, "NUMBER")) inner_size++;
                            else {
                                printf("Type Error Occurred789\n");
                                return;
                            }
                        }
                        dimension* d = (dimension*)malloc(sizeof(dimension));
                        d->size = inner_size;
                        d->next = NULL;
                        if (jd->inner_size == NULL) jd->inner_size = d;
                        else {
                            dimension* l = jd->inner_size;
                            while(l->next) l = l->next;
                            l->next = d;
                        }
                    }
                    else {
                        printf("Jagged Array Dimension out of range\n");
                    }
                    if (temp->exp->c->d == NULL) temp->exp->c->d = jd;
                    else {
                        jagged_dimension* l = temp->exp->c->d;
                        while(l->next) l = l->next;
                        l->next = jd;
                    }
                }
            }
            temp->exp->b->dimensions = dim;
            if (*T == NULL) *T = temp;
            else {
                typeExpressionTable* l = *T;
                while(l->next) l = l->next;
                l->next = temp;
            }
        }
    }
    else {
        printf("Traversal Error Occured\n");
    }

    // for (int i = 0; i< MAX_PARSE_TREE_CHILDREN; i++){
    //     if (t->children[i])
    //         printf("%s\t", t->children[i]->symbolName);
    // }
    // printf("\n");
    // for (int i = 0; i< MAX_PARSE_TREE_CHILDREN; i++){
    //     if (t->children[i])
    //         printParseTree(t->children[i]);
    // }
    // printf("\n");.
    return;
}

typeExpressionTable* getExpression(parseTree * input, typeExpressionTable* table){
    // printf("Inside getExpression\n\n\n\n");
    if(!strcmp(input->symbolName,"lhs")){
        if(!strcmp(input->children[0]->symbolName,"VAR")){
            if(input->children[1] == NULL){
                typeExpressionTable* temp = table;
                while(strcmp(temp->name,input->children[0]->lexeme)){
                    temp = temp->next;
                }
                return temp;
            }
            else if(!strcmp(input->children[1]->symbolName,"LSQUARE")){
              //do this
                int numNums = 0;
                // while(input->children[2]->children[numNums]!=NULL) numNums++;
                parseTree* rec = input->children[2];
                while(rec){
                    numNums++;
                    rec = rec->children[1];
                }
                rec = input->children[2];
                typeExpressionTable* temp = table;
                while(strcmp(temp->name,input->children[0]->lexeme)){
                    temp = temp->next;
                }
                if (temp->type == jagged_array){
                    if (numNums != temp->exp->c->dimensions)
                        return NULL;
                    jagged* dim_details = temp->exp->c;
                    int dimensions = temp->exp->c->dimensions;
                    if(dimensions == 2){
                        int low_1 = dim_details->low;
                        int high_1 = dim_details->high;
                        int s1 = atoi(rec->children[0]->lexeme);
                        int s2 = atoi(rec->children[1]->children[0]->lexeme);
                        if(s1<low_1 || s1>high_1)
                            return NULL;
                        jagged_dimension* inner_details = dim_details->d;
                        while(inner_details && s1 != inner_details->parent){
                            inner_details = inner_details->next;
                            // s1--;
                        }
                        int high_2 = inner_details->size;
                        if(s2>=high_2)
                            return NULL;

                        typeExpressionTable* temp_ = (typeExpressionTable*)malloc(sizeof(typeExpressionTable));
                        temp_->name = (char*)malloc(sizeof(char)*strlen("non_terminal"));
                        strcpy(temp_->name,"non_terminal");
                        temp_->type = primitive;
                        temp_->next = NULL;
                        temp_->exp = (expression *)malloc(sizeof(expression));
                        temp_->exp->a = (prim *)malloc(sizeof(prim));
                        temp_->exp->a->basicElementType = (char *)malloc(sizeof(char)*strlen("INTEGER"));
                        strcpy(temp_->exp->a->basicElementType,"INTEGER");
                        return temp_;
                    }
                    else if(dimensions == 3){
                        int low_1 = dim_details->low;
                        int high_1 = dim_details->high;
                        int s1 = atoi(rec->children[0]->lexeme);
                        int s2 = atoi(rec->children[1]->children[0]->lexeme);
                        int s3 = atoi(rec->children[1]->children[1]->children[0]->lexeme);
                        if(s1<low_1 || s1>high_1)
                            return NULL;
                        jagged_dimension* inner_details = dim_details->d;
                        while(s1>low_1){
                            inner_details = inner_details->next;
                            s1--;
                        }
                        int high_2 = inner_details->size;
                        if(s2>=high_2)
                            return NULL;
                        dimension* inner_inner_details = inner_details->inner_size;
                        int high_3 = 1;
                        while(inner_inner_details->next != NULL){
                          high_3++;
                          inner_inner_details = inner_inner_details->next;
                        }
                        if(s3>=high_3)
                            return NULL;

                        typeExpressionTable* temp_ = (typeExpressionTable*)malloc(sizeof(typeExpressionTable));
                        temp_->name = (char*)malloc(sizeof(char)*strlen("non_terminal"));
                        strcpy(temp_->name,"non_terminal");
                        temp_->next = NULL;
                        temp_->type = primitive;
                        temp_->exp = (expression *)malloc(sizeof(expression));
                        temp_->exp->a = (prim *)malloc(sizeof(prim));
                        temp_->exp->a->basicElementType = (char *)malloc(sizeof(char)*strlen("INTEGER"));
                        strcpy(temp_->exp->a->basicElementType,"INTEGER");
                        return temp_;
                    }
                }
                else if (temp->type == rect_array){
                    if (numNums != temp->exp->b->dimensions)
                      return NULL;
                    rect_dimension* temp_rect = temp->exp->b->d;
                    while(temp_rect!=NULL){
                        int dim_ = atoi(rec->children[0]->symbolName);
                        int high = atoi(temp_rect->high);
                        int low = atoi(temp_rect->low);
                        if(dim_<low || dim_>high)
                            return NULL;
                        temp_rect = temp_rect->next;
                        rec = rec->children[1];
                    }
                }

            }
            else{
                printf("This should not have occured lhs !->var lsq || var \n");
                return NULL;
            }
        }
        else{
            printf("This should not have occured lhs !-> var\n");
            return NULL;
        }
    }
    else if(!strcmp(input->symbolName,"factor")){
        if(!strcmp(input->children[0]->symbolName,"VAR")){
          //do this (same as above)
          if(input->children[1] == NULL){
              typeExpressionTable* temp = table;
              while(strcmp(temp->name,input->children[0]->lexeme)){
                  temp = temp->next;
              }
              return temp;
          }
          else if(!strcmp(input->children[1]->symbolName,"LSQUARE")){
            //do this
              int numNums = 0;
              // while(input->children[2]->children[numNums]!=NULL) numNums++;
              parseTree* rec = input->children[2];
              while(rec){
                  numNums++;
                  rec = rec->children[1];
              }
              rec = input->children[2];
              typeExpressionTable* temp = table;
              while(strcmp(temp->name,input->children[0]->lexeme)){
                  temp = temp->next;
              }
              if (temp->type == jagged_array){
                  if (numNums != temp->exp->c->dimensions)
                      return NULL;
                  jagged* dim_details = temp->exp->c;
                  int dimensions = temp->exp->c->dimensions;
                  if(dimensions == 2){
                      int low_1 = dim_details->low;
                      int high_1 = dim_details->high;
                      int s1 = atoi(rec->children[0]->lexeme);
                      int s2 = atoi(rec->children[1]->children[0]->lexeme);
                      if(s1<low_1 || s1>high_1)
                          return NULL;
                      jagged_dimension* inner_details = dim_details->d;
                      while(s1>low_1){
                          inner_details = inner_details->next;
                          s1--;
                      }
                      int high_2 = inner_details->size;
                      if(s2>=high_2)
                          return NULL;

                      typeExpressionTable* temp_ = (typeExpressionTable*)malloc(sizeof(typeExpressionTable));
                      temp_->name = (char*)malloc(sizeof(char)*strlen("non_terminal"));
                      strcpy(temp_->name,"non_terminal");
                        temp_->next = NULL;
                      temp_->type = primitive;
                      temp_->exp = (expression *)malloc(sizeof(expression));
                      temp_->exp->a = (prim *)malloc(sizeof(prim));
                      temp_->exp->a->basicElementType = (char *)malloc(sizeof(char)*strlen("INTEGER"));
                      strcpy(temp_->exp->a->basicElementType,"INTEGER");
                      return temp_;
                  }
                  else if(dimensions == 3){
                      int low_1 = dim_details->low;
                      int high_1 = dim_details->high;
                      int s1 = atoi(rec->children[0]->lexeme);
                      int s2 = atoi(rec->children[1]->children[0]->lexeme);
                      int s3 = atoi(rec->children[1]->children[1]->children[0]->lexeme);
                      if(s1<low_1 || s1>high_1)
                          return NULL;
                      jagged_dimension* inner_details = dim_details->d;
                      while(s1>low_1){
                          inner_details = inner_details->next;
                          s1--;
                      }
                      int high_2 = inner_details->size;
                      if(s2>=high_2)
                          return NULL;
                      dimension* inner_inner_details = inner_details->inner_size;
                      int high_3 = 1;
                      while(inner_inner_details->next != NULL){
                        high_3++;
                        inner_inner_details = inner_inner_details->next;
                      }
                      if(s3>=high_3)
                          return NULL;

                      typeExpressionTable* temp_ = (typeExpressionTable*)malloc(sizeof(typeExpressionTable));
                      temp_->name = (char*)malloc(sizeof(char)*strlen("non_terminal"));
                      strcpy(temp_->name,"non_terminal");
                        temp_->next = NULL;
                      temp_->type = primitive;
                      temp_->exp = (expression *)malloc(sizeof(expression));
                      temp_->exp->a = (prim *)malloc(sizeof(prim));
                      temp_->exp->a->basicElementType = (char *)malloc(sizeof(char)*strlen("INTEGER"));
                      strcpy(temp_->exp->a->basicElementType,"INTEGER");
                      return temp_;
                  }
              }
              else if (temp->type == rect_array){
                  if (numNums != temp->exp->b->dimensions)
                      return NULL;
                  rect_dimension* temp_rect = temp->exp->b->d;
                  while(temp_rect!=NULL){
                      int dim_ = atoi(rec->children[0]->symbolName);
                      int high = atoi(temp_rect->high);
                      int low = atoi(temp_rect->low);
                      if(dim_<low || dim_>high)
                          return NULL;
                      temp_rect = temp_rect->next;
                      rec = rec->children[1];
                  }
              }
            }
        }
        else if(!strcmp(input->children[0]->symbolName,"id")){
            // printf("Inside factor id\n\n\n\n");
            if(!strcmp(input->children[0]->children[0]->symbolName,"VAR")){
                typeExpressionTable* temp = table;
                while(strcmp(temp->name,input->children[0]->children[0]->lexeme)){
                    temp = temp->next;
                }
                return temp;
            }
            else if (!strcmp(input->children[0]->children[0]->symbolName,"NUMBER")){
                typeExpressionTable* temp = (typeExpressionTable*)malloc(sizeof(typeExpressionTable));
                temp->name = (char*)malloc(sizeof(char)*strlen("non_terminal"));
                strcpy(temp->name,"non_terminal");
                temp->next = NULL;
                temp->type = primitive;
                temp->exp = (expression *)malloc(sizeof(expression));
                temp->exp->a = (prim *)malloc(sizeof(prim));
                temp->exp->a->basicElementType = (char *)malloc(sizeof(char)*strlen("INTEGER"));
                strcpy(temp->exp->a->basicElementType,"INTEGER");
                return temp;
            }
            else{
                printf("This should not have occured id!->VAR || NUMBER");
                return NULL;
            }
        }
        else{
            printf("This should not have occured factor !-> VAR || id\n");
            return NULL;
        }
    }
}

int compare(typeExpressionTable* a, typeExpressionTable* b, int op){

    if (op == 1 && a->type == primitive && !strcmp(a->exp->a->basicElementType,"BOOLEAN") &&
        b->type == primitive && !strcmp(b->exp->a->basicElementType,"BOOLEAN")) {
        return 1;
    } else if (op == 2){
        if ((a->type == primitive && !strcmp(a->exp->a->basicElementType,"INTEGER") &&
            b->type == primitive && !strcmp(b->exp->a->basicElementType,"INTEGER")) || 
            (a->type == primitive && !strcmp(a->exp->a->basicElementType,"REAL") &&
            b->type == primitive && !strcmp(b->exp->a->basicElementType,"REAL")))
            return 1;
        else if (a->type == rect_array && b->type == rect_array){
            bool ans = true;
            if (a->exp->b->dimensions != b->exp->b->dimensions) ans = false;
            else {
                rect_dimension* t1 = a->exp->b->d;
                rect_dimension* t2 = b->exp->b->d;
                while (t1 && t2){
                    if (strcmp(t1->low, t2->low) || strcmp(t1->high, t2->high)) ans = false;
                    t1 = t1->next;
                    t2 = t2->next;
                }
            }
            return ans;
        } else if (a->type == jagged_array && b->type == jagged_array) {
            bool ans = true;
            if (a->exp->c->dimensions != b->exp->c->dimensions || a->exp->c->low != b->exp->c->low || a->exp->c->high != b->exp->c->high) ans = false;
            else {
                jagged_dimension* t1 = a->exp->c->d;
                jagged_dimension* t2 = b->exp->c->d;
                while (t1 && t2){
                    if (t1->parent != t2->parent || t1->size != t2->size) ans = false;
                    dimension* dt1 = t1->inner_size;
                    dimension* dt2 = t2->inner_size;
                    while (dt1 && dt2){
                        if (dt1->size != dt2->size) ans = false;
                        dt1 = dt1->next;
                        dt2 = dt2->next;
                    }
                    t1 = t1->next;
                    t2 = t2->next;
                }
            }
            return ans;
        }
    } else if (op == 3){
        if ((a->type == primitive && !strcmp(a->exp->a->basicElementType,"INTEGER") &&
            b->type == primitive && !strcmp(b->exp->a->basicElementType,"INTEGER")) || 
            (a->type == primitive && !strcmp(a->exp->a->basicElementType,"REAL") &&
            b->type == primitive && !strcmp(b->exp->a->basicElementType,"REAL"))) {
            return 1;
        }
    } else {
        return 0;
    }
}

void addAssignment(parseTree** t, typeExpressionTable *T){
    // printf("Inside addAssignment\n%s\n%s\n\n", (*t)->symbolName, (*t)->lexeme);
    // printParseTree(*t);
    int op = -1;
    if (!strcmp((*t)->symbolName, "assignment")){
        (*t)->children[0]->type = getExpression((*t)->children[0], T);
        addAssignment(&(*t)->children[2], T);
        op = 4;
        //compare
    } else if (!strcmp((*t)->symbolName, "factor") || !strcmp((*t)->symbolName, "lhs")) {
        (*t)->type = getExpression(*t, T);
        return;
    } else {
        if ((*t)->children[1] == NULL){
            addAssignment(&(*t)->children[0], T);
            (*t)->type = (*t)->children[0]->type;
            return;
        }
        if (!strcmp((*t)->children[1]->symbolName, "OP_OR") || !strcmp((*t)->children[1]->symbolName, "OP_AND"))
            op = 1; //boolean
        else if (!strcmp((*t)->children[1]->children[0]->symbolName, "OP_PLUS") || !strcmp((*t)->children[1]->children[0]->symbolName, "OP_MINUS") || !strcmp((*t)->children[1]->children[0]->symbolName, "OP_MULT")) 
            op = 2; //integer
        else if (!strcmp((*t)->children[1]->children[0]->symbolName, "OP_DIV"))
            op = 3; //division
        if (!strcmp((*t)->children[0]->symbolName, "factor")){
            (*t)->children[0]->type = getExpression((*t)->children[0], T);
        } else {
            addAssignment(&(*t)->children[0], T);
        }
        if (!strcmp((*t)->children[2]->symbolName, "factor")){
            (*t)->children[2]->type = getExpression((*t)->children[2], T);
        } else {
            addAssignment(&(*t)->children[2], T);
        }
    }
    if ((*t)->children[0]->type == NULL || (*t)->children[2]->type == NULL) {
        (*t)->type = NULL;
        // printf("Error aa gayi %s %d\n", (*t)->symbolName, (*t)->lineNumber);
    } else {
        if (op == 1){
            // if ((*t)->children[0]->type->type == primitive && !strcmp((*t)->children[0]->type->exp->a->basicElementType,"BOOLEAN") &&
            //     (*t)->children[2]->type->type == primitive && !strcmp((*t)->children[2]->type->exp->a->basicElementType,"BOOLEAN")) {
            if (compare((*t)->children[0]->type, (*t)->children[2]->type, op)){
                (*t)->type = (*t)->children[0]->type;
            } else {
                (*t)->type = NULL;
                printf("Type Error\
                        Line Number: %d\
                        Statement Type: Assignment\
                        Operator\
                        First Lexeme\n", (*t)->lineNumber);
                printTypeExpressionTable((*t)->children[0]->type);
                printf("Second Lexeme\
                        Second Type: ");
                printTypeExpressionTable((*t)->children[2]->type);
                printf("Error aa gayi 1 %s %d\n", (*t)->symbolName, (*t)->lineNumber);
            }
        } else if (op == 2){
            if (compare((*t)->children[0]->type, (*t)->children[2]->type, op)){
                    (*t)->type = (*t)->children[0]->type;
            } else {
                (*t)->type = NULL;
                // printTypeExpressionTable((*t)->children[0]->type);
                // printTypeExpressionTable((*t)->children[2]->type);
                printf("Type Error\
                        Line Number: %d\
                        Statement Type: Assignment\
                        Operator\
                        First Lexeme\n", (*t)->lineNumber);
                printTypeExpressionTable((*t)->children[0]->type);
                printf("Second Lexeme\
                        Second Type: ");
                printTypeExpressionTable((*t)->children[2]->type);
                printf("Error aa gayi 2 %s %d\n", (*t)->symbolName, (*t)->lineNumber);
            }
        } else if (op == 3){
            if (compare((*t)->children[0]->type, (*t)->children[2]->type, op)){
                typeExpressionTable* temp = (typeExpressionTable*)malloc(sizeof(typeExpressionTable));
                temp->name = (char*)malloc(strlen("non-terminal"));
                strcpy(temp->name, "non-terminal");
                temp->type = primitive;
                temp->array_type = NA;
                temp->exp = (expression*)malloc(sizeof(expression));
                temp->exp->a = (prim*)malloc(sizeof(prim));
                temp->exp->a->basicElementType = (char*)malloc(strlen("REAL"));
                temp->next = NULL;
                strcpy(temp->exp->a->basicElementType,"REAL");
                (*t)->type = temp;
            } else {
                (*t)->type = NULL;
                printf("Type Error\
                        Line Number: %d\
                        Statement Type: Assignment\
                        Operator\
                        First Lexeme\n", (*t)->lineNumber);
                printTypeExpressionTable((*t)->children[0]->type);
                printf("Second Lexeme\
                        Second Type: ");
                printTypeExpressionTable((*t)->children[2]->type);
                printf("Error aa gayi 3 %s %d\n", (*t)->symbolName, (*t)->lineNumber);
            }
        } else if (op == 4){
            if (compare((*t)->children[0]->type, (*t)->children[2]->type, 1) || compare((*t)->children[0]->type, (*t)->children[2]->type, 2))
                (*t)->type = (*t)->children[0]->type;
            else {
                (*t)->type = NULL;
                // printTypeExpressionTable((*t)->children[0]->type);
                // printTypeExpressionTable((*t)->children[2]->type);
                printf("Type Error\
                        Line Number: %d\
                        Statement Type: Assignment\
                        Operator\
                        First Lexeme\n", (*t)->lineNumber);
                printTypeExpressionTable((*t)->children[0]->type);
                printf("Second Lexeme\
                        Second Type: ");
                printTypeExpressionTable((*t)->children[2]->type);
                printf("Error aa gayi 4 %s %d\n", (*t)->symbolName, (*t)->lineNumber);
            }
        }
    }
}

// Function Definition: traverseParseTree (parseTree *t, typeExpressionTable T)
int traverseParseTree (parseTree **t, typeExpressionTable **T){
    // printf ("Inside traverseParseTree.\n");
    if (t != NULL && !strcmp((*t)->symbolName, "declaration")) {
        addDeclaration(t, T);
        return 0;
    }
    if (t != NULL && !strcmp((*t)->symbolName, "assignment")) {
        addAssignment(t, *T);
        return 0;
    }
    for (int i = 0; i< MAX_PARSE_TREE_CHILDREN; i++){
        if ((*t)->children[i])
            traverseParseTree(&((*t)->children[i]), T);
    }
    return 0;
}

// Function Definition: printTypeExpressionTable (typeExpressionTable T)
int printTypeExpressionTable (typeExpressionTable *T){
    printf ("Inside printTypeExpressionTable.\n");
    printf("Field1\t\tField2\t\tField3\t\tField4\n");
    typeExpressionTable* l = T;
    // while(l){
        printf("%s\t", l->name);
        if (l->type == primitive)
            printf("Primitive\t");
        else if (l->type == rect_array)
            printf("Rectangular Array\t");
        else if (l->type == jagged_array)
            printf("Jagged Array\t");
        if (l->array_type == stat)
            printf("Static\t");
        else if (l->array_type == dyn)
            printf("Dynamic\t");
        else if (l->array_type == NA)
            printf("Not Applicable\t");
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
        printf("\n");
        // l = l->next;
    // }
    return 0;
}
