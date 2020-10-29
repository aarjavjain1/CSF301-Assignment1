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

int readGrammar(char* grammarFilePath, grammarNode** G) {
    printf("Inside readGrammar.\n");
    FILE* fp;
    char* token;
    fp = fopen(grammarFilePath, "r");
    if (fp == NULL) {
        perror("Failed: ");
        return 1;
    }
    char* buffer = (char*)malloc(sizeof(char) * MAX_LEN);

    int line_num = 0;
    while (fgets(buffer, MAX_LEN - 1, fp)) {
        // Remove trailing newline
        buffer[strcspn(buffer, "\n")] = 0;
        buffer = trimwhitespace(buffer);

        token = strtok(buffer, " ");
        grammarNode* temp = (grammarNode*)malloc(sizeof(grammarNode));
        temp->grammarRuleNum = line_num + 1;
        grammarNode* head = temp;
        temp->grammarWord = (char*)malloc(sizeof(char) * (strlen(token) + 1));
        strcpy(temp->grammarWord, token);
        token = strtok(NULL, " ");
        /* walk through other tokens */
        while (token != NULL) {
            if (strcmp(token, "->") == 0) {
                token = strtok(NULL, " ");
                continue;
            }
            temp->next = (grammarNode*)malloc(sizeof(grammarNode));
            temp = temp->next;
            temp->grammarRuleNum = line_num + 1;
            temp->next = NULL;
            temp->grammarWord = (char*)malloc(sizeof(char) * (strlen(token) + 1));
            strcpy(temp->grammarWord, token);
            token = strtok(NULL, " ");
        }
        G[line_num++] = head;
    }
    fclose(fp);
    return 0;
}