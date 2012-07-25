/*
 * basic.h
 */

#ifndef BASIC_H
#define BASIC_H

#ifdef __cplusplus
# define SOURCE_DECL	extern "C" {
# define SOURCE_STOP	}
#else
# define SOURCE_DECL
# define SOURCE_STOP
#endif

SOURCE_DECL

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

extern void* (*MEM_malloc)(size_t sz);
extern void* (*MEM_calloc)(size_t nr, size_t sz);
extern void* (*MEM_realloc)(void* ptr, size_t sz);
extern void (*MEM_free)(void* ptr);

#define OOM	"Out of memory."

#define safe_new(var, type, stmt) { \
		if ((var = MEM_calloc(1, sizeof(type))) == NULL) { stmt; } \
	} \

#define safe_create(type, var, stmt) \
	type* var = MEM_calloc(1, sizeof(type)); \
	if (var == NULL) { stmt; } \

#define safe_calloc(var, type, nr, stmt) { \
		if ((var = MEM_calloc(nr, sizeof(type))) == NULL) { stmt; } \
	} \

#define safe_malloc(var, size, stmt) { \
		if ((var = MEM_malloc(size)) == NULL) { stmt; } \
	} \

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef unsigned long ulong;

typedef union {
	s8	i8;
	s16	i16;
	s32	i32;
	s64	i64;
	u8	ui8;
	u16	ui16;
	u32	ui32;
	u64	ui64;
	long	lng;
	ulong	ulng;
	float	flt;
	double	dbl;
	bool	bln;
	size_t	size;
	void*	ptr;
	s8*	buf;
	u8*	ubuf;
} cval;

#define cvpod(pod) \
	((sizeof(pod) == 1) ? ((cval) (0xffUL & (pod))) \
	: (sizeof(pod) == 2) ? ((cval) (0xffffUL & (pod))) \
	: (sizeof(pod) == 4) ? ((cval) (0xffffffffUL & (pod))) \
	: (cval) (pod))

#define cvflov(flov) \
	((sizeof(flov) == 4) ? \
	 ((cval) (0xffffffffUL & ((u64) *((u32*) &(flov))))) \
	: (cval) (flov))

#define cvptr(ptr)	((cval) ((void*) (ptr)))

#define nil		cvptr(0)

typedef int (*foreach_fn)(cval data, cval ctx);
typedef int (*key_cmp)(const cval lhs, const cval rhs);
typedef u32 (*key_hash)(const cval key);

void	debugf(const char* fmt, ...);
void	bprint(char* data, u32 len);

int	CMP_str(const cval lhs, const cval rhs);
int	CMP_int(const cval lhs, const cval rhs);

int	DTOR_free(cval data, cval ctx);

u32	HASH_8(const cval pod);
u32	HASH_16(const cval pod);
u32	HASH_32(const cval pod);
u32	HASH_64(const cval pod);
u32	HASH_fnv(const u8* val, u32 len);
u32	HASH_str(const cval str);

void*	RAND_new();
void	RAND_get(void** state, char* buf, u32 len);
void	RAND_free(void** state);

SOURCE_STOP

#endif /* BASIC_H */
