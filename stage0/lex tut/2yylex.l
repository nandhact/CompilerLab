
%option noyywrap

%{
#include<stdio.h>
#include<stdlib.h>
char* file1;
%}


number [0-9]+

%%

{number} {printf("number ");}


%%

int main(){

file1="test.txt";

FILE * myfd = fopen("test.txt","r");
yyin = myfd;
yylex();

//If yylex() is called more than once, it simply starts scanning from the position in the input file where it had returned in the previous call. 

printf("once ended\n");\
//yyin = fopen("test.txt","r");

yylex();
printf("second\n");

return 0;

}


