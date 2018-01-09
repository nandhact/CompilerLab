%{
    #include<stdio.h>     
    #include<stdlib.h> 
	  #include "y.tab.h"
%}

%%

[a-zA-Z]+	{
          char * temp = strdup(yytext);
          yylval = temp;
          return CHAR;
        }
"+"	  return *yytext;
"*"	  return *yytext;
[()]	  return *yytext;
[\n]      return NEWLINE;
. ;
%%

yywrap()
{
	return 1;
}
