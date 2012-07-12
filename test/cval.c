#include <stdint.h>
#include <assert.h>

union value {
	uint32_t a;
	uint64_t b;
};

void test1() {
	uint32_t x = 0x12341234;
	uint32_t y = 0xffffffff;

	union value v = ((union value) y);
	assert(v.a == 0xffffffff); /* OK */
	assert(v.b == 0x00000000ffffffff); /* gcc OK, but clang fails */

	union value z = *((union value*) &y);
	assert(z.a == 0xffffffff); /* OK */
	assert(z.b == 0x12341234ffffffff); /* Expected result */

	assert(z.b == 0x00000000ffffffff); /* (both compilers expected to fail, and do) */
}

int main() {
	test1();
	return 0;
}
