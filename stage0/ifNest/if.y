%{
      #include <stdio.h>
      #define YYSTYPE char*
     int level = 0;
%}


%token IF ENDIF
%token 	CHAR NEWLINE

%%

start : expr NEWLINE {
                        printf("\nComplete\n");
                        exit(1);
                      }
                      
  ;

expr:  IF expr ENDIF {
						printf("level: %d ", level++);
					  }
		| CHAR {	printf("num");	} 
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
