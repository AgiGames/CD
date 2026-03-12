#include <stdbool.h>
#include <stddef.h>

#include "helper.h"

bool visited[128] = {0};
char nonterm_ptr = 65;
char term_ptr = 97;

void set_visited(char chr) {
    visited[chr] = true;
}

void update_visited(char* chars, size_t num_chars) {
    for (size_t i = 0; i < num_chars; i++) {
        visited[chars[i]] = true;
    }
}

char get_next_nonterm() {
    while (visited[nonterm_ptr]) {
        nonterm_ptr++;
    }
    
    visited[nonterm_ptr] = true;
    return (char) nonterm_ptr;
}

char get_next_term() {
    while (visited[term_ptr]) {
        term_ptr++;
    }
    
    visited[term_ptr] = true;
    return (char) term_ptr;
}

bool is_non_term(char token) {
    return ('A' <= token && token <= 'Z');
}

bool is_term(char token) {
    return (token == EPS_CHAR) || !is_non_term(token);
}

int linear_search(char needle, char *haystack, size_t haystack_len) {
    int index = -1;
    for (size_t i = 0; i < haystack_len; ++i) {
        if (haystack[i] == needle) {
            index = i;
            break;
        }
    }

    return index;
}

void union_sets(char *dest, char *source, size_t* dest_len, size_t* source_len) {
    for (size_t i = 0; i < *source_len; ++i) {
        if ((linear_search(source[i], dest, *dest_len) == -1) && (source[i] != EPS_CHAR)) {
            AC(source[i], dest, dest_len);
        }
    }
}
