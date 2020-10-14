
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <string.h>
#include <getopt.h>

#define TRIVIUM		10
#define MAX_FILE	4096

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

// Trivium initialization function
static void trivium_init(struct trivium_context *ctx)
{
	memset(ctx, 0, sizeof(*ctx));
}

// Function key and iv setup
static void trivium_keysetup(struct trivium_context *ctx)
{
	uint32_t w[10];
	uint8_t s[40];
	int i;

	memset(s, 0, sizeof(s));

	for(i = 0; i < ctx->keylen; i++)
		s[i] = ctx->key[i];
	
	for(i = 0; i < ctx->ivlen; i++)
		s[i + 12] = ctx->iv[i];

	s[37] = 0x70;
	
	w[0] = U8TO32_LITTLE(s + 0);
	w[1] = U8TO32_LITTLE(s + 4);
	w[2] = U8TO32_LITTLE(s + 8);
	w[3] = U8TO32_LITTLE(s + 12);
	w[4] = U8TO32_LITTLE(s + 16);
	w[5] = U8TO32_LITTLE(s + 20);
	w[6] = U8TO32_LITTLE(s + 24);
	w[7] = U8TO32_LITTLE(s + 28);
	w[8] = U8TO32_LITTLE(s + 32);
	w[9] = U8TO32_LITTLE(s + 36);

	for(i = 0; i < 4 * 9; i++)
		WORK_1(w);
	
	memcpy(ctx->w, w, sizeof(w));
}

// Fill the trivium context (key and iv)
// Return value: 0 (if all is well), -1 is all bad
int trivium_set_key_and_iv(struct trivium_context *ctx, const uint8_t *key, const int keylen, const uint8_t iv[10], const int ivlen)
{
	trivium_init(ctx);
	
	if((keylen > 0) && (keylen <= TRIVIUM))
		ctx->keylen = keylen;
	else
		return -1;
	
	if((ivlen > 0) && (ivlen <= 10))
		ctx->ivlen = ivlen;
	else
		return -1;
	
	memcpy(ctx->key, key, keylen);
	memcpy(ctx->iv, iv, 10);
	
	trivium_keysetup(ctx);
	
	return 0;
}

/*
 * Trivium crypt algorithm.
 * ctx - pointer on trivium context
 * buf - pointer on buffer data
 * buflen - length the data buffer
 * out - pointer on output array
*/
void trivium_crypt(struct trivium_context *ctx, const uint8_t *buf, uint32_t buflen, uint8_t *out)
{
	uint32_t z, w[10], i;

	memcpy(w, ctx->w, sizeof(w));

	for(; buflen >= 4; buflen -= 4, buf += 4, out += 4) 
    {
		WORK_2(w, z);
		
		*(uint32_t *)(out + 0) = *(uint32_t *)(buf + 0) ^ U32TO32(z);
	}

	if(buflen) 
    {
		WORK_2(w, z);
		
		for(i = 0; i < buflen; i++, z >>= 8)
			out[i] = buf[i] ^ (uint8_t)(z);
	}
	
	memcpy(ctx->w, w, sizeof(w));
}

#define PRINT_U32TO32(x)\
	(printf("%02x %02x %02x %02x ", (x >> 24), ((x >> 16) & 0xFF), ((x >> 8) & 0xFF), (x & 0xFF)))

// Test vectors print
void trivium_test_vectors(struct trivium_context *ctx)
{
	uint32_t z, w[10], i;
	
	memcpy(w, ctx->w, sizeof(w));

	printf("\nTest vectors for the Trivium:\n");

	printf("\nKey:       ");

	for(i = 0; i < 10; i++)
		printf("%02x ", ctx->key[i]);
	
	printf("\nIV:        ");

	for(i = 0; i < 10; i++)
		printf("%02x ", ctx->iv[i]);

	printf("\nKeystream: ");

	for(i = 0; i < 10; i++) 
    {
		WORK_2(w, z);
		PRINT_U32TO32(U32TO32(z));
	}
	
	printf("\n\n");
}

