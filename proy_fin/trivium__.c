#include "trivium.h"

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
	
	if((keylen > 0) && (keylen <= TRIVIUM_SIZE))
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
		return 0;
	}

	return fp;
}

void trivium_main()
{
	FILE *fp, *fd;
	struct trivium_context ctx;
	uint32_t byte, block = 10000;
	uint8_t *buf, *out, key[10], iv[10];

	int res, action = 1;

	char file_ini[] = "C:\\Users\\g\\Documents\\repos\\especialidad\\Cripto\\1.txt";
	char file_fin[] = "C:\\Users\\g\\Documents\\repos\\especialidad\\Cripto\\crypto";

	uint8_t value = 0;

	buf = xmalloc(sizeof(uint8_t) * block);
	out = xmalloc(sizeof(uint8_t) * block);

	fp = open_file(file_ini, 1);
	fd = open_file(file_fin, 2);
	
	memset(key, 'k', sizeof(key));
	memset(iv, 'i', sizeof(iv));

	if(trivium_set_key_and_iv(&ctx, (uint8_t *)key, 10, iv, 2)) 
	{
		printf("Trivium context filling error!\n");
		return;
	}
	
	while((byte = fread(buf, 1, block, fp)) > 0) 
	{
		if(action == 1)
			trivium_crypt(&ctx, buf, byte, out);
		
		fwrite(out, 1, byte, fd);
	}
	
	free(buf);
	free(out); 		
}
