/*
 * alist.h
 *
 * A dynamically-sized array.
 */

#ifndef ALIST_H
#define ALIST_H

#include <basic.h>
SOURCE_DECL

typedef struct {
	u32	cur;
	u32	max;
	cval*	arr;
} alist;

alist*	AL_new();
void	AL_free(alist* li);
u32	AL_size(alist* li);
void	AL_resize(alist* li, u32 len);
void	AL_insert(alist* li, u32 idx, cval data);
cval	AL_get(alist* li, u32 idx);
cval	AL_remove(alist* li, u32 idx);
void	AL_push_back(alist* li, cval data);
cval	AL_pop_back(alist* li);
void	AL_clear(alist* li);
void	AL_foreach(alist* li, foreach_fn fn, cval ctx);
void	AL_print(alist* li);

SOURCE_STOP

#endif /* ALIST_H */
