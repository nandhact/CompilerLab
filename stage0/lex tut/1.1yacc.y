%{

#include <stdio.h>
#include <stdlib.h>


/* Variable to keep track of the nesting */
int nest=0;
%}


%token IF
%token THEN
%token ELSE
%token BOOL

%%

start : stmt '\n'		{printf("\nComplete\n");exit(1);}
		;
		
stmt : IF BOOL THEN stmt ELSE stmt {printf("Nesting: %d\n",nest); nest++;}
		| '{' stmt '}' 
		| BOOL
		;

%%
void yyerror()
{
	printf("Error\n");
}

int yylex()
{
	int c;
	c = getchar();
	if(c == ' '){
        yylex();         /*This is to ignore whitespaces in the input*/
    }
    else {
		return c;
	}
}

int main()
{
	yyparse();
	return 1;
}


