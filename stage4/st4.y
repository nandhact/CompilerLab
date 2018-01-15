%{
	#include<stdio.h>
	#include<stdlib.h>
	#include <limits.h>
	#include<string.h>
	#include"st4.h"
	#include"st4.c"
	#define YYSTYPE tnode*
	extern FILE *yyin;
	FILE* fout;
%}


%token BEG END READ WRITE NUM SEMI ID IF THEN ELSE ENDIF WHILE DO ENDWHILE BREAK CONTINUE REPEAT UNTIL
%token DECL ENDDECL INT STR
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
%token COMMA ","
%left ADD SUB
%left MUL DIV
%nonassoc LT GT LE GE EQ NE

%%

prog : Declarations BEG Slist END SEMI {
		showST();
		fprintf(fout,"%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\nMOV SP, 4121\n",0,2056,0,0,0,0,1,0);
		codeGen($3,fout);
		fprintf(fout,"INT 10\n");
		printf("\nSuccessfully parsed program\n");
		exit(1);
		}
	 | Declarations BEG END SEMI{
	 	showST();
	 	printf("Successfully parsed empty program\n");
	 	exit(1);
		};
Declarations : DECL DeclList ENDDECL {$$=$2;}
			| DECL ENDDECL	{$$=createTree(NULL,NULL,NULL,NULL,NULL,NULL,NULL);};

DeclList : DeclList Decl	{$$ = createTree(NULL,NULL, NULL,tDCONNECT, $1,NULL, $2);}
		| Decl 				{$$=$1;};

Decl : Type VarList SEMI	{
							$$=assignTypeDecl($1,$2);
							};
							

Type :	INT					{$$=createTypeNode(intType);}
		|STR				{$$=createTypeNode(stringType);};

VarList :VarList COMMA ID {	//Problem: $3 is holding whole string::::Solution: don't pass yytext
									if(lookupSymbol($3->varname) == NULL){	
										insertSymbol($3->varname,NULL,1,getHeapSpace(1));
										$3->middle=$1;
										$$=$3; 
							} else {
								yyerror("Variable already declared\n");
								exit(1);
							}
							}
		| ID {
				if(lookupSymbol($1->varname) == NULL){
					insertSymbol($1->varname,NULL,1,getHeapSpace(1));
					$$=$1;
				} else {
					yyerror("Variable already declared\n");
					exit(1);
				}
				};

Slist : Slist Stmt SEMI{
		$$ = createTree(NULL,NULL, NULL,tCONNECT, $1,NULL, $2);
	}
	| Stmt SEMI{
		$$=$1;
	};
Stmt : InputStmt	//defaults to $$=$1
		|OutputStmt
		|AsgStmt 
		|IfStmt 
		|WhileStmt
		|BrkContStmt
		|DoWhileStmt
		|RepeatStmt;

InputStmt: READ '(' ID ')'		{if(lookupSymbol($3->varname) != NULL){
									$3->Gentry = lookupSymbol($3->varname);
								} else {
									yyerror("Variable undeclared\n");
									exit(1);
								}
								$$= createReadNode($3);};

OutputStmt: WRITE '(' Expr ')'	{	$$= createWriteNode($3);};


AsgStmt: ID '=' Expr			{	if(lookupSymbol($1->varname) != NULL){
									$1->Gentry = lookupSymbol($1->varname);
								} else {
									yyerror("Variable undeclared\n");
									exit(1);
								}
									$$ = createAsgNode($1, $3);};

IfStmt: IF '(' Expr ')' THEN Slist ELSE Slist ENDIF {
							$$ = createIfNode($3,$6,$8);
						}
		| IF '(' Expr ')' THEN Slist ENDIF {
							$$ = createIfNode($3,$6,NULL);
						};
						
WhileStmt: WHILE '(' Expr ')' DO Slist ENDWHILE {
							$$ = createWhileNode($3,$6);
							};

BrkContStmt: BREAK { $$=createBreakNode();
					}
			| CONTINUE {
					$$=createContinueNode();
					};
DoWhileStmt: DO Slist WHILE '(' Expr ')' {
								$$ = createDoWhileNode($2,$5);
								};
RepeatStmt: REPEAT Slist UNTIL '(' Expr ')' {
								$$ = createRepeatNode($2,$5);
								};
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
	| ID				{	if(lookupSymbol($1->varname) != NULL){
								$1->Gentry = lookupSymbol($1->varname);
								$$=$1;
							} else {
								yyerror("Variable undeclared\n");
								exit(1);
							}
						$$ = $1;}
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

