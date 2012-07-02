#include <assert.h>
#include <cc/htable.h>

int main() {
	htable* ht = HT_new(CMP_str, HASH_str);
	
	char* s1 = "Hello ";
	cval a = cvptr(s1);
	char* s2 = "world!";
	cval b = cvptr(s2);
	
	HT_put(ht, a, b);
	HT_put(ht, b, a);
	
	assert(HT_get(ht, a).ptr == b.ptr);
	assert(HT_get(ht, b).ptr == a.ptr);

	HT_put(ht, a, b);
	HT_put(ht, b, a);

	assert(HT_get(ht, a).ptr == b.ptr);
	assert(HT_get(ht, b).ptr == a.ptr);
	
	HT_print(ht);
	HT_free(ht);

	ht = HT_new(CMP_int, HASH_32);

	for (int i=0; i < 100000; ++i) {
		HT_put(ht, cval_s32(i), cval_s32(i + 1));
		assert(HT_get(ht, cval_s32(i)).i32 == (i + 1));
	}

	HT_clear(ht);
	HT_print(ht);

	for (int i=0; i < 100000; ++i) {
		assert(HT_get(ht, cval_s32(i)).ptr == nil.ptr);
	}

	for (int i=0; i < 100000; ++i) {
		HT_put(ht, cval_s32(i * 10), cval_s32(i));
		assert(HT_get(ht, cval_s32(i * 10)).i32 == i);
	}

	for (int i=0; i < 50000; ++i) {
		HT_remove(ht, cval_s32(i * 10));
		assert(HT_get(ht, cval_s32(i * 10)).ptr == nil.ptr);
	}

	for (int i=50000; i < 100000; ++i) {
		assert(HT_get(ht, cval_s32(i * 10)).i32 == i);
	}
	
	HT_free(ht);

	return 0;
}
