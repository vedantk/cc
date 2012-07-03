/*
 * htable.c
 */

#include <htable.h>
SOURCE_DECL

typedef struct {
	cval	key;
	cval	val;
} PAIR;

struct htable {
	u32	size;
	u32	deleted;
	u32	max_size;

	/* A rehash is triggered when the current size exceeds the 'put'
	 * threshold, and when the number of deleted elements exceeds the
	 * 'delete' threshold. */
	u32	rehash_put_thresh;
	u32	rehash_del_thresh;

	/* The PAIR array doesn't support null keys, so nullval was added. */
	PAIR*	table;
	cval	nullval;

	key_cmp	cmp_fn;
	key_hash hash_fn;
};

#define MAX_LOAD	0.80
#define MAXDEL_LOAD	(0.40 * MAX_LOAD)
#define START_SIZE	(1 << 3)
#define MAGIC		((void*) 0xCAFEBABE)

/* A method for traversing potential indices in the table. */
#define STEP(ht, _hash, _step) \
	hash = (_hash + _step) % ht->max_size; \

#define ASSIGN(pcell, _key, _val) \
	pcell.key = _key; pcell.val = _val; \

#define IS_FULL(ht, _idx) \
	(ht->table[_idx].key.ptr) \

/* During collision resolution, it's possible to step across keys that were
 * removed from the table. Magic values are needed to ensure that table
 * accesses don't terminate early. */
#define IS_OCCUPIED(ht, _idx) \
	(IS_FULL(ht, _idx) || ht->table[_idx].val.ptr == MAGIC) \

#define IS_UNIQUE(ht, _idx, _key) \
	(IS_FULL(ht, _idx) && (ht->cmp_fn(_key, ht->table[_idx].key) == 0)) \

#define SET_THRESH(ht) \
	ht->rehash_put_thresh = (u32) (MAX_LOAD * ht->max_size); \
	ht->rehash_del_thresh = (u32) (MAXDEL_LOAD * ht->max_size); \

htable* HT_new(key_cmp cmp, key_hash hash) {
	htable* ht;
	safe_new(ht, htable, return NULL);
	ht->max_size = START_SIZE;
	ht->cmp_fn = cmp;
	ht->hash_fn = hash;
	SET_THRESH(ht);
	safe_calloc(ht->table, PAIR, START_SIZE, return NULL);
	return ht;
}

void HT_free(htable* ht) {
	MEM_free(ht->table);
	MEM_free(ht);
}

static inline void hash_idx(htable* ht, cval key, u32* hash, u32* step) {
	*hash = ht->hash_fn(key);
	*step = *hash % (ht->max_size / (START_SIZE / 2));
	if (*step % 2 == 0) {
		*step += 1;
	}
	*hash %= ht->max_size;
}

static bool oa_ht_put(htable* ht, cval key, cval val) {
	u32 hash, step;
	hash_idx(ht, key, &hash, &step);

	while (IS_FULL(ht, hash)) {
		if (ht->cmp_fn(ht->table[hash].key, key) == 0) {
			return false;
		}

		STEP(ht, hash, step);
	}

	ASSIGN(ht->table[hash], key, val);
	return true;
}

static void oa_ht_rehash(htable* ht, u32 newsz) {
	PAIR* temp = NULL;
	safe_calloc(temp, PAIR, newsz, return);
	PAIR* old = ht->table;
	ht->table = temp;
	u32 cur_max = ht->max_size;
	ht->max_size = newsz;
	ht->deleted = 0;
	SET_THRESH(ht);

	for (u32 it = 0; it < cur_max; ++it) {
		if (old[it].key.ptr != NULL) {
			oa_ht_put(ht, old[it].key, old[it].val);
		}
	}

	MEM_free(old);
}

void HT_put(htable* ht, cval key, cval val) {
	if (key.ptr == nil.ptr) {
		ht->nullval = val;
		return;
	}

	if (oa_ht_put(ht, key, val)) {
		++ht->size;
	}

	if (ht->size > ht->rehash_put_thresh) {
		oa_ht_rehash(ht, ht->max_size * 2);
	}
}

cval HT_get(htable* ht, cval key) {
	if (key.ptr == nil.ptr) {
		return ht->nullval;
	}

	u32 hash, step;
	hash_idx(ht, key, &hash, &step);

	while (IS_OCCUPIED(ht, hash)) {
		if (IS_UNIQUE(ht, hash, key)) {
			return ht->table[hash].val;
		}

		STEP(ht, hash, step);
	}

	return nil;
}

void HT_remove(htable* ht, cval key) {
	if (key.ptr == nil.ptr) {
		ht->nullval = nil;
		return;
	}

	u32 hash, step;
	hash_idx(ht, key, &hash, &step);

	while (IS_OCCUPIED(ht, hash)) {
		if (IS_UNIQUE(ht, hash, key)
		    && ht->table[hash].val.ptr != MAGIC)
		{
			ASSIGN(ht->table[hash], nil, cvptr(MAGIC));
			--ht->size;
			++ht->deleted;
			if (ht->deleted > ht->rehash_del_thresh) {
				oa_ht_rehash(ht, ht->max_size / 2);
			}
			return;
		}

		STEP(ht, hash, step);
	}
}

void HT_clear(htable* ht) {
	ht->size = 0;
	ht->deleted = 0;
	ht->nullval = nil;
	ht->max_size = START_SIZE;
	SET_THRESH(ht);
	free(ht->table);
	safe_calloc(ht->table, PAIR, START_SIZE, return);
}

void HT_foreach(htable* ht, foreach_fn fn, cval ctx) {
	for (u32 i = 0; i < ht->max_size; ++i) {
		fn(cvpod(ht->table[i].val), ctx);
	}
	fn(ht->nullval, ctx);
}

void HT_print(htable* ht) {
	puts("{");
	for (ulong i = 0; i < ht->max_size; ++i) {
		printf("\t%p : %p,\n",
			ht->table[i].key.ptr, ht->table[i].val.ptr);
	}
	printf("\t<null> : %p\n", ht->nullval.ptr);
	puts("}");
}

SOURCE_STOP
