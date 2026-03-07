#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "first-follow.h"

static size_t num_non_terms;
static char* non_terms;
static size_t* num_prod_rules;
static char* prod_rules[MAX_CAPACITY][MAX_CAPACITY];
static char distinct_chars[MAX_CAPACITY] = {'$'};
static size_t dc_idx = 1;
char* conflict = "<!>";

static char first[MAX_CAPACITY][MAX_CAPACITY] = {0};
static size_t first_app_indexes[MAX_CAPACITY] = {0};

static char follow[MAX_CAPACITY][MAX_CAPACITY] = {0};
static size_t follow_app_indexes[MAX_CAPACITY] = {0};

int char_cmp(const void* a, const void* b) {
    if (*(char*) a < *(char*)b) {
        return -1;
    }
    if (*(char*) a > *(char*)b) {
        return 1;
    }
    return 0;
}

void first_follow_init(size_t num_non_terms_p, char non_terms_p[MAX_CAPACITY], size_t num_prod_rules_p[MAX_CAPACITY], char* prod_rules_p[MAX_CAPACITY][MAX_CAPACITY]) {
    num_non_terms = num_non_terms_p;
    non_terms = non_terms_p;
    num_prod_rules = num_prod_rules_p;
    
    for (size_t i = 0; i < num_non_terms_p; ++i) {
        size_t set_len = num_prod_rules_p[i];
        for (size_t j = 0; j < set_len; ++j) {
            prod_rules[i][j] = prod_rules_p[i][j];
        }
    }

    for (size_t i = 0; i < num_non_terms; ++i) {
        char parent = non_terms[i];
        if (linear_search(parent, distinct_chars, dc_idx) == -1) {
            AC(parent, distinct_chars, &dc_idx);
        }

        char** prod_rule_set = prod_rules[i];
        size_t set_len = num_prod_rules[i];

        for (size_t j = 0; j < set_len; ++j) {
            char* jth_prod_rule = prod_rule_set[j];
            size_t prod_rule_len = strlen(jth_prod_rule);

            for (size_t k = 0; k < prod_rule_len; ++k) {
                if (linear_search(jth_prod_rule[k], distinct_chars, dc_idx) == -1) {
                    AC(jth_prod_rule[k], distinct_chars, &dc_idx);
                }
            }
        }
    }

    qsort(distinct_chars, dc_idx, sizeof(char), char_cmp);
}

bool add_to_first_set(char parent, char child) {
    int first_index = linear_search(parent, distinct_chars, dc_idx);
    if (first_index == -1) {
        return false;
    }

    // we first check if character exists already
    size_t set_len = first_app_indexes[first_index];
    if (linear_search(child, first[first_index], set_len) != -1) return false;

    char* first_set = first[first_index];
    first_set[set_len] = child;
    first_app_indexes[first_index] += 1;

    return true;
}

bool add_to_follow_set(char parent, char child) {
    int follow_index = linear_search(parent, distinct_chars, dc_idx);
    if (follow_index == -1) {
        return false;
    }

    // we first check if character exists already
    size_t set_len = follow_app_indexes[follow_index];
    if (linear_search(child, follow[follow_index], set_len) != -1) return false;

    char* follow_set = follow[follow_index];
    follow_set[set_len] = child;
    follow_app_indexes[follow_index] += 1;

    return true;
}

bool has_eps(char parent) {
    int first_index = linear_search(parent, distinct_chars, dc_idx);
    return (linear_search(EPS_CHAR, first[first_index], first_app_indexes[first_index]) != -1);
}

bool union_first_sets(char parent, char child) {
    int child_first_index =  linear_search(child, distinct_chars, dc_idx);

    if (child_first_index == -1) {
        return false;
    }
    
    bool changed = false;
    char* child_first = first[child_first_index];
    size_t child_set_len = first_app_indexes[child_first_index];
    for (size_t i = 0; i < child_set_len; ++i) {
        changed = changed || add_to_first_set(parent, child_first[i]);
    }

    return changed;
}


bool union_follow_sets(char parent, char child) {
    int child_follow_index = linear_search(child, distinct_chars, dc_idx);

    if (child_follow_index == -1) {
        return false;
    }

    bool changed = false;
    char* child_follow = follow[child_follow_index];
    size_t child_set_len = follow_app_indexes[child_follow_index];
    for (size_t i = 0; i < child_set_len; ++i) {
        changed = changed || add_to_follow_set(parent, child_follow[i]);
    }

    return changed;
}

