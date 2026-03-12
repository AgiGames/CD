#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "left-recursion.h"
#include "left-factoring.h"
#include "first-follow.h"
#include "helper.h"

size_t num_non_terms_p = 3;
char non_terms_p[MAX_CAPACITY] = {'S', 'A', 'B'};
size_t num_prod_rules_p[MAX_CAPACITY] = {2, 2, 2};
char* prod_rules_p[MAX_CAPACITY][MAX_CAPACITY] = {
    {"aAd", "aB"},
    {"a", "ab"},
    {"ccd", "ddc"}
};

int main() {
    for (size_t i = 0; i < num_non_terms_p; ++i) {
        char parent = non_terms_p[i];
        set_visited(parent);
        char** prod_rule_set = prod_rules_p[i];
        size_t set_len = num_prod_rules_p[i];

        for (size_t j = 0; j < set_len; ++j) {
            char* jth_prod_rule = prod_rule_set[j];
            size_t prod_rule_len = strlen(jth_prod_rule);

            for (size_t k = 0; k < prod_rule_len; ++k) {
                set_visited(jth_prod_rule[k]);
            }
        }
    }

    left_recur_init(num_non_terms_p, non_terms_p, num_prod_rules_p, prod_rules_p);
    lr_print_prod_rules();

    printf("\n\n");

    remove_left_recursion();
    lr_print_new_prod_rules();
    size_t lr_num_non_terms = lr_get_new_num_non_terms();
    char* lr_non_terms = lr_get_new_non_terms();
    size_t* lr_num_prod_rules = lr_get_new_num_prod_rules();
    char* (*lr_prod_rules)[MAX_CAPACITY] = lr_get_new_prod_rules();
    
    printf("\n\n");

    left_factor_init(
            lr_num_non_terms,
            lr_non_terms,
            lr_num_prod_rules,
            lr_prod_rules);
    do_left_factoring();
    lf_print_new_prod_rules();
    size_t lf_num_non_terms = lf_get_new_num_non_terms();
    char* lf_non_terms = lf_get_new_non_terms();
    size_t* lf_num_prod_rules = lf_get_new_num_prod_rules();
    char* (*lf_prod_rules)[MAX_CAPACITY] = lf_get_new_prod_rules();

    printf("\n\n");

    first_follow_init(
            lf_num_non_terms,
            lf_non_terms,
            lf_num_prod_rules,
            lf_prod_rules);
    find_first_follow();

    printf("\n\n");
    ff_print_parsing_table();

    return 0;
}
