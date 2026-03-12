#include "helper.h"

void left_factor_init(size_t num_non_terms_p, char non_terms_p[MAX_CAPACITY], size_t num_prod_rules_p[MAX_CAPACITY], char* prod_rules_p[MAX_CAPACITY][MAX_CAPACITY]);
void lf_print_new_prod_rules();
void lf_print_prod_rules();
void do_left_factoring();
size_t lf_get_new_num_non_terms();
char* lf_get_new_non_terms();
size_t* lf_get_new_num_prod_rules();
char* (*lf_get_new_prod_rules())[MAX_CAPACITY];
