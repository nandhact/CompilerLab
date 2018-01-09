/*Declarations section start here*/

/* Auxiliary declarations start here*/

/*like 'inline' c code*/

%{		
	#include <stdlib.h>
	#include <stdio.h>
	char *file1;
	int count = 0;
%}

/*Auxiliary declarations end & Regular definitions start here
first match*/



number [0-9]+           
op [-|+|*|/|^|=]


%%


 /* Rules */
 
{number} {printf("number");}
{op} {printf("<OP,%c>", *yytext);}


%%

/* Auxiliary functions */


int yywrap()
{
    
    
  	/*char *file2="stdin";
  	newfile_pointer = stdin;
  	*/
  	
  	/*char *file2="test2.l";
  	newfile_pointer = fopen("test2.l","r");
  	*/
  	
  	if(count == 0)
  	{
  		yyin = stdin;
  		count++;
    	return 0;
  	}
  	else
		return 1;
}


int main()
{
	/*file, then console
	
	file1="test.txt";
	yyin = fopen("test.txt","r");
	yylex();
	*/


	/*
	console, then file
	
	file1="stdin";
	yyin = stdin;
    yylex();
    */
    
    
    yyin = stdin;
    yylex();
    printf("after yylex\n");
    return 1;
}





