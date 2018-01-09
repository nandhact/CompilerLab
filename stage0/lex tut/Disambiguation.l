
%{		
	#include <stdlib.h>
	#include <stdio.h>
%}



number [0-9]+           
zero [0]+


%%


 /* Rules in this order*/
 

{zero} {printf("zero");}
{number} {printf("number");}
 

%%

/* Auxiliary functions */


int yywrap()
{
		return 1;
}


int main()
{
    
    yyin = stdin;
    yylex();
    printf("after yylex\n");
    return 1;
}





