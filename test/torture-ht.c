#include <cc/htable.h>
#include <cc/profile.h>

#include <assert.h>

int main() {
	htable* ht = HT_new(CMP_int, HASH_32);

	u64 usecs;
	int lim = 1000000;
	TIMEIT_USECS(usecs, {
		for (int i=0; i < lim; ++i) {
			HT_put(ht, cvpod(i), cvpod(i));
		}
	});
	printf("%d i32 inserts: %lu us\n", lim, usecs);

	TIMEIT_USECS(usecs, {
		for (int i=0; i < lim; ++i) {
			assert(HT_get(ht, cvpod(i)).i32 == i);
		}
	});
	printf("%d i32 gets+comparisons: %lu us\n", lim, usecs);

	TIMEIT_USECS(usecs, {
		for (int i=0; i < lim; ++i) {
			HT_remove(ht, cvpod(i));
		}
	});
	printf("%d i32 deletions: %lu us\n", lim, usecs);

	for (int i=0; i < lim; ++i) {
		assert(HT_get(ht, cvpod(i)).i32 == 0);
	}

	HT_free(ht);
	
	return 0;
}

