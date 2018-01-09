
%{		
	#include <stdlib.h>
	#include <stdio.h>
	int charCount = 0, wordCount = 0, lineCount=0;
%}

/*no regular definitions here*/

%%


 /* Rules 
 	\s doesn't work?
 	*/

[^\n|^' '|^\t]+[' '|\t]	{
						printf(" word: %s", yytext);
						charCount+=yyleng;
						wordCount++;
					}
[^\n|^' '|^\t]+[\n] {
					printf(" word: %s", yytext);
					charCount+=yyleng;
					wordCount++;
					lineCount++;
				}

\n {	
		charCount++;
		lineCount++;
	}
	
. {charCount++;}
 

%%

/* Auxiliary functions */


int yywrap()
{
   
		return 1;
}


int main()
{  
    yylex();
    printf("\n Char count: %d\n Word Count: %d\n Line Count:%d\n",charCount, wordCount, lineCount);
    
    return 1;
}





