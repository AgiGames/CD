#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define NUM_NON_TERMS 1
char non_terms[NUM_NON_TERMS] = {'S'};
int all_num_prod_rules[NUM_NON_TERMS] = {3};
char* all_prod_rules[NUM_NON_TERMS][256] = {
    {
        "S+S", "S*S", "a"
    }
};

int main() {
    for (size_t i = 0; i < NUM_NON_TERMS; ++i) {
        char** ith_prod_rules = all_prod_rules[i];
        int num_prod_rules = all_num_prod_rules[i];
        char non_term = non_terms[i];

        printf("Original Grammar\n");
        printf("\t%c -> ", non_term);
        for (size_t j = 0; j < num_prod_rules; ++j) {
            if (j != num_prod_rules - 1) {
                printf("%s | ", ith_prod_rules[j]);
            }
            else {
                printf("%s\n", ith_prod_rules[j]);
            }
        }
        printf("\n");

        char* left_recurs[256] = {0};
        size_t lr_idx = 0;

        char* nonleft_recurs[256] = {0};
        size_t nlr_idx = 0;

        for (size_t j = 0; j < num_prod_rules; ++j) {
            char* prod_rule = ith_prod_rules[j];
            if (prod_rule[0] == non_term) {
                left_recurs[lr_idx] = prod_rule;
                lr_idx += 1;
            }
            else {
                nonleft_recurs[nlr_idx] = prod_rule;
                nlr_idx += 1;
            }
        }

        if (lr_idx == 0) continue;
        char Adash[3] = {non_term, '\'', '\0'};
        printf("Transformed Grammar\n");

        // print: S -> B1S' | B2S'
        printf("\t%c -> ", non_term);
        for (size_t j = 0; j < nlr_idx; ++j) {
            if (j != nlr_idx - 1) {
                printf("%s%s | ", nonleft_recurs[j], Adash);
            }
            else {
                printf("%s%s\n", nonleft_recurs[j], Adash);
            }
        }

        // print: S' -> a1S' | a2S' | eps
        printf("\t%s -> ", Adash);
        for (size_t k = 0; k < lr_idx; ++k) {
            char* Sa = left_recurs[k];

            // skip first char (the S)
            char a[256];
            strcpy(a, Sa + 1);

            printf("%s%s", a, Adash);
            if (k != lr_idx - 1) printf(" | ");
        }

        printf(" | eps\n\n");
    }

    return 0;
}