bool union_follow_first_sets(char parent, char child) {
    int child_first_index = linear_search(child, distinct_chars, dc_idx);

    if (child_first_index == -1) {
        return false;
    }

    bool changed = false;
    char* child_first = first[child_first_index];
    size_t child_set_len = first_app_indexes[child_first_index];
    for (size_t i = 0; i < child_set_len; ++i) {
        if (child_first[i] != EPS_CHAR) {
            changed = changed || add_to_follow_set(parent, child_first[i]);
        }
    }

    return changed;
}

void create_first_sets() {
    bool changed = false;
    
    do {
        changed = false;
        for (size_t i = 0; i < num_non_terms; ++i) {
            size_t ith_num_prod_rules = num_prod_rules[i];
            for (size_t j = 0; j < ith_num_prod_rules; ++j) {
                char* jth_prod_rule = prod_rules[i][j];
                // we define a terminal as any character that is not upper case
                
                size_t jth_prod_rule_len = strlen(jth_prod_rule);
                for (size_t k = 0; k < jth_prod_rule_len; ++k) {
                    char current_char = jth_prod_rule[k];
                    if (is_term(current_char)) {
                        changed = changed || add_to_first_set(current_char, current_char);
                    }
                }
            }
        }

        for (size_t i = 0; i < num_non_terms; ++i) {
            char parent = non_terms[i];
            size_t ith_num_prod_rules = num_prod_rules[i];
            for (size_t j = 0; j < ith_num_prod_rules; ++j) {
                char* jth_prod_rule = prod_rules[i][j];
                size_t jth_prod_rule_len = strlen(jth_prod_rule);
                for (size_t k = 0; k < jth_prod_rule_len; ++k) {
                    char first_char = jth_prod_rule[k];
                    if (first_char == '\0') continue;
                    changed = changed || union_first_sets(parent, first_char);
                    if (has_eps(first_char)) {
                        if (k == jth_prod_rule_len - 1) {
                            changed = changed || add_to_first_set(parent, '@');
                        }
                    }
                    else {
                        break;
                    }
                }
            }
        }
    } while(changed);
}

void create_follow_sets() {
    bool changed = false;
    
    do {
        changed = false;
        for (size_t i = 0; i < num_non_terms; ++i) {
            char parent = non_terms[i];
            size_t ith_num_prod_rules = num_prod_rules[i];
            for (size_t j = 0; j < ith_num_prod_rules; ++j) {
                char* jth_prod_rule = prod_rules[i][j];
                // we define a terminal as any character that is lower case
                
                size_t jth_prod_rule_len = strlen(jth_prod_rule);
                for (size_t k = 0; k < jth_prod_rule_len; ++k) {
                    char last_char = jth_prod_rule[k];
                    if (last_char == '\0') continue;
                    
                    if (is_non_term(last_char)) {
                        size_t l = k + 1;
                        while (l < jth_prod_rule_len) {
                            changed = changed || union_follow_first_sets(last_char, jth_prod_rule[l]);
                            if (has_eps(jth_prod_rule[l])) {
                                l += 1;
                            }
                            else {
                                break;
                            }
                        }
                        if (l == jth_prod_rule_len) {
                            changed = changed || union_follow_sets(last_char, parent);
                        }
                    }
               }
            }
        }
    } while(changed);
}

void print_first_sets() {
    printf("First Sets:\n");
    for (size_t i = 0; i < dc_idx; ++i) {
        size_t set_len = first_app_indexes[i];
        char* first_set = first[i];
        char parent = distinct_chars[i];
        printf("%c: ", parent);
        for (size_t j = 0; j < set_len; ++j) {
            char child = first_set[j];
            printf("%c", child);
            if (j != set_len - 1) {printf(" ");}
        }
        printf("\n");
    }
}

void print_follow_sets() {
    printf("Follow Sets:\n");
    for (size_t i = 0; i < dc_idx; ++i) {
        size_t set_len = follow_app_indexes[i];
        char* follow_set = follow[i];
        char parent = distinct_chars[i];
        if (is_non_term(parent)) {
            printf("%c: ", parent);
            for (size_t j = 0; j < set_len; ++j) {
                char child = follow_set[j];
                printf("%c", child);
                if (j != set_len - 1) {printf(" ");}
            }
            printf("\n");
        }
    }
}

void find_first_follow() {
    add_to_follow_set('S', '$');
    create_first_sets();
    create_follow_sets();
    print_first_sets();
    print_follow_sets();
}

