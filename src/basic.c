/*
 * basic.c
 */

#include <basic.h>
SOURCE_DECL

void* (*MEM_malloc)(size_t sz) = malloc;
void* (*MEM_calloc)(size_t nr, size_t sz) = calloc;
void* (*MEM_realloc)(void* ptr, size_t sz) = realloc;
void (*MEM_free)(void* ptr) = free;

#define DEF_CVAL_CTOR(_type_) \
	cval cval_ ## _type_ (_type_ v) { \
		cval data; \
		data.i64 = 0; \
		memcpy(&data, &v, sizeof(v)); \
		return data; \
	}

DEF_CVAL_CTOR(s8);
DEF_CVAL_CTOR(s16);
DEF_CVAL_CTOR(s32);
DEF_CVAL_CTOR(s64);
DEF_CVAL_CTOR(u8);
DEF_CVAL_CTOR(u16);
DEF_CVAL_CTOR(u32);
DEF_CVAL_CTOR(u64);
DEF_CVAL_CTOR(long);
DEF_CVAL_CTOR(ulong);
DEF_CVAL_CTOR(float);
DEF_CVAL_CTOR(double);
DEF_CVAL_CTOR(bool);
DEF_CVAL_CTOR(size_t);

#undef DEF_CVAL_CTOR

void debugf(const char* fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	fprintf(stderr, "\n");
	fflush(stderr);
}

void bprint(char* data, u32 len) {
	for (u32 i = 0; i < len; ++i) {
		printf("%d ", (int) data[i]);
	}
	printf("\n");
}

int CMP_str(const cval lhs, const cval rhs) {
	return strcmp(lhs.ptr, rhs.ptr);
}

int CMP_int(const cval lhs, const cval rhs) {
	if (lhs.i64 == rhs.i64) {
		return 0;
	} else {
		return lhs.i64 < rhs.i64 ? -1 : 1;
	}
}

int DTOR_free(cval data, cval ctx) {
	(void) ctx;
	if (data.ptr) {
		free(data.ptr);
	}
	return 0;
}

u32 HASH_8(const cval pod) {
	return HASH_fnv(&pod.ui8, 1);
}

u32 HASH_16(const cval pod) {
	return HASH_fnv(&pod.ui8, 2);
}

u32 HASH_32(const cval pod) {
	return HASH_fnv(&pod.ui8, 4);
}

u32 HASH_64(const cval pod) {
	return HASH_fnv(&pod.ui8, 8);
}

u32 HASH_fnv(const u8* val, u32 len) {
	const unsigned char* p = val;
	unsigned h = 2166136261;

	for (u32 i = 0; i < len; ++i) {
		h = (h * 16777619) ^ p[i];
	}

	return h;
}

u32 HASH_str(const cval val) {
	unsigned char* p = val.ptr;
	unsigned h = 2166136261;

	for (int i = 0; p[i] != '\0'; ++i) {
		h = (h * 16777619) ^ p[i];
	}

	return h;
}

void* RAND_new() {
	FILE* rs = fopen("/dev/urandom", "rb");
	if (rs == NULL) {
		srand((ulong) &rs);
	}
	return rs;
}

void RAND_get(void** state, char* buf, u32 len) {
	if (*state) {
		if (fread(buf, 1, len, *state) == 0) {
			*state = NULL;
			RAND_get(state, buf, len);
		}
	} else {
		for (u32 pos = 0; pos < len; pos += 4) {
			int r = rand();
			memcpy(buf + pos, &r, (len - pos) % 4);
		}
	}
}

void RAND_free(void** state) {
	if (*state) {
		fclose(*state);
	}
}

SOURCE_STOP
