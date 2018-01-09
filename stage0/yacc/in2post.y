%{
      #include <stdio.h>
      
      #define YYSTYPE char*
%}

%token CHAR NEWLINE
%left '+'
%left '*'
%%

start : expr NEWLINE  { 
                        printf("\nComplete\n");
                        exit(1);
                      }
  ;

expr:  expr '+' expr        {printf("+ ");}
  | expr '*' expr     {printf("* ");}
  | '(' expr ')'
  | CHAR             {printf("%s ",$1);}
  ;

%%

void yyerror(char const *s) 
{ 
    printf("yyerror  %s\n",s); 
    return ;
} 
int main()
{
  yyparse();
  return 1;
}