void ff_print_distinct_chars() {
    printf("Distinct Characters:\n");
    for (size_t i = 0; i < dc_idx; ++i) {
        printf("%c", distinct_chars[i]);
        if (i != dc_idx - 1) printf(" ");
    }
    printf("\n");
}

void ff_print_prod_rules() {
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

void ff_print_parsing_table() {
    char* table[MAX_CAPACITY][MAX_CAPACITY] = {0};

    char cols[MAX_CAPACITY];
    size_t num_cols = 0;
    
    char rows[MAX_CAPACITY];
    size_t num_rows = 0;

    size_t table_indexes[128];

    for (size_t i = 0; i < dc_idx; ++i) {
        if (distinct_chars[i] == EPS_CHAR) continue;
        char curr_char = distinct_chars[i];
        if (is_term(curr_char)) {
            table_indexes[curr_char] = num_cols;
            AC(curr_char, cols, &num_cols);
        }
        else if(is_non_term(curr_char)) {
            table_indexes[curr_char] = num_rows;
            AC(curr_char, rows, &num_rows);
        }
    }

    for (size_t i = 0; i < num_non_terms; ++i) {
        char parent = non_terms[i];
        char** prod_rule_set = prod_rules[i];
        size_t set_len = num_prod_rules[i];
        size_t prod_rule_follow_idx = linear_search(parent, distinct_chars, dc_idx);
        char* prod_rule_follow_set = follow[prod_rule_follow_idx];
        size_t prod_rule_follow_set_len = follow_app_indexes[prod_rule_follow_idx];

        for (size_t j = 0; j < set_len; ++j) {
            char* jth_prod_rule = prod_rule_set[j];
            size_t jth_prod_rule_len = strlen(jth_prod_rule);
            char jth_first_char = jth_prod_rule[0];
            char jth_prod_rule_set[MAX_CAPACITY] = {0};
            size_t jth_prod_rule_set_len = 0;
            bool requires_follow = false;

            for (size_t k = 0; k < jth_prod_rule_len; ++k) {
                char kth_char = jth_prod_rule[k];
                if (kth_char == '\0') continue;
                size_t kth_first_follow_idx = linear_search(kth_char, distinct_chars, dc_idx);
                char* kth_char_set = first[kth_first_follow_idx];
                union_sets(jth_prod_rule_set, kth_char_set, &jth_prod_rule_set_len, &first_app_indexes[kth_first_follow_idx]);
                if (has_eps(kth_char)) {
                    if (k == jth_prod_rule_len - 1) {
                        if (linear_search(EPS_CHAR, jth_prod_rule_set, jth_prod_rule_set_len) == -1) {
                            AC(EPS_CHAR, jth_prod_rule_set, &jth_prod_rule_set_len);
                        }
                        requires_follow = true;
                    }
                }
                else {
                    break;
                }
            }

            for (size_t k = 0; k < jth_prod_rule_set_len; ++k) {
                char first_set_char = jth_prod_rule_set[k];
                if (first_set_char == EPS_CHAR) continue;

                char* table_element = table[table_indexes[parent]][table_indexes[first_set_char]];
                if (table_element != NULL && strcmp(table_element, jth_prod_rule) != 0) {
                    table[table_indexes[parent]][table_indexes[first_set_char]] = conflict;
                }
                else {
                    table[table_indexes[parent]][table_indexes[first_set_char]] = jth_prod_rule;
                }
            }

            if (requires_follow) {
                for (size_t k = 0; k < prod_rule_follow_set_len; ++k) {
                    char follow_set_char = prod_rule_follow_set[k];

                    char* table_element = table[table_indexes[parent]][table_indexes[follow_set_char]];
                    if (table_element != NULL && strcmp(table_element, jth_prod_rule) != 0) {
                        table[table_indexes[parent]][table_indexes[follow_set_char]] = conflict;
                    }
                    else {
                        table[table_indexes[parent]][table_indexes[follow_set_char]] = jth_prod_rule;
                    }
                }
            }
        }
    }

    printf("#\t");
    for (size_t i = 0; i < dc_idx; ++i) {
        if (distinct_chars[i] != EPS_CHAR && is_term(distinct_chars[i])) {
            printf("%c\t", distinct_chars[i]);
        }
    }
    printf("\n");

    for (size_t i = 0; i < dc_idx; ++i) {
        if (is_term(distinct_chars[i])) continue;
        char parent = distinct_chars[i];
        printf("%c\t", parent);
        for (size_t j = 0; j < num_cols; ++j) {
            printf("%s\t", table[table_indexes[parent]][j]);
        }
        printf("\n");
    }
}
