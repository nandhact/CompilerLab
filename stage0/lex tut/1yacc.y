%{

#include <stdio.h>
#include <stdlib.h>


/* Variable to keep track of the nesting */
int nest=0;
%}

%token LETTER
%token DIGIT


%%

start: var {printf("valid variable\n");exit(1);}
	;
	
var: LETTER any
	;
	
any: LETTER|DIGIT
	;

/*start : stmt '\n'		{printf("\nComplete\n");exit(1);}
		;
		
stmt : 'i''f' B 't''h''e''n' stmt 'e''l''s''e' stmt {printf("Nesting: %d\n",nest); nest++;}
		| '{' stmt '}' 
		| B
		;
B: 't''r''u''e' | 'f''a''l''s''e';*/



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
    else if(isdigit(c))
	{
		return DIGIT;

	}
    else if(isalpha(c)){
		return LETTER;
	} else 
	{return c;}
}

int main()
{
	yyparse();
	return 1;
}


