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
%token DECL ENDDECL INT STR LIT
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
		fprintf(fout,"%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\nMOV SP, %d\n",0,2056,0,0,0,0,1,0,heapSize);
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
			| DECL ENDDECL	{$$=createTree(NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);};

DeclList : DeclList Decl	{$$ = createTree(NULL,NULL, NULL,tDCONNECT,NULL, $1,NULL, $2);}
		| Decl 				{$$=$1;};

Decl : Type VarList SEMI	{
							$$=assignTypeDecl($1,$2);
							};
							

Type :	INT					{$$=createTypeNode(intType);}
		|STR				{$$=createTypeNode(stringType);};

VarList :VarList COMMA ID {	//Problem: $3 is holding whole string::::Solution: don't pass yytext
									if(lookupSymbol($3->varname) == NULL){	
										insertSymbol($3->varname,NULL,tVAR,1,0,getHeapSpace(1));
										$3->middle=$1;
										
								printf("assigning tvar for %s\n",$3->varname);
										$3->nodetype=tVAR;
										$$=$3; 
							} else {
								yyerror("Variable already declared\n");
								exit(1);
							}
							}
		| ID {
				if(lookupSymbol($1->varname) == NULL){
					printf("assigning tvar for %s\n",$1->varname);
					insertSymbol($1->varname,NULL,tVAR,1,0,getHeapSpace(1));
					$1->nodetype=tVAR;
					$$=$1;
				} else {
					yyerror("Variable already declared\n");
					exit(1);
				}
				}
		|ID '[' NUM ']'{
			if(lookupSymbol($1->varname) == NULL){
					insertSymbol($1->varname,NULL,tARR,$3->val,0,getHeapSpace($3->val));
					$1->nodetype=tARR;
					$$=$1;
				} else {
					yyerror("Variable already declared\n");
					exit(1);
				}
		}
		|ID '[' NUM ']' '[' NUM ']'{
			if(lookupSymbol($1->varname) == NULL){
					insertSymbol($1->varname,NULL,tDARR,$3->val,$6->val,getHeapSpace(($3->val)*($5->val)));
					$1->nodetype=tDARR;
					$$=$1;
				} else {
					yyerror("Variable already declared\n");
					exit(1);
				}
		};

