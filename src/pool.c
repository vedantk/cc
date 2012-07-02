/*
 * pool.c
 */

#include <pool.h>
SOURCE_DECL

#include <alist.h>
#include <unistd.h>
#include <pthread.h>

enum job_flags {
	NORMAL = 1 << 0,
	SHUTDOWN = 1 << 1,
};

typedef struct {
	u8	flags;
	cval	data;
	cval	context;
} job;

typedef struct {
	/* Data is written into fd[1] and read from fd[0]. */
	int 	fd[2];	

	pthread_t thread;
	POOL_worker callback;
} engine;

struct pool {
	int	last;
	alist*	workers; /* [engine*] */
};

static void* dispatcher(void* arg) {
	engine* e = arg;
	while (true) {
		job input;
		if (read(e->fd[0], &input, sizeof(job)) != sizeof(job)) {
			continue;
		}

		if (input.flags & NORMAL) {
			e->callback(input.data, input.context);
		}

		if (input.flags & SHUTDOWN) {
			break;
		}
	}
	close(e->fd[0]);
	close(e->fd[1]);
	MEM_free(e);
	pthread_exit(NULL);
	return NULL;
}

pool* POOL_new() {
	pool* p;
	safe_new(p, pool, return NULL);
	if (!(p->workers = AL_new())) {
		MEM_free(p);
		return NULL;
	}
	return p;
}

void POOL_alloc(pool* p, int nr, POOL_worker f) {
	for (int i=0; i < nr; ++i) {
		engine* e;
		safe_new(e, engine, return);
		if (pipe(e->fd) != 0) {
			MEM_free(e);
			return;
		}
		e->callback = f;
		int err = pthread_create(&e->thread, NULL, dispatcher, e);
		if (err != 0) {
			MEM_free(e);
			return;
		}
		AL_push_back(p->workers, cvptr(e));
	}
}

bool POOL_submit_job(pool* p, cval data, cval ctx) {
	int cur = (p->last + 1) % POOL_get_nr_workers(p);
	p->last = cur;
	engine* worker = AL_get(p->workers, cur).ptr;
	job input = {
		.flags = NORMAL,
		.data = data,
		.context = ctx,
	};
	if (write(worker->fd[1], &input, sizeof(job)) == sizeof(job)) {
		return true;
	}
	return false;
}

int POOL_get_nr_workers(pool* p) {
	return AL_size(p->workers);
}

static int shutdown_engine(cval worker, cval ctx) {
	job kill = {
		.flags = SHUTDOWN,
		.data = nil,
		.context = ctx,
	};
	engine* e = worker.ptr;
	write(e->fd[1], &kill, sizeof(job));
	return 0;
}

void POOL_set_nr_workers(pool* p, int nr) {
	int cur = POOL_get_nr_workers(p);
	engine* zero = AL_get(p->workers, 0).ptr;
	POOL_worker f = zero->callback;
	if (nr > cur) {
		POOL_alloc(p, nr - cur, f);
	} else if (nr < cur) {
		for (int i=0; i < cur && (i < (cur - nr)); ++i) {
			engine* e = AL_pop_back(p->workers).ptr;
			shutdown_engine(cvptr(e), nil);
		}
	}
}

void POOL_free(pool* p) {
	AL_foreach(p->workers, shutdown_engine, nil);
	AL_free(p->workers);
	MEM_free(p);
}

SOURCE_STOP
