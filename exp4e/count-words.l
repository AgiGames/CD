%{
	int no_of_spaces = 0;
%}

%%
" "	++no_of_spaces;
\n	return 0;
.	;
%%

int yywrap(){}
int main() {
	yylex();
	printf("Number of words: %d\n", no_of_spaces + 1);
	return 0;
}
