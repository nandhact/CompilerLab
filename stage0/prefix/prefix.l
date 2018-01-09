%{
    #include<stdio.h> 
    #include<stdlib.h> 
	  #include "y.tab.h"
%}

%%

[a-zA-Z]+	{
			
         	yylval = malloc(20*sizeof(char));
         	strcpy(yylval,yytext);
         	//printf("\nlexeme: %s\n",yylval);
          	return CHAR;
        }
\+	{
	     return ADD;
	    }
\-		{
	     return SUB;
	    }
\*		{
	     return MUL;
	    }
\/		{
	     return DIV;
	    }	    	    
[()]	  return *yytext;
[\n]      return NEWLINE;

%%

yywrap()
{
	return 1;
}
