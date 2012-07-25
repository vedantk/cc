#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include <cc/basic.h>

#define _cvpod(u_t, pod) \
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

	union value v = _cvpod(union value, y);
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
	// test2();

	float PAD = 1298374.123;
	float A = 3.14;
	cval Ac = cvflov(A);

	double B = 2.718;
	cval Bc = cvflov(B);

	printf("%f\n", Ac.flt);
	printf("%f\n", Ac.dbl);
	printf("%f\n", Bc.dbl);

	return 0;
}
