%{
      #include <stdio.h>
      #include<string.h>
      #define YYSTYPE char*
      yydebug =1;
%}

%token CHAR NEWLINE ADD SUB MUL DIV
%left ADD SUB
%left MUL DIV
%left '('

%%
start : expr NEWLINE  { printf("%s ", $1);
                        printf("\nComplete!\n");
                        exit(1);
                      }
  ;

expr:  expr ADD expr {						
						$$ = malloc(200);
						strcat($$,"+ "); 
						strcat($$,$1); strcat($$," "); 
						strcat($$,$3);
					}
	| expr SUB expr {						
						$$ = malloc(200);
						strcat($$,"- "); 
						strcat($$,$1); strcat($$," "); 
						strcat($$,$3);
					}
	| expr MUL expr {						
						$$ = malloc(200);
						strcat($$,"* ");; 
						strcat($$,$1); strcat($$," "); 
						strcat($$,$3);
					}
	| expr DIV expr {						
						$$ = malloc(200);
						strcat($$,"/ "); 
						strcat($$,$1); strcat($$," "); 
						strcat($$,$3);
					}
  | '(' expr ')'	{	$$ = $2;}
  | CHAR             {	$$ = $1;
  						}
  ;
	
%%

void yyerror(char const *s) 
{ 
    printf("\nyyerror  %s\n",s); 
    return ;
} 
int main()
{
  yyparse();
  return 1;
}
