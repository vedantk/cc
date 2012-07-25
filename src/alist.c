/*
 * alist.c
 */

#include <alist.h>
SOURCE_DECL

#define START_SIZE	1 << 3

alist* AL_new() {
	safe_create(alist, al, return NULL);
	al->cur = 0;
	al->max = START_SIZE;
	safe_calloc(al->arr, cval, al->max, return NULL);
	return al;
}

void AL_free(alist* li) {
	MEM_free(li->arr);
	MEM_free(li);
}

u32 AL_size(alist* li) {
	return li->cur;
}

void AL_resize(alist* li, u32 len) {
	li->max = len;
	void* arr = MEM_realloc(li->arr, sizeof(cval) * len);
	if (arr) {
		li->arr = arr;
	} else {
		AL_free(li);
		debugf(OOM);
	}
}

void AL_insert(alist* li, u32 idx, cval data) {
	if (li->cur + 1 >= li->max) {
		AL_resize(li, li->max * 2);
	}

	if (idx == li->cur) {
		li->arr[li->cur++] = data;
	} else if (idx < li->cur) {
		for (u32 i = li->cur; i >= idx; --i) {
			li->arr[i+1] = li->arr[i];
		}
		li->arr[idx] = data;
		++li->cur;
	} else return;
}

cval AL_get(alist* li, u32 idx) {
	return li->arr[idx];
}

cval AL_remove(alist* li, u32 idx) {
	if (li->cur < li->max / 4
		&& li->cur > START_SIZE) 
	{
		AL_resize(li, li->max / 4);
	}

	if (idx <= li->cur) {
		cval tmp = li->arr[idx];
		for (u32 i = idx; i < li->cur; ++i) {
			li->arr[i] = li->arr[i+1];
		}
		--li->cur;
		return tmp;
	} else return nil;
}

void AL_push_back(alist* li, cval data) {
	AL_insert(li, li->cur, data);
}

cval AL_pop_back(alist* li) {
	return li->arr[--li->cur];
}

void AL_clear(alist* li) {
	li->cur = 0;
	AL_resize(li, START_SIZE);
}

void AL_foreach(alist* li, foreach_fn fn, cval ctx) {
	for (u32 i = 0; i < li->cur; ++i) {
		fn(li->arr[i], ctx);
	}
}

void AL_print(alist* li) {
	printf("[");
	for (u32 i = 0; i < li->cur; ++i) {
		printf("%p", li->arr[i].ptr);
		if (i != li->cur - 1) {
			printf(", ");
		}
	}
	printf("]");
}

SOURCE_STOP
