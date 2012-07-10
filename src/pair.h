/*
 * pair.h
 */

#ifndef PAIR_H
#define PAIR_H

#include <basic.h>
SOURCE_DECL

typedef struct {
	cval	first;
	cval	second;
} PAIR;

cval	PAIR_new(cval first, cval second);
void	PAIR_print(PAIR* p);

SOURCE_STOP

#endif /* PAIR_H */
