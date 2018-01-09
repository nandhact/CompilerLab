%{
	#include<stdio.h>
	#include<stdlib.h>
	#include"y.tab.h"
	int val;
%}
%option noyywrap

%%

begin { 
		return BEG;
		}
end {	
		return END;
	}
	
read	{
		return READ;
		}
write {
		return WRITE;
	}


[a-z] {
		yylval = createVarNode(*yytext);
		return ID;}

[0-9]+ { val = atoi(yytext);
		yylval = createNumNode(val);
		return NUM;}
		
[ \t\n] 	{}

;		{return SEMI;}
		
[()] {return *yytext;}

[\+\-\*\/] 	{
		return *yytext;}
[\=] {
		return *yytext;
		}
. {	yyerror("unknown character\n");
	exit(1);
	}

%%




