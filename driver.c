#include <stdio.h>

int main(int argc, char* argv[]){


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
                printf ("Parse tree creation: yet to be implemented\n\n");
                break;
            }
            case 2:{
                printf ("Parse tree traversal: yet to be implemented\n\n");
                break;
            }
            case 3:{
                printf ("Parse tree printing: yet to be implemented\n\n");
                break;
            }
            case 4:{
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