%{
	#include<stdio.h>
	#include<stdlib.h>
	#include<string.h>
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
repeat {
	return REPEAT;
}

until {
	return UNTIL;
}

decl {
	return DECL;	
}

enddecl {
	return ENDDECL;
}

int {
	return INT;
}

str {
	return STR;
}
breakpoint {
	return BRKP;
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

[a-z]+ {//printf("%s lexeme\n",yytext);
		char * temp = strdup(yytext);
		yylval = createVarNode(temp);
		return ID;}
		
"[" {return *yytext;}
"]" {return *yytext;}
"&" {return *yytext;}

[\"].*[\"] {
		char * temp = strdup(yytext);
		yylval = createLiteralNode(temp);
		return LIT;
	}

[0-9]+ { val = atoi(yytext);
		yylval = createNumNode(val);
		return NUM;}
		
[ \t\n] 	{}

;		{return SEMI;}
		
[()] {return *yytext;}


[\=] {
		return *yytext;
		}

"," {
		return COMMA;
}
. {	yyerror("unknown character\n");
	exit(1);
	}

%%




