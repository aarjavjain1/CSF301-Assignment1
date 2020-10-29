/*
Group Number: 55
Group Member details:
1. Aarjav Jain -- 2018A7PS0222P
2. Pranav Gupta -- 2018A7PS0190P
3. Harsh Sulakhe -- 2018A7PS0186P
*/

#include <stdio.h>

#include "datastructuresandprototypes.h"

#define GRAMMAR_FILE_PATH "grammar.txt"

int main(int argc, char* argv[]) {

    if (argc != 2){
        printf ("Usage: ./output testcase.txt\n");
        return 0;
    }
    char* SOURCE_CODE_FILE_PATH = argv[1];

    grammarNode* Grammar[NUMBER_OF_GRAMMAR_RULES];
    tokenStream* TokenStreamPointer = NULL;
    parseTree* ParseTree = NULL;
    typeExpressionTable* TypeExpressionTable = NULL;
    readGrammar(GRAMMAR_FILE_PATH, Grammar);
    TokenStreamPointer = tokeniseSourcecode(SOURCE_CODE_FILE_PATH, TokenStreamPointer);
    // Menu
    int menu_choice = 0;
    do {
        printf("Please choose an option:\n");
        printf("0. Exit\n");
        printf("1. Create Parse Tree\n");
        printf("2. Traverse the parse tree to construct typeExpressionTable and print errors\n");
        printf("3. Print parse tree in the specified format\n");
        printf("4. Print typeExpressionTable in the specified format.\n");

        scanf("%d", &menu_choice);

        switch (menu_choice) {
        case 0: {
            printf("Exiting.\n");
            break;
        }
        case 1: {
            if (ParseTree == NULL){
                ParseTree = createParseTree(ParseTree, TokenStreamPointer, Grammar);
                printf("Parse Tree Created\n");
            }
            else
                printf("Parse Tree already exists!\n");
            break;
        }
        case 2: {
            if (TypeExpressionTable == NULL){
                traverseParseTree(&ParseTree, &TypeExpressionTable);
                printf("Traversal Complete\n");
            }
            else
                printf("Type Expression Table already exists!\n");
            break;
        }
        case 3: {
            if (ParseTree == NULL){
                printf("Parse Tree does not exist. Please create it first!\n");
            }
            else
                printParseTree(ParseTree);
            break;
        }
        case 4: {
            if (TypeExpressionTable == NULL){
                printf("Type Expression Table does not exist. Please create it first!\n");
            }
            else
                printTypeExpressionTable(TypeExpressionTable);
            break;
        }
        default: {
            printf("Please choose a valid option.\n\n");
            break;
        }
        }
    } while (menu_choice != 0);

    return 0;
}
