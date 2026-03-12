%define api.value.type {double}

%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <math.h>
    void yyerror(const char *s);
    int yylex();
%}
/* Operator precedence and associativity */
%left '+' '-'
%left '*' '/'
%right UMINUS /* Unary minus */
%token NUMBER
%token SIN
%token COS
%token ASIN
%token ACOS

%%
equation:
        expression '\n' { printf("Result: %lf\n", $1); return 0; }
;
expression:
    expression '+' expression { $$ = $1 + $3; }
    | expression '-' expression { $$ = $1 - $3; }
    | expression '*' expression { $$ = $1 * $3; }
    | expression '/' expression { $$ = $1 / $3; }
    | '-' expression %prec UMINUS { $$ = -$2; } /* Unary minus */
    | '(' expression ')' { $$ = $2; }
    | NUMBER { $$ = $1; }
    | SIN expression {$$ = sin($2); }
    | COS expression {$$ = cos($2); }
    | ASIN expression {$$ = asin($2); }
    | ACOS expression {$$ = acos($2); }
;
%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main() {
    printf("Enter an arithmetic expression:\n");
    yyparse();
    return 0;
}
