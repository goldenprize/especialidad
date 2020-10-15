#include <stdint.h>
#include <stdio.h>

#ifndef CHACHA_H
#define CHACHA_H

#define ROTL(a,b) (((a) << (b)) | ((a) >> (32 - (b))))
#define QR(a, b, c, d)(		\
	b ^= ROTL(a + d, 7),	\
	c ^= ROTL(b + a, 9),	\
	d ^= ROTL(c + b,13),	\
	a ^= ROTL(d + c,18))
#define ROUNDS 20

void chacha(uint32_t out[16], uint32_t const in[16]);

void chachacha_main();

#endif