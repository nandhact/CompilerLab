%{

#include <stdio.h>
#include <stdlib.h>

%}

%token DIGIT
%left '+'
%left '*'

%%


start : expr '\n'		{printf("\nComplete\n");exit(1);}
	;

expr:  expr '+' expr		{printf("+ ");}
	| expr '*' expr		{printf("* ");}
	| '(' expr ')'
	| DIGIT			{printf("%d ",$1);}
	;

%%
void yyerror()
{
	printf("Error");
}

int yylex()
{
	int c;
	c = getchar();
	if(isdigit(c))
	{
		yylval = c - '0';
		return DIGIT;

	}
	return c;
}

int main()
{
	yyparse();
	return 1;
}


