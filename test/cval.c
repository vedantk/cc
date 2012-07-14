#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#define cvpod(u_t, pod) \
	(u_t) (sizeof(pod) == 1 ? (0xffUL & pod) : \
		sizeof(pod) == 2 ? (0xffffUL & pod) : \
			sizeof(pod) == 4 ? (0xffffffffUL & pod) : pod)

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

void test2() {
	uint32_t x = 0x12341234;
	uint32_t y = 0xffffffff;

	union value v = cvpod(union value, y);
	assert(v.a == 0xffffffff); /* OK */
	
	printf("v.b = %lx\n", v.b);
	assert(v.b == 0x00000000ffffffff); /* gcc OK, but clang fails */

	union value z = *((union value*) &y);
	assert(z.a == 0xffffffff); /* OK */
	assert(z.b == 0x12341234ffffffff); /* Expected result */

	assert(z.b == 0x00000000ffffffff); /* (both compilers expected to fail, and do) */
}

int main() {
	// test1();
	test2();
	return 0;
}
