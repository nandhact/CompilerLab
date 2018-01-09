%{
    #include<stdio.h> 
    #include<stdlib.h> 
	#include "y.tab.h"
%}

%%


if {
		return IF;
	}
endif
	{
		return ENDIF;
	}

	
[0-9 ]+ {
		yylval = malloc(30);
		strcpy(yylval, yytext);
		return CHAR;
}
\n {return NEWLINE;}
. ;

%%

yywrap()
{
	return 1;
}
