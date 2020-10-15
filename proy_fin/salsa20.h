
#include <stdio.h>
#include <stdint.h> 

#ifndef SALSA20_H
#define SALSA20_H

#define R(x,n) (((x) << (n)) | ((x) >> (32-(n))))
#define LE(p) ( (p)[0] | ((p)[1]<<8) | ((p)[2]<<16) | ((p)[3]<<24) )

#define quarter(a,b,c,d) do {\
	b ^= R(d+a, 7);\
	c ^= R(a+b, 9);\
	d ^= R(b+c, 13);\
	a ^= R(c+d, 18);\
} while (0)

void salsa20_words(uint32_t *out, uint32_t in[16]);

void salsa20_block(uint8_t *out, uint8_t key[32], uint64_t nonce, uint64_t index);

void salsa20(uint8_t *message, uint64_t mlen, uint8_t key[32], uint64_t nonce);

uint8_t getValue();

void printMessage(uint8_t msg[]);

void salsa_main();



#endif