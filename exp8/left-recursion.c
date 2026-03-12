#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "left-recursion.h"
#include "helper.h"

static size_t num_non_terms;
static char* non_terms;
static char inters[MAX_CAPACITY] = {0};
static size_t* num_prod_rules;
static char* prod_rules[MAX_CAPACITY][MAX_CAPACITY];

static size_t new_num_non_terms = 0;
static char new_non_terms[MAX_CAPACITY];
static size_t nnt_idx = 0;
static size_t new_num_prod_rules[MAX_CAPACITY];
static size_t nnpr_idx = 0; 
static char* new_prod_rules[MAX_CAPACITY][MAX_CAPACITY];

void left_recur_init(size_t num_non_terms_p, char non_terms_p[MAX_CAPACITY], size_t num_prod_rules_p[MAX_CAPACITY], char* prod_rules_p[MAX_CAPACITY][MAX_CAPACITY]) {
    num_non_terms = num_non_terms_p;
    non_terms = non_terms_p;
    num_prod_rules = num_prod_rules_p;
    
    for (size_t i = 0; i < num_non_terms_p; ++i) {
        size_t set_len = num_prod_rules_p[i];
        for (size_t j = 0; j < set_len; ++j) {
            prod_rules[i][j] = prod_rules_p[i][j];
        }
    }
}

void lr_add_new_prod_rule(char parent, char* prod_rule) {
    int prod_rule_set_idx = linear_search(parent, new_non_terms, new_num_non_terms);
    if (prod_rule_set_idx == -1) {
        prod_rule_set_idx = new_num_non_terms;
        size_t* set_len = &new_num_non_terms;
        AC(parent, new_non_terms, set_len);
    }

    char** prod_rule_set = new_prod_rules[prod_rule_set_idx];
    size_t* set_len = &new_num_prod_rules[prod_rule_set_idx];
    AC(prod_rule, prod_rule_set, set_len);
}

void remove_left_recursion() {
    for (size_t i = 0; i < num_non_terms; ++i) {
        char parent = non_terms[i];
        char** prod_rule_set = prod_rules[i];
        size_t set_len = num_prod_rules[i];

        bool has_left_recur = false;
        for (size_t j = 0; j < set_len; ++j) {
            char* jth_prod_rule = prod_rule_set[j];
            if (jth_prod_rule[0] == parent) {
                has_left_recur = true;

                // S -> Aa | eps
                // S -> A'
                // A' -> aA' | eps
                
                char* A_dash = calloc(2, sizeof(char));
                if (inters[i] == 0) {
                    inters[i] = get_next_nonterm();
                    A_dash[0] = inters[i];
                    A_dash[1] = '\0';
                    lr_add_new_prod_rule(parent, A_dash);
                }
                
                else {
                    A_dash[0] = inters[i];
                    A_dash[1] = '\0';
                }

                char* a_A_dash = calloc(strlen(jth_prod_rule + 1) + 2, sizeof(char));
                size_t k = 0;
                for (; k < strlen(jth_prod_rule + 1); ++k) {
                    a_A_dash[k] = (jth_prod_rule + 1)[k];
                }
                a_A_dash[k++] = inters[i];
                a_A_dash[k++] = '\0';

                lr_add_new_prod_rule(inters[i], a_A_dash);
            }
        }

         for (size_t j = 0; j < set_len; ++j) {
            char* jth_prod_rule = prod_rule_set[j];
            if (jth_prod_rule[0] != parent && has_left_recur) {
                lr_add_new_prod_rule(inters[i], jth_prod_rule);
            }
            else if(!has_left_recur) {
                lr_add_new_prod_rule(parent, jth_prod_rule);
            }
         }
    }
}

void lr_print_prod_rules() {
     for (size_t i = 0; i < num_non_terms; ++i) {
        char parent = non_terms[i];
        char** prod_rule_set = prod_rules[i];
        size_t set_len = num_prod_rules[i];
        printf("%c -> ", parent);

        for (size_t j = 0; j < set_len; ++j) {
            printf("%s", prod_rule_set[j]);
            if (j != set_len - 1) printf(" | ");
        }
        printf("\n");
    }   
}

void lr_print_new_prod_rules() {
    for (size_t i = 0; i < new_num_non_terms; ++i) {
        char parent = new_non_terms[i];
        char** prod_rule_set = new_prod_rules[i];
        size_t set_len = new_num_prod_rules[i];
        printf("%c -> ", parent);

        for (size_t j = 0; j < set_len; ++j) {
            printf("%s", prod_rule_set[j]);
            if (j != set_len - 1) printf(" | ");
        }
        printf("\n");
    }
}

size_t lr_get_new_num_non_terms() {
    return new_num_non_terms;
}

char* lr_get_new_non_terms() {
    return new_non_terms;
}

size_t* lr_get_new_num_prod_rules() {
    return new_num_prod_rules;
}

char* (*lr_get_new_prod_rules())[MAX_CAPACITY] {
    return new_prod_rules;
}
