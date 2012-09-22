/*
 * profile.h
 */

#ifndef PROFILE_H
#define PROFILE_H

#include <sys/time.h>

#define TIMEIT_USECS(var, block) \
{ \
	struct timeval t0, tf; \
	gettimeofday(&t0, NULL); \
	{ block; } \
	gettimeofday(&tf, NULL); \
	var = (tf.tv_sec - t0.tv_sec) * 1000000 + (tf.tv_usec - t0.tv_usec); \
} \

#endif /* PROFILE_H */
