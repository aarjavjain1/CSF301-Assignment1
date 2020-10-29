/*
Group Number: 55
Group Member details:
1. Aarjav Jain -- 2018A7PS0222P
2. Pranav Gupta -- 2018A7PS0190P
3. Harsh Sulakhe -- 2018A7PS0186P
*/

#include <ctype.h>  // isalpha(), isalnum(), isnumber()
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // strlen(), strcpy(), memset()

int isNumber(char* str) {
    // printf("isNumber %s\n",str);
    for (int yu = 0; yu < (int)strlen(str); ++yu) {
        printf("%c\n", str[yu]);
        if (!isdigit(str[yu]))
            return 0;
    }
    return 1;
}

void main() {
    char* str = (char*)malloc(100);

    scanf("%s", str);
    printf("%d", isNumber(str));
}
