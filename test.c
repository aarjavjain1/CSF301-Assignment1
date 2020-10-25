#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>       // isalpha(), isalnum(), isnumber()
#include<string.h>      // strlen(), strcpy(), memset()
#include<stdbool.h>

int isNumber(char* str){
    // printf("isNumber %s\n",str);
    for (int yu=0; yu < (int)strlen(str); ++yu){
      printf("%c\n",str[yu] );
        if (!isdigit(str[yu]))
            return 0;
    }
    return 1;
}

void main(){
  char* str = (char*)malloc(100);

  scanf("%s",str);
  printf("%d",isNumber(str));
}
