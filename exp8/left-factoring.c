#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "left-factoring.h"

static size_t num_non_terms;
static char* non_terms;
static size_t* num_prod_rules;
static char* prod_rules[MAX_CAPACITY][MAX_CAPACITY];

static size_t copy_num_non_terms;
static char copy_non_terms[MAX_CAPACITY];
static size_t copy_num_prod_rules[MAX_CAPACITY];
static char* copy_prod_rules[MAX_CAPACITY][MAX_CAPACITY];

static size_t new_num_non_terms = 0;
static char new_non_terms[MAX_CAPACITY];
static size_t nnt_idx = 0;
static size_t new_num_prod_rules[MAX_CAPACITY];
static size_t nnpr_idx = 0; 
static char* new_prod_rules[MAX_CAPACITY][MAX_CAPACITY];

void left_factor_init(size_t num_non_terms_p, char non_terms_p[MAX_CAPACITY], size_t num_prod_rules_p[MAX_CAPACITY], char* prod_rules_p[MAX_CAPACITY][MAX_CAPACITY]) {
    num_non_terms = num_non_terms_p;
    non_terms = non_terms_p;
    num_prod_rules = num_prod_rules_p;
    
    for (size_t i = 0; i < num_non_terms_p; ++i) {
        size_t set_len = num_prod_rules_p[i];
        for (size_t j = 0; j < set_len; ++j) {
            prod_rules[i][j] = prod_rules_p[i][j];
        }
    }

    copy_num_non_terms = num_non_terms;
    for (size_t i = 0; i < num_non_terms; ++i) {
        copy_non_terms[i] = non_terms[i];
    }

    
    for (size_t i = 0; i < num_non_terms; ++i) {
        copy_num_prod_rules[i] = num_prod_rules[i];
    }

    for (size_t i = 0; i < num_non_terms; ++i) {
        size_t set_len = num_prod_rules[i];
        for (size_t j = 0; j < set_len; ++j) {
            copy_prod_rules[i][j] = prod_rules[i][j];
        }
    }
}

void lf_add_new_prod_rule(char parent, char* prod_rule) {
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

int strcmp_void(const void* a, const void* b) {
    return strcmp(*(char**)a, *(char**)b);
}

size_t longest_common_prefix(char** strs, size_t num_strs, size_t custom_start) {
    char* min_str = strs[0] + custom_start;
    char* max_str = strs[0] + custom_start;

    for (size_t i = 1; i < num_strs; ++i) {
        if (strcmp(strs[i] + custom_start, min_str) < 0) {
            min_str = strs[i];
        }
        if (strcmp(strs[i] + custom_start, max_str) > 0) {
            max_str = strs[i];
        }
    }

    size_t i = 0;
    while (min_str[i] && max_str[i] && min_str[i] == max_str[i]) {
        ++i;
    }

    return i;
}

void do_left_factoring() {
    bool changed;
    do {
        changed = false;
        for (size_t i = 0; i < copy_num_non_terms; ++i) {
            char parent = copy_non_terms[i];
            char** prod_rule_set = copy_prod_rules[i];
            size_t set_len = copy_num_prod_rules[i];

            char first_chars[MAX_CAPACITY] = {0};
            size_t num_first_chars = 0;

            for (size_t j = 0; j < set_len; ++j) {
                if (linear_search(prod_rule_set[j][0], first_chars, num_first_chars) == -1) {
                    AC(prod_rule_set[j][0], first_chars, &num_first_chars);
                }
            }

            for (size_t j = 0; j < num_first_chars; ++j) {
                char first_char = first_chars[j];
                size_t common_idxs[MAX_CAPACITY] = {0};
                size_t num_common_idxs = 0;

                for (size_t k = 0; k < set_len; ++k) {
                    if (prod_rule_set[k][0] == first_char) {
                        AC(k, common_idxs, &num_common_idxs);
                    }
                }

                if (num_common_idxs == 1) {
                    lf_add_new_prod_rule(parent, prod_rule_set[common_idxs[0]]);
                    continue;
                }
                
                changed = true;

                char* common_strs[MAX_CAPACITY];
                size_t num_common_strs = 0;
                for (size_t k = 0; k < num_common_idxs; ++k) {
                    AC(prod_rule_set[common_idxs[k]], common_strs, &num_common_strs);
                }

                size_t common_len = longest_common_prefix(common_strs, num_common_strs, 0);
                char inter = get_next_nonterm();
                char* inter_str = calloc(common_len + 1 + 1, sizeof(char));
                for (size_t k = 0; k < common_len; ++k) {
                    inter_str[k] = common_strs[0][k];
                }
                inter_str[common_len] = inter;

                lf_add_new_prod_rule(parent, inter_str);
                for (size_t k = 0; k < num_common_strs; ++k) {
                    if (common_strs[k][common_len] != '\0') {
                        lf_add_new_prod_rule(inter, common_strs[k] + common_len);
                    }
                    else {
                        lf_add_new_prod_rule(inter, EPS_STR);
                    }
                }
            }
        }

        if (changed) {
            copy_num_non_terms = new_num_non_terms;
            for (size_t i = 0; i < new_num_non_terms; ++i) {
                copy_non_terms[i] = new_non_terms[i];
            }

            for (size_t i = 0; i < new_num_non_terms; ++i) {
                copy_num_prod_rules[i] = new_num_prod_rules[i];
            }

            for (size_t i = 0; i < new_num_non_terms; ++i) {
                size_t set_len = new_num_prod_rules[i];
                for (size_t j = 0; j < set_len; ++j) {
                    copy_prod_rules[i][j] = new_prod_rules[i][j];
                }
            }

            memset(new_num_prod_rules, 0, sizeof(new_num_prod_rules));
            new_num_non_terms = 0;
            nnt_idx = 0;
            nnpr_idx = 0;
        }
    } while(changed);
}

void lf_print_prod_rules() {
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

void lf_print_new_prod_rules() {
    for (size_t i = 0; i < copy_num_non_terms; ++i) {
        char parent = copy_non_terms[i];
        char** prod_rule_set = copy_prod_rules[i];
        size_t set_len = copy_num_prod_rules[i];
        printf("%c -> ", parent);

        for (size_t j = 0; j < set_len; ++j) {
            printf("%s", prod_rule_set[j]);
            if (j != set_len - 1) printf(" | ");
        }
        printf("\n");
    }
}

size_t lf_get_new_num_non_terms() {
    return copy_num_non_terms;
}

char* lf_get_new_non_terms() {
    return copy_non_terms;
}

size_t* lf_get_new_num_prod_rules() {
    return copy_num_prod_rules;
}

char* (*lf_get_new_prod_rules())[MAX_CAPACITY] {
    return copy_prod_rules;
}
