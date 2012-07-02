#include <assert.h>
#include <cc/alist.h>

int main() {
	alist* al = AL_new();
	assert(AL_size(al) == 0);

	AL_insert(al, 0, cval_s32(1));
	assert(AL_pop_back(al).i32 == 1);
	assert(AL_size(al) == 0);

	for (int i=0; i < 256; ++i) {
		AL_push_back(al, cval_s32(i + 1));
		AL_push_back(al, cval_s32(i + 2));
		assert(AL_pop_back(al).i32 == (i + 2));
	}

	for (int i=0; i < 256; ++i) {
		assert(AL_get(al, i).i32 == (i + 1));
	}

	int sz = AL_size(al);
	for (int i=0; i < sz; ++i) {
		AL_remove(al, 0);
		if (AL_size(al) > 0) {
			assert(AL_get(al, 0).i32 == (i + 2));
		}
	}

	for (int i=0; i < 10000; ++i) {
		AL_push_back(al, cval_s32(i));
	}

	for (int i=0; i < 9900; ++i) {
		AL_remove(al, 0);
	}

	for (int i=0; i < 100; ++i) {
		assert(AL_get(al, i).i32 == 9900 + i);
	}

	AL_print(al);
	AL_free(al);

	return 0;
}
