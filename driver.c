#include <stdio.h>

#include "customcompiler.h"

#define GRAMMAR_FILE_PATH "grammar.txt"
#define SOURCE_CODE_FILE_PATH "sourcecode.txt"

int main(int argc, char* argv[]){
    // TODO
    // Argument checking and parsing goes here

    // Data Structures initialisation
    grammarNode* Grammar[NUMBER_OF_GRAMMAR_RULES];
    tokenStream* TokenStreamPointer = NULL;
    parseTree* ParseTree = NULL;
    typeExpressionTable TypeExpressionTable;
    // Menu
    int menu_choice = 0;
    do {
        printf ("Please choose an option:\n");
        printf ("0. Exit\n");
        printf ("1. Create Parse Tree\n");
        printf ("2. Traverse the parse tree to construct typeExpressionTable and print errors\n");
        printf ("3. Print parse tree in the specified format\n");
        printf ("4. Print typeExpressionTable in the specified format.\n");

        scanf ("%d", &menu_choice);

        switch (menu_choice){
            case 0:{
                printf ("Exiting.\n");
                break;
            }
            case 1:{
                // The variables used as actual parameters here have been declared above the do while menu loop, inside the main function
                readGrammar(GRAMMAR_FILE_PATH, Grammar);
                TokenStreamPointer = tokeniseSourcecode(SOURCE_CODE_FILE_PATH, TokenStreamPointer);
                tokenStream* test =TokenStreamPointer;
                while(test != NULL){
                printf("%s\n",test->tokenName );
                test = test->next;
                }
                printf ("tokenised\n");
                // for (int i = 0; i<NUMBER_OF_GRAMMAR_RULES; i++){
                //     printf("%d", i);print_grammar_rule(Grammar[i]);
                // }
                createParseTree(ParseTree, TokenStreamPointer, Grammar);

                printf ("Parse tree creation works\n\n");
                break;
            }
            case 2:{
                // The variables used as actual parameters here have been declared above the do while menu loop, inside the main function
                traverseParseTree(ParseTree, TypeExpressionTable);

                printf ("Parse tree traversal: yet to be implemented\n\n");
                break;
            }
            case 3:{
                // The variables used as actual parameters here have been declared above the do while menu loop, inside the main function
                printParseTree(ParseTree);

                printf ("Parse tree printing: yet to be implemented\n\n");
                break;
            }
            case 4:{
                // The variables used as actual parameters here have been declared above the do while menu loop, inside the main function
                printTypeExpressionTable(TypeExpressionTable);

                printf ("typeExpressionTable printing: yet to be implemented\n\n");
                break;
            }
            default:{
                printf("Please choose a valid option.\n\n");
                break;
            }
        }
    }while(menu_choice != 0);

    return 0;
}
