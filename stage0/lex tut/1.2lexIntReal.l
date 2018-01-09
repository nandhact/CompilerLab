
%{		
	#include <stdlib.h>
	#include <stdio.h>
	int intcount = 0, realcount = 0;
%}


int [0-9]+       

%%


 /* Rules  */
 
(-|\+)?{int} {printf("int ");intcount++;}
(-|\+)?{int}*\.{int} {printf("real ");realcount++;}
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
    printf(" Integer count: %d\n Real Count: %d\n",intcount, realcount);
    
    return 1;
}





