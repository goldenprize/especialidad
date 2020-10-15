#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <string.h>
#include <getopt.h>

#ifndef TRIVIUM_H
#define TRIVIUM_H

#define TRIVIUM_SIZE		10
#define MAX_FILE	        4096

struct trivium_context 
{
	int keylen;
	int ivlen;
	uint8_t key[10];
	uint8_t iv[10];
	uint32_t w[10];
};

#define U32TO32(x)								\
	((x << 24) | ((x << 8) & 0xFF0000) | ((x >> 8) & 0xFF00) | (x >> 24))

#define U8TO32_LITTLE(p) 	     	\
	(((uint32_t)((p)[0])) 	   | 	\
	((uint32_t)((p)[1]) << 8)  | 	\
	((uint32_t)((p)[2]) << 16) | 	\
	((uint32_t)((p)[3]) << 24))

// Macros bit allocation
#define S64(a, b, c)	((a << (96 - c))  | (b >> (c - 64)))
#define S96(a, b, c)	((a << (128 - c)) | (b >> (c - 96)))

// Macros update the keystream
#define T(w) { 							\
	t1 = S64(w[2], w[1], 66) ^ S64(w[2], w[1], 93); 	\
	t2 = S64(w[5], w[4], 69) ^ S64(w[5], w[4], 84); 	\
	t3 = S64(w[8], w[7], 66) ^ S96(w[9], w[8], 111);	\
}

#define UPDATE(w) {									\
	t1 ^= (S64(w[2], w[1], 91) & S64(w[2], w[1], 92)) ^ S64(w[5], w[4], 78);        \
	t2 ^= (S64(w[5], w[4], 82) & S64(w[5], w[4], 83)) ^ S64(w[8], w[7], 87);        \
	t3 ^= (S96(w[9], w[8], 109) & S96(w[9], w[8], 110)) ^ S64(w[2], w[1], 69);      \
											\
	w[2] = w[1];                                                                    \
	w[1] = w[0];                                                                    \
	w[0] = t3;                                                                      \
											\
	w[5] = w[4];                                                                    \
	w[4] = w[3];                                                                    \
	w[3] = t1;                                                                      \
											\
	w[9] = w[8];                                                                    \
	w[8] = w[7];                                                                    \
	w[7] = w[6];                                                                    \
	w[6] = t2;									\
}

// Macro update the array w
#define WORK_1(w) {		\
	uint32_t t1, t2, t3;	\
	T(w);			\
	UPDATE(w);		\
}

// Macro generate keystream (z)
#define WORK_2(w, z) {			\
	uint32_t t1, t2, t3;		\
	T(w);				\
	z = t1 ^ t2 ^ t3;		\
	UPDATE(w);			\
}

#define PRINT_U32TO32(x)\
	(printf("%02x %02x %02x %02x ", (x >> 24), ((x >> 16) & 0xFF), ((x >> 8) & 0xFF), (x & 0xFF)))

static void trivium_init(struct trivium_context *ctx);

static void trivium_keysetup(struct trivium_context *ctx);

int trivium_set_key_and_iv(struct trivium_context *ctx, const uint8_t *key, const int keylen, const uint8_t iv[10], const int ivlen);

void trivium_crypt(struct trivium_context *ctx, const uint8_t *buf, uint32_t buflen, uint8_t *out);

void trivium_test_vectors(struct trivium_context *ctx);

void *xmalloc(size_t size);

FILE *open_file(char *s, int i);

void trivium_main();

#endif