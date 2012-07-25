/*
 * pool.c
 */

#include <pool.h>
SOURCE_DECL

#include <semaphore.h>
#include <pthread.h>

#include <pair.h>
#include <queue.h>
#include <alist.h>

typedef struct {
	queue*	jobs;
	sem_t	jobs_sem;
	pthread_mutex_t jobs_lock;
	POOL_worker callback;
	pthread_t thread;
} engine;

struct pool {
	int	last;
	alist*	workers; /* [engine*] */
};

#define WITH(lck, body) { \
	if (pthread_mutex_lock(&lck) == 0) { \
		{ body; } \
		pthread_mutex_unlock(&lck); \
	} \
}

static void* dispatcher(void* arg) {
	engine* e = arg;
	while (true) {
		if (sem_wait(&e->jobs_sem) != 0) {
			continue;
		}

		PAIR* input;
		WITH(e->jobs_lock, {
			input = Q_front(e->jobs).ptr;
			Q_pop_front(e->jobs);
		});

		if (!input) {
			break;
		}

		e->callback(input->first, input->second);
		MEM_free(input);
	}

	Q_foreach(e->jobs, DTOR_free, nil);
	pthread_mutex_destroy(&e->jobs_lock);
	sem_destroy(&e->jobs_sem);
	MEM_free(e);
	pthread_exit(NULL);
	return NULL;
}

pool* POOL_new() {
	safe_create(pool, p, return NULL);
	if (!(p->workers = AL_new())) {
		MEM_free(p);
		return NULL;
	}
	return p;
}

void POOL_alloc(pool* p, int nr, POOL_worker f) {
	for (int i=0; i < nr; ++i) {
		safe_create(engine, e, return);
		e->callback = f;
		if (!(e->jobs = Q_new())) {
			goto del0;
		}
		if (pthread_mutex_init(&e->jobs_lock, NULL) != 0) {
			goto del1;
		}
		if (sem_init(&e->jobs_sem, 0, 0) != 0) {
			goto del2;
		}

		int err = pthread_create(&e->thread, NULL, dispatcher, e);
		if (err != 0) {
			goto del3;
		}

		AL_push_back(p->workers, cvptr(e));
		continue;

del3:
		sem_destroy(&e->jobs_sem);
del2:
		pthread_mutex_destroy(&e->jobs_lock);
del1:
		Q_free(e->jobs);
del0:
		MEM_free(e);
		return;
	}
}

bool POOL_submit_job(pool* p, cval data, cval ctx) {
	int cur = (p->last + 1) % POOL_get_nr_workers(p);
	p->last = cur;
	engine* worker = AL_get(p->workers, cur).ptr;
	WITH(worker->jobs_lock,
		Q_push_back(worker->jobs, PAIR_new(data, ctx));
	);
	return sem_post(&worker->jobs_sem) == 0;
}

int POOL_get_nr_workers(pool* p) {
	return AL_size(p->workers);
}

static int shutdown_engine(cval worker, cval ctx) {
	(void) ctx;
	engine* e = worker.ptr;
	Q_push_back(e->jobs, nil);
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
