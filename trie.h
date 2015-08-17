#ifndef __TRIE_H__
#define __TRIE_H__

struct trie_s {
    unsigned     size;
    const char **strings;
    void       **values;
    unsigned    *weight;
    unsigned    *cp;
};

struct trie_cursor_s {
    unsigned level;
    /* next string position */
    unsigned pos;
    unsigned idx;
};

void  trie_create(struct trie_s *ta, const char **strings, void **values, unsigned size);
void  trie_traverse_init(struct trie_s *ta, struct trie_cursor_s *c);
int   trie_traverse(struct trie_s *ta, struct trie_cursor_s *c, char ch);
void *trie_get_value(struct trie_s *ta, struct trie_cursor_s *c);
void  trie_destroy(struct trie_s *ta);

#endif
