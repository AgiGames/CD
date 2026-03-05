#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define NUM_NON_TERMS 1
#define MAX_PROD_RULES 256
#define NUM_DISTINCT 3

char distinct_chars[NUM_DISTINCT] = {'S', 'a', 'b'};
char non_terms[NUM_NON_TERMS] = {'S'};
size_t num_prod_rules[NUM_NON_TERMS] = {2};
char* prod_rules[NUM_NON_TERMS][MAX_PROD_RULES] = {
    {"abS", "aSb"} // 2 production rules
};

char first[NUM_DISTINCT][256] = {0};
size_t first_app_indexes[NUM_DISTINCT] = {0};

char follow[NUM_DISTINCT][256] = {0};
size_t follow_app_indexes[NUM_DISTINCT] = {0};

bool is_lower_case(char chr) {
    return (97 <= chr && chr <= 122) || (chr == '@');
}

int linear_search(char needle, char* haystack, size_t haystack_len) {
    int index = -1;
    for (size_t i = 0; i < haystack_len; ++i) {
        if (haystack[i] == needle) {
            index = i;
            break;
        }
    }

    return index;
}

bool add_to_follow_set(char parent, char child) {
    int follow_index = linear_search(parent, distinct_chars, NUM_DISTINCT);
    if (follow_index == -1) {
        return false;
    }

    // we first check if character exists already
    size_t set_len = follow_app_indexes[follow_index];
    if (linear_search(child, follow[follow_index], set_len) != -1) {return false;}

    char* follow_set = follow[follow_index];
    follow_set[set_len] = child;
    follow_app_indexes[follow_index] += 1;

    return true;
}

bool add_to_first_set(char parent, char child) {
    int first_index = linear_search(parent, distinct_chars, NUM_DISTINCT);
    if (first_index == -1) {
        return false;
    }

    // we first check if character exists already
    size_t set_len = first_app_indexes[first_index];
    if (linear_search(child, first[first_index], set_len) != -1) {return false;}

    char* first_set = first[first_index];
    first_set[set_len] = child;
    first_app_indexes[first_index] += 1;

    return true;
}

bool has_eps(char parent) {
    int first_index = linear_search(parent, distinct_chars, NUM_DISTINCT);
    return (linear_search('@', first[first_index], first_app_indexes[first_index]) != -1);
}

bool union_first_sets(char parent, char child) {
    int child_first_index =  linear_search(child, distinct_chars, NUM_DISTINCT);

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

bool union_follow_follow_sets(char parent, char child) {
    int child_follow_index = linear_search(child, distinct_chars, NUM_DISTINCT);

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
    int child_first_index = linear_search(child, distinct_chars, NUM_DISTINCT);

    if (child_first_index == -1) {
        return false;
    }

    bool changed = false;
    char* child_first = first[child_first_index];
    size_t child_set_len = first_app_indexes[child_first_index];
    for (size_t i = 0; i < child_set_len; ++i) {
        if (child_first[i] != '@') {
            changed = changed || add_to_follow_set(parent, child_first[i]);
        }
    }

    return changed;
}

void create_first_sets() {
    bool changed = false;
    
    do {
        changed = false;
        for (size_t i = 0; i < NUM_NON_TERMS; ++i) {
            size_t ith_num_prod_rules = num_prod_rules[i];
            for (size_t j = 0; j < ith_num_prod_rules; ++j) {
                char* jth_prod_rule = prod_rules[i][j];
                // we define a terminal as any character that is lower case
                
                size_t jth_prod_rule_len = strlen(jth_prod_rule);
                for (size_t k = 0; k < jth_prod_rule_len; ++k) {
                    char current_char = jth_prod_rule[k];
                    if (is_lower_case(current_char)) {
                        changed = changed || add_to_first_set(current_char, current_char);
                    }
                }
            }
        }

        for (size_t i = 0; i < NUM_NON_TERMS; ++i) {
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
        for (size_t i = 0; i < NUM_NON_TERMS; ++i) {
            char parent = non_terms[i];
            size_t ith_num_prod_rules = num_prod_rules[i];
            for (size_t j = 0; j < ith_num_prod_rules; ++j) {
                char* jth_prod_rule = prod_rules[i][j];
                // we define a terminal as any character that is lower case
                
                size_t jth_prod_rule_len = strlen(jth_prod_rule);
                for (size_t k = 0; k < jth_prod_rule_len; ++k) {
                    char last_char = jth_prod_rule[k];
                    if (last_char == '\0') continue;
                    
                    if (!is_lower_case(last_char)) {
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
                            changed = changed || union_follow_follow_sets(last_char, parent);
                        }
                    }
               }
            }
        }
    } while(changed);
}

void print_first_sets() {
    printf("First Sets:\n");
    for (size_t i = 0; i < NUM_DISTINCT; ++i) {
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
    for (size_t i = 0; i < NUM_DISTINCT; ++i) {
        size_t set_len = follow_app_indexes[i];
        char* follow_set = follow[i];
        char parent = distinct_chars[i];
        if (!is_lower_case(parent)) {
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

int main() {
    add_to_follow_set('S', '$');

    create_first_sets();
    create_follow_sets();
    print_first_sets();
    printf("\n");
    print_follow_sets();

    return 0;
}
