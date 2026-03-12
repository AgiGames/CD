%{
    int vowel_count = 0;
%}

%%
end {
    return 0;
}

a|e|i|o|u {
    printf("%s is a vowel.\n", yytext);
    ++vowel_count;
}

[ \t\n] {}

. {
    printf("%s not a vowel.\n", yytext);
}
%%

int yywrap() { return 1; }
int main() {
    yylex();
    printf("Number of vowels: %d\n", vowel_count);
    return 0;
}