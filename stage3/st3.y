%{
	#include<stdio.h>
	#include<stdlib.h>
	#include"st3.h"
	#include"st3.c"
	#define YYSTYPE tnode*
	extern FILE *yyin;
	FILE* fout;
%}


%token BEG END READ WRITE NUM SEMI ID IF THEN ELSE ENDIF WHILE DO ENDWHILE
%token LT "<"
%token GT ">"
%token LE "<="
%token GE ">="
%token EQ "=="
%token NE "!="
%token ADD "+"
%token SUB "-"
%token MUL "*"
%token DIV "/"
%left ADD SUB
%left MUL DIV
%nonassoc LT GT LE GE EQ NE

%%

prog : BEG Slist END SEMI {
		//printTree($2);
		//eval($2);
		fprintf(fout,"%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\nMOV SP, 4121\n",0,2056,0,0,0,0,1,0);
		codeGen($2,fout);
		fprintf(fout,"INT 10\n");
		printf("\nSuccessfully parsed program\n");
		exit(1);
		}
	 | BEG END SEMI{
	 	printf("Successfully parsed empty program\n");
	 	exit(1);
		};
	 	
Slist : Slist Stmt {
		$$ = createTree(NULL,NULL, NULL,tCONNECT, $1,NULL, $2);
	}
	| Stmt {
		$$=$1;
	};
Stmt : InputStmt {$$=$1;}
		| OutputStmt{$$=$1;}
		| AsgStmt {$$=$1;}
		|IfStmt {$$=$1;}
		|WhileStmt{$$=$1;};

InputStmt: READ '(' ID ')' SEMI {$$= createReadNode($3);};

OutputStmt: WRITE '(' Expr ')' SEMI {$$= createWriteNode($3);};


AsgStmt: ID '=' Expr SEMI {	
							$$ = createAsgNode($1, $3);
							};

IfStmt: IF '(' Expr ')' THEN Slist ELSE Slist ENDIF SEMI{
							$$ = createIfNode($3,$6,$8);
						}
		| IF '(' Expr ')' THEN Slist ENDIF SEMI{
							$$ = createIfNode($3,$6,NULL);
						}
WhileStmt: WHILE '(' Expr ')' DO Slist ENDWHILE SEMI{
							$$ = createWhileNode($3,$6);
							}

Expr : Expr "+" Expr	{
							$$ = createOpNode(tADD,$1,$3);
						}
						
	| Expr "-" Expr  	{
								$$ = createOpNode(tSUB,$1,$3);
						}
	| Expr "*" Expr		{
								$$ = createOpNode(tMUL,$1,$3);
						}
	| Expr "/" Expr		{
								$$ = createOpNode(tDIV,$1,$3);
						}
	| Expr "<" Expr		{		
								$$ = createOpNode(tLT,$1,$3);
						}
	| Expr ">" Expr		{	
								$$ = createOpNode(tGT,$1,$3);
						}
	| Expr "<=" Expr		{
								$$ = createOpNode(tLE,$1,$3);
						}
	| Expr ">=" Expr		{
								$$ = createOpNode(tGE,$1,$3);
						}
	| Expr "==" Expr	{
							$$ = createOpNode(tEQ,$1,$3);
						}
	| Expr "!=" Expr	{
							$$ = createOpNode(tNE,$1,$3);
						}
	| '(' Expr ')'		{$$ = $2;}
	| NUM				{$$ = $1;}
	| ID				{$$ = $1;}
	;
%%

yyerror(char const *s)
{
    printf("yyerror %s\n",s);
}


int main(int argc, char* argv[])
{
	
	reg = 0;
	if(argc > 2)
	{
		FILE *fp = fopen(argv[1], "r");
		fout = fopen(argv[2], "w");
		if(fp)
			yyin = fp;
		
	} else {
		//printf("\nExpected filename for input and output\n ./a.out <inputfile> <outputfile>\n\n");
		//return 0;
	}
	yyparse();
	return 0;
}

