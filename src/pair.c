/*
 * pair.c
 */

#include <pair.h>
SOURCE_DECL

cval PAIR_new(cval first, cval second) {
	safe_create(PAIR, p, return nil);
	p->first = first;
	p->second = second;
	return cvptr(p);
}

void PAIR_print(PAIR* p) {
	printf("(%lx, %lx)", p->first.i64, p->second.i64);
}

SOURCE_STOP
