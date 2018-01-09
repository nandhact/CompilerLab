
%{		
	#include <stdlib.h>
	#include <stdio.h>
%}

/*regular definitions here*/
single \/\/
open \/\*
close \*\/

%%


 /* Rules */

					
{open}([^\*\/]|(\*[^\/])|(\/))*{close} {
						printf("%s\n",yytext);
					}
					

{single}.*[\n]	{	
						printf("%s\n",yytext);
					}					
\n ;
. ;
 

%%

/* Auxiliary functions */


int yywrap()
{
   
		return 1;
}


int main()
{  
    yylex();    
    return 1;
}





