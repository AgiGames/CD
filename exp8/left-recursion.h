#include <stddef.h>

#include "helper.h"

void left_recur_init(size_t num_non_terms_p, char non_terms_p[MAX_CAPACITY], size_t num_prod_rules_p[MAX_CAPACITY], char* prod_rules_p[MAX_CAPACITY][MAX_CAPACITY]);
void lr_print_new_prod_rules();
void lr_print_prod_rules();
void remove_left_recursion();
size_t lr_get_new_num_non_terms();
char* lr_get_new_non_terms();
size_t* lr_get_new_num_prod_rules();
char* (*lr_get_new_prod_rules())[MAX_CAPACITY];
