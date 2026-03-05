#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define NUM_NON_TERMS 1 // number of non terminals given in the question
#define MAX_NON_TERMS 256 // number of non terminals we can have in the final set of production rules

char non_terms[NUM_NON_TERMS] = {'S'}; // all the non terminals in the question
size_t all_num_prod_rules[NUM_NON_TERMS] = {2}; // number of production rules for each non terminal
char* all_prod_rules[NUM_NON_TERMS][256] = { // production rules for each non terminal
    {
	"abS", "aSb"
    }
};
char unused_char = 33; // character pointer that points to the next non-terminal character that we can use

size_t curr_non_terms = 0; // number of non terminals in the final set of production rules
char new_non_terms[MAX_NON_TERMS] = {0}; // all the non terminals in the final set of production rules
size_t apr_idxs[MAX_NON_TERMS] = {0}; // number of production rules for each non terminal in the final set of production rules
char* all_new_prod_rules[MAX_NON_TERMS][256] = {0}; // production rules for each non terminal in the final set of production rules

// function that appends a production rule to our collection
void add_prod_rule(char non_term, char* prod_rule) {
    // find if given non term already exists
    int found_i = -1;
    for (size_t i = 0; i < curr_non_terms; ++i) {
	    if (new_non_terms[i] == non_term) {
	        found_i = i;
	        break;
	    }
    }

    if (found_i != -1) { // if non term already exists
	    char** new_prod_rules = all_new_prod_rules[found_i];
	    size_t pr_idx = apr_idxs[found_i]++;
	    new_prod_rules[pr_idx] = prod_rule;
    }

    else {
	    new_non_terms[curr_non_terms++] = non_term;
	    found_i = curr_non_terms - 1;
	    size_t pr_idx = apr_idxs[found_i]++;
	    all_new_prod_rules[found_i][pr_idx] = prod_rule;
    }
}

// function that performs left factoring given a production rule
void left_factor_unit(char non_term, char* prod_rule) {
    char fc = prod_rule[0];
    char inter = unused_char++;
    char* fc_and_inter = (char*) calloc(3, sizeof(char));
    fc_and_inter[0] = fc;
    fc_and_inter[1] = inter;
    fc_and_inter[2] = '\0';
    add_prod_rule(non_term, fc_and_inter);

    char *rest = (char*) calloc(256, sizeof(char));
    strcpy(rest, prod_rule + 1);
    add_prod_rule(inter, rest);
}

int main() {
    for (size_t i = 0; i < NUM_NON_TERMS; ++i) {
	int num_prod_rules = all_num_prod_rules[i];       
	char** prod_rules = all_prod_rules[i];
	char non_term = non_terms[i];
	bool visited[256] = {0};
	
	while (true) {
	    bool detected = false;

	    char* new_prod_rules[256] = {0};
	    size_t npr_idx = 0;
	    for (size_t j = 0; j < num_prod_rules; ++j) {
		if(visited[j]) {
		    continue;
		}
		visited[j] = true;
	
		char* ref_prod_rule = prod_rules[j];
		char fc = ref_prod_rule[0];
		left_factor_unit(non_term, ref_prod_rule);
		
		for (size_t k = j + 1; k < num_prod_rules; ++k) {
		    if (visited[k]) {
			continue;
		    }
		    char* compar_prod_rule = prod_rules[k];
		    if (compar_prod_rule[0] == fc) {
			visited[k] = true;
			detected = true;
			
			left_factor_unit(non_term, compar_prod_rule);
		    }
		}
	    }

	    if (!detected) {
		break;
	    }
	}
    }

    for (size_t i = 0; i < curr_non_terms; ++i) {
	char non_term = new_non_terms[i];
	printf("%c -> ", non_term);

	char** new_prod_rules = all_new_prod_rules[i];
	size_t num_new_prod_rules = apr_idxs[i];
	for (size_t j = 0; j < num_new_prod_rules; ++j) {
	    printf("%s", new_prod_rules[j]);
	    free(new_prod_rules[j]);
	    if (j != num_new_prod_rules - 1) {
		printf(" | ");
	    }
	}

	printf("\n");
    }
}
