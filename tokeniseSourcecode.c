/*
Group Number: 55
Group Member details:
1. Aarjav Jain -- 2018A7PS0222P
2. Pranav Gupta -- 2018A7PS0190P
3. Harsh Sulakhe -- 2018A7PS0186P
*/

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "datastructuresandprototypes.h"

bool isVariable(char *str) {
    for (int yu = 0; yu < (int)strlen(str); ++yu) {
        if (yu == 0) {
            if (isalpha(str[yu]) || str[yu] == '_')
                continue;
            else
                return false;
        } else {
            if (isalnum(str[yu]) || str[yu] == '_')
                continue;
            else
                return false;
        }
    }
    return true;
}

bool isNumber(char *str) {
    // printf("isNumber %s\n",str);
    for (int yu = 0; yu < (int)strlen(str); ++yu) {
        if (!isdigit(str[yu]))
            return false;
    }
    return true;
}

int search(char *token) {
    char* tokens[30] = {"declare","colon","list","of","variables","semicolon","array","integer","jagged","boolean","real","lsquare","dot","rsquare","number","r1","gsize","values","lcurly",\
    "rcurly","equals","op_or","op_and","op_plus","op_minus","op_mult","op_div","program","lsimple","rsimple"};

    // FILE *f = fopen("keywords.txt", "r");
    // char buffer[50];
    // while (EOF != fscanf(f, "%[^\n]\n", buffer)) {
    //     if (!strcmp(buffer, token))
    //         return 1;
    // }
    for(int i = 0;i<30;i++){
        if(!strcmp(token,tokens[i]))
            return 1;
    }
    // fclose(f);
    return 0;
}

char *trimwhitespace(char *str) {
    char *end;

    // Trim leading space
    while (isspace((unsigned char)*str)) str++;

    if (*str == 0)  // All spaces?
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    // Write new null terminator character
    end[1] = '\0';

    return str;
}

