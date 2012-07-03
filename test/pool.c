#include <cc/pool.h>

static int ctr = 0;

static void callback(cval job, cval ctx) {
	++ctr;
	printf("ctr = %d\n", ctr);
	puts(ctx.ptr);
	(void) job;
}

int main() {
	pool* p = POOL_new();
	POOL_alloc(p, 4, callback);
	POOL_submit_job(p, nil, cvptr("Hello"));
	POOL_submit_job(p, nil, cvptr("Goodbye"));
	POOL_submit_job(p, nil, cvptr("Hallo"));
	POOL_submit_job(p, nil, cvptr("Gutbyy"));
	printf("workers = %d\n", POOL_get_nr_workers(p));
	while (ctr < 4) {};
	POOL_free(p);
	return 0;
}
