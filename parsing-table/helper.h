#include <stddef.h>
#include <stdbool.h>
#define EPS_CHAR '@'
#define EPS_STR "@"
#define MAX_CAPACITY 256
#define AC(new_item, items, items_len) \
    do { \
        items[(*(items_len))++] = (new_item); \
    } while(0)

char get_next_nonterm(); // next capital letter
char get_next_term(); // next small letter
void update_visited(char* chars, size_t num_chars);
bool is_non_term(char token);
bool is_term(char token);
int linear_search(char needle, char* haystack, size_t haystack_len);
void union_sets(char* dest, char* source, size_t* dest_len, size_t* source_len);
