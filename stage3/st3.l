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

if {
	return IF;
	}
then {
	return THEN;	
	}
else {
	return ELSE;
	}
endif {
	return ENDIF;
	}

while {
	return WHILE;
	}

do {
	return DO;
}

endwhile {
	return ENDWHILE;
	}

break {
	return BREAK;
}

continue {
	return CONTINUE;
}
"<"	return LT;
">"	return GT;
"<="	return LE;
">="	return GE;
"=="	return EQ;
"!="	return NE;
"+"	return ADD;
"-"	return SUB;
"*"	return MUL;
"/"	return DIV;

[a-z] {
		yylval = createVarNode(*yytext);
		return ID;}

[0-9]+ { val = atoi(yytext);
		yylval = createNumNode(val);
		return NUM;}
		
[ \t\n] 	{}

;		{return SEMI;}
		
[()] {return *yytext;}


[\=] {
		return *yytext;
		}
		
. {	yyerror("unknown character\n");
	exit(1);
	}

%%




