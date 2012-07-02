/*
 * pool.h
 *
 * A thread-pool based on pthreads.
 */

#ifndef POOL_H
#define POOL_H

#include <basic.h>
SOURCE_DECL

#include <pthread.h>

typedef struct pool pool;
typedef void (*POOL_worker)(cval obj, cval ctx);

pool*	POOL_new();
void	POOL_alloc(pool* p, int nr, POOL_worker f);
bool	POOL_submit_job(pool* p, cval data, cval ctx);
size_t	POOL_get_nr_workers(pool* p);
void	POOL_set_nr_workers(pool* p, int nr);
void	POOL_free(pool* p);

SOURCE_STOP

#endif /* POOL_H */