Slist : Slist Stmt SEMI{
		$$ = createTree(NULL,NULL, NULL,tCONNECT,NULL, $1,NULL, $2);
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

InputStmt: READ '(' Expr ')'		{if($3->nodetype==tVAR ||$3->nodetype==tARR){
										if(lookupSymbol($3->varname) != NULL){
											$3->Gentry = lookupSymbol($3->varname);
										} else {
											yyerror("Variable undeclared\n");
											exit(1);
										}
									} else {
											yyerror("Expected variable for Read\n");
											exit(1);
									}
								$$= createReadNode($3);
								};

OutputStmt: WRITE '(' Expr ')'	{	$$= createWriteNode($3);};


AsgStmt: ID '=' Expr			{	if(lookupSymbol($1->varname) != NULL){
										$1->Gentry = lookupSymbol($1->varname);
										if(($1->Gentry)->nodetype!=tVAR){
											printf("nodetype is %d\n",($1->Gentry)->nodetype);
											yyerror("Type mismatch: Expected Var Asg\n");
										exit(1);
										}
										$1->nodetype=tVAR;
										$1->type=($1->Gentry)->type;
									} else {
										yyerror("Variable undeclared\n");
										exit(1);
									}
									$$ = createAsgNode($1, $3);
								}
		|ID '[' Expr ']' '=' Expr {	if(lookupSymbol($1->varname) != NULL){
											$1->Gentry = lookupSymbol($1->varname);
											if(($1->Gentry)->nodetype!=tARR){
												yyerror("Type mismatch: Expected Array\n");
												exit(1);
											}
											$1->nodetype=tARR;
											$1->type=($1->Gentry)->type;
											if($3->type==intType){
												if(($3->nodetype==tNUM) && ( $3->val >= $1->Gentry->size)){
													yyerror("ERROR BITTCHHH : Array out of bounds\n");
													exit(1);
												}
											}
											$1->middle=$3;
									} else {
										yyerror("Variable undeclared\n");
										exit(1);
									}				
									$$ = createAsgNode($1, $6);
									}
		|ID '[' Expr ']' '[' Expr ']' '=' Expr {	if(lookupSymbol($1->varname) != NULL){
														$1->Gentry = lookupSymbol($1->varname);
														if(($1->Gentry)->nodetype!=tDARR){
															yyerror("Type mismatch: Expected Double Array\n");
															exit(1);
														}
														$1->nodetype=tDARR;
														$1->type=($1->Gentry)->type;
														if($3->type==intType){
															if(($3->nodetype==tNUM) && ( $3->val >= $1->Gentry->size[0])){
																yyerror("ERROR BITTCHHH : Array out of bounds\n");
																exit(1);
															}
														}
														$1->middle=$3;
														if($6->type==intType){
															if(($6->nodetype==tNUM) && ( $6->val >= $1->Gentry->size[1])){
																yyerror("ERROR BITTCHHH : Array out of bounds\n");
																exit(1);
															}
														}
												$1->right=$6;
											} else {
												yyerror("Variable undeclared\n");
												exit(1);
											}				
											$$ = createAsgNode($1, $9);
											}
							
							;									

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
								$1->type=($1->Gentry)->type;
								if(($1->Gentry)->nodetype!=tVAR){
									yyerror("Type mismatch: Expected Var Expr \n");
										exit(1);
								}
								$1->nodetype=tVAR;
								$$=$1;
							} else {
								yyerror("Variable undeclared\n");
								exit(1);
							}
						$$ = $1;}
	|ID  '[' Expr ']' {
						if(lookupSymbol($1->varname) != NULL){
								$1->Gentry = lookupSymbol($1->varname);
								$1->type=($1->Gentry)->type;
								if(($1->Gentry)->nodetype!=tARR){
									yyerror("Type mismatch: Expected Array\n");
										exit(1);
								}
								$1->nodetype=tARR;
								if($3->type==intType){
									$1->middle = $3;
									if(($3->nodetype==tNUM) && ( $3->val >= $1->Gentry->size)){
										yyerror("ERROR BIITTCHH: Array out of bounds\n");
										exit(1);
									}
								} else {
									yyerror("Expected integer type for array offset\n");
									exit(1);
								}
								$$=$1;
							} else {
								yyerror("Variable undeclared\n");
								exit(1);
							}
	
	}
	|ID  '[' Expr ']' '[' Expr ']' {
						if(lookupSymbol($1->varname) != NULL){
								$1->Gentry = lookupSymbol($1->varname);
								$1->type=($1->Gentry)->type;
								if(($1->Gentry)->nodetype!=tDARR){
									yyerror("Type mismatch: Expected Double Array\n");
										exit(1);
								}
								$1->nodetype=tDARR;
								if($3->type==intType){
									$1->middle = $3;
									if(($3->nodetype==tNUM) && ( $3->val >= $1->Gentry->size[0])){
										yyerror("ERROR BIITTCHH: Array out of bounds\n");
										exit(1);
									}
								} else {
									yyerror("Expected integer type for array offset\n");
									exit(1);
								}
								if($6->type==intType){
									$1->right = $6;
									if(($6->nodetype==tNUM) && ( $6->val >= $1->Gentry->size[1])){
										yyerror("ERROR BIITTCHH: Array out of bounds\n");
										exit(1);
									}
								} else {
									yyerror("Expected integer type for array offset\n");
									exit(1);
								}
								$$=$1;
							} else {
								yyerror("Variable undeclared\n");
								exit(1);
							}
	
	}
	
	| LIT {$$ = $1;}
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