tokenStream *get_token(char *token, int *line_count) {
    tokenStream *new = (tokenStream *)malloc(sizeof(tokenStream));
    if (search(token) == 1) {
        // printf("NICHT\n");
        char *temp_ = (char *)malloc(sizeof(char) * (strlen(token) + 1));
        for (int yu = 0; yu < (int)strlen(token); ++yu)
            temp_[yu] = toupper(token[yu]);
        temp_[(int)strlen(token)] = '\0';
        //printf("token: %s\n", buff);
        new->tokenName = (char *)malloc(sizeof(char) * (strlen(temp_) + 1));
        strcpy(new->tokenName, temp_);
    } else if (!strcmp(token, "R1")) {
        new->tokenName = (char *)malloc(sizeof(char) * (strlen("R1") + 1));
        strcpy(new->tokenName, "R1");
    } else if (!strcmp(token, "size")) {
        new->tokenName = (char *)malloc(sizeof(char) * (strlen("GSIZE") + 1));
        strcpy(new->tokenName, "GSIZE");
    } else if (isVariable(token)) {
        new->tokenName = (char *)malloc(sizeof(char) * (strlen("VAR") + 1));
        strcpy(new->tokenName, "VAR");
    } else if (isNumber(token)) {
        new->tokenName = (char *)malloc(sizeof(char) * (strlen("NUMBER") + 1));
        strcpy(new->tokenName, "NUMBER");
    } else if (!strcmp(token, "&&&")) {
        new->tokenName = (char *)malloc(sizeof(char) * (strlen("OP_AND") + 1));
        strcpy(new->tokenName, "OP_AND");
    } else if (!strcmp(token, "|||")) {
        new->tokenName = (char *)malloc(sizeof(char) * (strlen("OP_OR") + 1));
        strcpy(new->tokenName, "OP_OR");
    } else if (!strcmp(token, "..")) {
        new->tokenName = (char *)malloc(sizeof(char) * (strlen("DOT") + 1));
        strcpy(new->tokenName, "DOT");
    } else if (strlen(token) == 1) {
        switch (token[0]) {
        case '+': {
            new->tokenName = (char *)malloc(sizeof(char) * (strlen("OP_PLUS") + 1));
            new->lexeme = (char *)malloc(sizeof(char) * (strlen("+") + 1));
            strcpy(new->tokenName, "OP_PLUS");
            strcpy(new->lexeme, "+");
            new->lineNumber = *line_count;
            new->next = NULL;
            return new;
            break;
        }
        case '-': {
            new->tokenName = (char *)malloc(sizeof(char) * (strlen("OP_MINUS") + 1));
            new->lexeme = (char *)malloc(sizeof(char) * (strlen("-") + 1));
            strcpy(new->tokenName, "OP_MINUS");
            strcpy(new->lexeme, "-");
            new->lineNumber = *line_count;
            new->next = NULL;
            return new;
            break;
        }
        case '/': {
            new->tokenName = (char *)malloc(sizeof(char) * (strlen("OP_DIV") + 1));
            new->lexeme = (char *)malloc(sizeof(char) * (strlen("/") + 1));
            strcpy(new->tokenName, "OP_DIV");
            strcpy(new->lexeme, "/");
            new->lineNumber = *line_count;
            new->next = NULL;
            return new;
            break;
        }
        case '[': {
            new->tokenName = (char *)malloc(sizeof(char) * (strlen("LSQUARE") + 1));
            new->lexeme = (char *)malloc(sizeof(char) * (strlen("[") + 1));
            strcpy(new->tokenName, "LSQUARE");
            strcpy(new->lexeme, "[");
            new->lineNumber = *line_count;
            new->next = NULL;
            return new;
            break;
        }
        case ']': {
            new->tokenName = (char *)malloc(sizeof(char) * (strlen("RQSQUARE") + 1));
            new->lexeme = (char *)malloc(sizeof(char) * (strlen("]") + 1));
            strcpy(new->tokenName, "RSQUARE");
            strcpy(new->lexeme, "]");
            new->lineNumber = *line_count;
            new->next = NULL;
            return new;
            break;
        }
        case '{': {
            new->tokenName = (char *)malloc(sizeof(char) * (strlen("LCURLY") + 1));
            new->lexeme = (char *)malloc(sizeof(char) * (strlen("{") + 1));
            strcpy(new->tokenName, "LCURLY");
            strcpy(new->lexeme, "{");
            new->lineNumber = *line_count;
            new->next = NULL;
            return new;
            break;
        }
        case '}': {
            new->tokenName = (char *)malloc(sizeof(char) * (strlen("RCURLY") + 1));
            new->lexeme = (char *)malloc(sizeof(char) * (strlen("}") + 1));
            strcpy(new->tokenName, "RCURLY");
            strcpy(new->lexeme, "}");
            new->lineNumber = *line_count;
            new->next = NULL;
            return new;
            break;
        }
        case '(': {
            new->tokenName = (char *)malloc(sizeof(char) * (strlen("LSIMPLE") + 1));
            new->lexeme = (char *)malloc(sizeof(char) * (strlen("(") + 1));
            strcpy(new->tokenName, "LSIMPLE");
            strcpy(new->lexeme, "(");
            new->lineNumber = *line_count;
            new->next = NULL;
            return new;
            break;
        }
        case ')': {
            new->tokenName = (char *)malloc(sizeof(char) * (strlen("RSIMPLE") + 1));
            new->lexeme = (char *)malloc(sizeof(char) * (strlen(")") + 1));
            strcpy(new->tokenName, "RSIMPLE");
            strcpy(new->lexeme, ")");
            new->lineNumber = *line_count;
            new->next = NULL;
            return new;
            break;
        }
        case ';': {
            new->tokenName = (char *)malloc(sizeof(char) * (strlen("SEMICOLON") + 1));
            new->lexeme = (char *)malloc(sizeof(char) * (strlen(";") + 1));
            strcpy(new->tokenName, "SEMICOLON");
            strcpy(new->lexeme, ";");
            new->lineNumber = *line_count;
            new->next = NULL;
            return new;
            break;
        }
        case ',': {
            new->tokenName = (char *)malloc(sizeof(char) * (strlen("COMMA") + 1));
            new->lexeme = (char *)malloc(sizeof(char) * (strlen(",") + 1));
            strcpy(new->tokenName, "COMMA");
            strcpy(new->lexeme, ",");
            new->lineNumber = *line_count;
            new->next = NULL;
            return new;
            break;
        }
        case '=': {
            new->tokenName = (char *)malloc(sizeof(char) * (strlen("EQUALS") + 1));
            new->lexeme = (char *)malloc(sizeof(char) * (strlen("=") + 1));
            strcpy(new->tokenName, "EQUALS");
            strcpy(new->lexeme, "=");
            new->lineNumber = *line_count;
            new->next = NULL;
            return new;
            break;
        }
        case ':': {
            new->tokenName = (char *)malloc(sizeof(char) * (strlen("COLON") + 1));
            new->lexeme = (char *)malloc(sizeof(char) * (strlen(":") + 1));
            strcpy(new->tokenName, "COLON");
            strcpy(new->lexeme, ":");
            new->lineNumber = *line_count;
            new->next = NULL;
            return new;
            break;
        }

        case '*': {
            new->tokenName = (char *)malloc(sizeof(char) * (strlen("OP_MULT") + 1));
            new->lexeme = (char *)malloc(sizeof(char) * (strlen("*") + 1));
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
    new->lexeme = (char *)malloc(sizeof(char) * (strlen(token) + 1));

    strcpy(new->lexeme, token);
    new->lineNumber = *line_count;
    new->next = NULL;
    return new;
}

// Function Definition: tokeniseSourcecode(  “sourcecode.txt”,  tokenStream  *s)
tokenStream *tokeniseSourcecode(char *sourceCodeFilePath, tokenStream *s) {
    // printf ("Inside tokeniseSourcecode.\n");
    FILE *fp;
    char *token;
    fp = fopen(sourceCodeFilePath, "r");

    char *buffer = (char *)malloc(sizeof(char) * MAX_LEN);
    // -1 to allow room for NULL terminator for really long string
    int line_num = 1;
    tokenStream *temp = NULL;
    tokenStream *head = NULL;
    tokenStream *prev = NULL;

    while (fgets(buffer, MAX_LEN - 1, fp)) {
        // Remove trailing newline
        buffer[strcspn(buffer, "\n")] = 0;
        buffer = trimwhitespace(buffer);
        token = strtok(buffer, " ");
        /* walk through other tokens */
        while (token != NULL) {
            temp = get_token(token, &line_num);
            if (!head)
                head = temp;
            if (prev)
                prev->next = temp;
            prev = temp;
            temp = temp->next;
            token = strtok(NULL, " ");
        }
        line_num++;
    }
    fclose(fp);
    s = head;
    tokenStream *test = head;
    return head;
}
