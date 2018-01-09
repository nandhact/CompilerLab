
%{		
	#include <stdlib.h>
	#include <stdio.h>
%}

/*no regular definitions here*/
letter [a-zA-Z]

%%


 /* Rules */

A{letter}{letter}	{ int i=0;
			printf("%s to ",yytext);
			for(i=0;i<3; i++){
				
				if(yytext[i]>='a' && yytext[i]<='z'){
					printf("%c", yytext[i]-'a'+'A');
				} else {
					printf("%c", yytext[i]);
					}
				}
			printf("\n");
			}

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