// Allocates memory
void *xmalloc(size_t size)
{
	void *p = malloc(size);

	if(p == NULL) 
    {
		printf("Allocates memory error!\n");
		exit(1);
	}
	else
		return p;
}

// Open the file
FILE *open_file(char *s, int i)
{
	FILE *fp;
	
	if(i == 1)
		fp = fopen(s, "rb+");
	else
		fp = fopen(s, "w+");

	if(fp == NULL) 
    {
		printf("Error open file!\n");
		exit(1);
	}

	return fp;
}

// Help function
void help(void)
{
	printf("\nThis program is designed to encrypt/decrypt files.\n");
	printf("\nOptions:\n");
	printf("\t--help(-h) - reference manual\n");
	printf("\t--type(-t) - type running of program: 1 - encrypt, 2 - decrypt\n");
	printf("\t--block(-b) - block size data read from the file. By default = 10000\n");
	printf("\t--input(-i) - input file\n");
	printf("\t--output(-o) - output file\n");
	printf("Example: ./bigtest -t 1 -b 1000 -i 1.txt -o crypt or ./bigtest -t 2 -b 1000 -i crypt -o decrypt\n\n");
}

int main(int argc, char *argv[])
{
	FILE *fp, *fd;
	struct trivium_context ctx;
	uint32_t byte, block = 10000;
	uint8_t *buf, *out, key[10], iv[10];
	char file1[MAX_FILE], file2[MAX_FILE];
	int res, action = 1;

	const struct option long_option [] = 
    {
		{"input",  1, NULL, 'i'},
		{"output", 1, NULL, 'o'},
		{"block",  1, NULL, 'b'},
		{"type",   1, NULL, 't'},
        {"algo",   1, NULL, 'a'},
		{"help",   0, NULL, 'h'},
		{0, 	   0, NULL,  0 }
	};
	
	if(argc < 2) 
    {
		help();
		return 0;
	}

	while((res = getopt_long(argc, argv, "i:o:b:t:a:h", long_option, 0)) != -1) 
    {
		switch(res)
        {
            case 'b' : 
                block = atoi(optarg);
                break;
            case 'i' :
                strcpy(file1, optarg);
                break;
            case 'o' : 
                strcpy(file2, optarg);
                break;
            case 't' : 
                action = atoi(optarg);
                break;
            case 'a' : 
                printf("hola");
                break;
            case 'h' : 
                help();
                return 0;
		}
	}
	
	buf = xmalloc(sizeof(uint8_t) * block);
	out = xmalloc(sizeof(uint8_t) * block);
	
	fp = open_file(file1, 1);
	fd = open_file(file2, 2);
	
	memset(key, 'k', sizeof(key));
	memset(iv, 'i', sizeof(iv));

	if(trivium_set_key_and_iv(&ctx, (uint8_t *)key, 10, iv, 2)) 
    {
		printf("Trivium context filling error!\n");
		exit(1);
	}
	
	while((byte = fread(buf, 1, block, fp)) > 0) 
    {
		if(action == 1)
			trivium_crypt(&ctx, buf, byte, out);
		else
			trivium_crypt(&ctx, buf, byte, out);
		
		fwrite(out, 1, byte, fd);
	}
	
	free(buf);
	free(out);

	return 0;
}

/*
void main()
{
    uint8_t value = 0;

    FILE *fp, *fd;
	struct trivium_context ctx;
	uint32_t byte, block = 10000;
	uint8_t *buf, *out, key[10], iv[10];
	char file1[MAX_FILE], file2[MAX_FILE];
	int res, action = 1;

    while((value = getchar()) != '0')
    {
        switch(value)
        {
            case 'b' : 
                block = atoi(optarg);
                break;
            case 'i' :
                strcpy(file1, optarg);
                break;
            case 'o' : 
                strcpy(file2, optarg);
                break;
            case 't' : 
                action = atoi(optarg);
                break;
            case 'a' : 
                printf("hola");
                break;
            //case 'h' : 
            //    help();
            //    break;
		}







    }        

}

*/