#include <cc/basic.h>

int main() {
	void* rs = RAND_new();
	char buf[512];
	RAND_get(&rs, buf, 512);
	bprint(buf, 512);
	RAND_free(&rs);
	return 0;
}
