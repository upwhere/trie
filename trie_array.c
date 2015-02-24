#include "trie.h"
#include <stdlib.h>
#include <string.h>

typedef struct trie_s {
    unsigned     size;
    const char **strings;
    unsigned    *weight;
    unsigned    *cp;
} trie_s;

typedef struct trie_cursor_s {
    unsigned level;
    /* next string position */
    unsigned pos;
    unsigned idx;
} trie_cursor_s;

void ta_create(trie_s *ta, const char **strings, unsigned size) {
    ta->size = size;
    ta->strings = strings;
    const char **_strings = malloc(sizeof(const char *) * size);
    unsigned    *weight = malloc(sizeof(unsigned) * size);
    unsigned    *cp     = malloc(sizeof(unsigned) * size * 2);

    /* use O(n^2) sorting */
    for (unsigned i = 0; i < size; ++ i) {
        _strings[i] = strings[i];
        weight[i] = 1;
    }

    for (unsigned i = 0; i < size - 1; ++ i) {
        unsigned min_idx = i;
        for (unsigned j = i + 1; j < size; ++ j) {
            if (strcmp(_strings[j],
                       _strings[min_idx]) < 0) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            const char *t = _strings[i];
            _strings[i] = _strings[min_idx];
            _strings[min_idx] = t;
        }
    }

    /* compute weight */
    for (unsigned i = 2; i < size; i <<= 1)
        for (unsigned j = 0; j < size; j += i)
            ++ weight[j];

    unsigned sum = 0;
    for (unsigned i = 0; i < size; ++ i) {
        unsigned o = sum;
        sum += weight[i];
        weight[i] = o;
    }

    /* compute common prefix for base level */
    for (unsigned i = 0; i < size - 1; ++ i) {
        unsigned _cp = 0;
        while (_strings[i][_cp] == _strings[i + 1][_cp])
            ++ _cp;
        cp[weight[i]] = _cp;
    }

    unsigned l = 0;
    for (unsigned i = 2; i < size; i <<= 1) {
        ++ l;
        for (unsigned j = 0; j < size - 1; j += i) {
            unsigned nx = (j + i < size) ? (j + i) : (size - 1);
            cp[weight[j] + l] = cp[weight[j] + l - 1];
            if (cp[weight[j] + l] > cp[weight[nx] + l - 1])
                cp[weight[j] + l] = cp[weight[nx] + l - 1];
        }
    }
    
    ta->strings = _strings;
    ta->weight  = weight;
    ta->cp      = cp;
}

void ta_traverse_init(trie_s *ta, trie_cursor_s *c) {
    c->level = ta->size > 1 ? ta->weight[1] - 1 : 0;
    c->pos = 0;
    c->idx = 0;
}

int ta_traverse(trie_s *ta, trie_cursor_s *c, char ch) {
    char _ch = ta->strings[c->idx][c->pos];
    if (_ch == ch) {
        return ta->strings[c->idx][++ c->pos];
    } else if (_ch < ch) {
        while (1) {
            /* need to move right-ward */
            /* failed if cannot move further */
            if (c->idx == ta->size - 1) return -1;
            if (ta->cp[ta->weight[c->idx] + c->level] < c->pos) {
                /* imply new_string is bigger */
                if (c->level == 0)
                    return -1;
                else -- c->level;
            } else {
                /* test next node in the level */
                unsigned nx = c->idx + (1 << c->level);
                if (nx >= ta->size) nx = ta->size - 1;
                char nx_ch = ta->strings[nx][c->pos];
                if (nx_ch == ch) {
                    c->idx = nx;
                    return ta->strings[c->idx][++ c->pos];
                } else if (nx_ch < ch) {
                    c->idx = nx;
                } else {
                    if (c->level == 0)
                        return -1;
                    else -- c->level;
                }
            }
        }
    } else {
        /* should not happen */
        return -1;
    }
}
