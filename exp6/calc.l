%{
    #include "calc.tab.h"
    #include <stdio.h>
%}

%%
SIN { return SIN; }
COS { return COS; }
ASIN { return ASIN; }
ACOS { return ACOS; }
[0-9]+ { yylval = atof(yytext); return NUMBER; }
[+\-*/()] { return yytext[0]; }
[ \t] { /* Ignore whitespace */ }
\n { return '\n'; }
. { printf("Invalid character: %s\n", yytext); }
%%

int yywrap() {
    return 1;
}
