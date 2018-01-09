%{
	#include <stdio.h>
	#include <stdlib.h>

	#define YYSTYPE tnode *

	#include "exptree.h"
	#include "exptree.c"
	FILE *fp;
	int yylex(void);
%}

%token NUM PLUS MINUS MUL DIV END
%left PLUS MINUS
%left MUL DIV

%%

program : expr1 END	{
				prefix($1);
				printf("\n");
				postfix($1);
				printf("\nAnswer : %d\n",evaluate($1));
				fprintf(fp,"%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n",0,2056,0,0,0,0,1,0);
				codeGen($1,fp);
				fprintf(fp,"MOV [4096], R0\n");
				write_xsm("R0",fp);
				fprintf(fp,"INT 10\n");
				exit(1);
			}
		;

expr : expr PLUS expr		{$$ = makeOperatorNode('+',$1,$3);}
	 | expr MINUS expr  	{$$ = makeOperatorNode('-',$1,$3);}
	 | expr MUL expr	{$$ = makeOperatorNode('*',$1,$3);}
	 | expr DIV expr	{$$ = makeOperatorNode('/',$1,$3);}
	 | '(' expr ')'		{$$ = $2;}
	 | NUM			{$$ = $1;}
	 ;

expr1 :  PLUS expr1 expr1		{$$ = makeOperatorNode('+',$2,$3);}
	 |  MINUS expr1 expr1  	{$$ = makeOperatorNode('-',$2,$3);}
	 |  MUL expr1 expr1	{$$ = makeOperatorNode('*',$2,$3);}
	 |  DIV expr1 expr1	{$$ = makeOperatorNode('/',$2,$3);}
	 | '(' expr1 ')'		{$$ = $2;}
	 | NUM			{$$ = $1;}
	 ;
	 
%%

yyerror(char const *s)
{
    printf("yyerror %s",s);
}


int main(void) {
	
	reg = 0;
	fp = fopen("output.xsm","w");
	yyparse();
	return 0;
}

