#include <map>
#include <cc/profile.h>
#include <assert.h>
#include <cc/basic.h>

#include <stdint.h>
#include <cstdlib>

using namespace std;

int main() {
	map<int, int> ht;

	uint64_t usecs;
	int lim = 1000000;
	TIMEIT_USECS(usecs, {
		for (int i=0; i < lim; ++i) {
			ht[i] = i;
		}
	});
	printf("%d i32 inserts: %lu us\n", lim, usecs);

	TIMEIT_USECS(usecs, {
		for (int i=0; i < lim; ++i) {
			assert(ht[i] == i);
		}
	});
	printf("%d i32 gets+comparisons: %lu us\n", lim, usecs);

	TIMEIT_USECS(usecs, {
		for (int i=0; i < lim; ++i) {
			ht.erase(i);
		}
	});
	printf("%d i32 deletions: %lu us\n", lim, usecs);

	return 0;
}
