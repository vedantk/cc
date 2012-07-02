/*
 * htable.h 
 * 
 * An open-addressed hash table.
 */

#ifndef HTABLE_H
#define HTABLE_H

#include <basic.h>
SOURCE_DECL

typedef struct htable htable;

htable*	HT_new(key_cmp cmp, key_hash hash);
void	HT_free(htable* ht);
void	HT_put(htable* ht, cval key, cval val);
cval	HT_get(htable* ht, cval key); 
void	HT_remove(htable* ht, cval key);
void	HT_clear(htable* ht);
void	HT_foreach(htable* ht, foreach_fn fn, cval ctx);
void 	HT_print(htable* ht);

SOURCE_STOP

#endif /* HTABLE_H */
