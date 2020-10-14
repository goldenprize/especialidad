/*=============================================================================
 |  Assignment: Salsa20
 |
 |  Author:     Gerardo Arriaga Ramirez
 |  Language:   C
 |  To Compile: The compiler used was the gcc compiler. 
                In order to compile it, first install the gcc compiler. 
                Open a cmd, in it type: 
                    gcc salsa20.c -o salsa20
                It should Generate an .exe file
 |
 |  Class:      Ingenieria de Software en Ambientes Embebidos
 |  Instructor: Luis Julian Dominguez
 
 |  Due Date:   8/10/20
 *===========================================================================*/

#include "salsa20.h"

void salsa20_words(uint32_t *out, uint32_t in[16])
{
	uint32_t x[4][4];
	int i;
	for (i=0; i<16; ++i)
        x[i/4][i%4] = in[i];
	for (i=0; i<10; ++i) 
    { 
		// column round
		quarter(x[0][0], x[1][0], x[2][0], x[3][0]);
		quarter(x[1][1], x[2][1], x[3][1], x[0][1]);
		quarter(x[2][2], x[3][2], x[0][2], x[1][2]);
		quarter(x[3][3], x[0][3], x[1][3], x[2][3]);
        
		// row round
		quarter(x[0][0], x[0][1], x[0][2], x[0][3]);
		quarter(x[1][1], x[1][2], x[1][3], x[1][0]);
		quarter(x[2][2], x[2][3], x[2][0], x[2][1]);
		quarter(x[3][3], x[3][0], x[3][1], x[3][2]);
	}
	for (i=0; i<16; ++i)
        out[i] = x[i/4][i%4] + in[i];
}

// inputting a key, message nonce, keystream index and constants to that transormation
void salsa20_block(uint8_t *out, uint8_t key[32], uint64_t nonce, uint64_t index) 
{
    int i;
    uint32_t wordout[16];
	static const char c[16] = "expand 32-byte k"; // arbitrary constant

	uint32_t in[16] = 
    {
        LE(c),            LE(key),    LE(key+4),        LE(key+8),
	    LE(key+12),       LE(c+4),    nonce&0xffffffff, nonce>>32,
	    index&0xffffffff, index>>32,  LE(c+8),          LE(key+16),
	    LE(key+20),       LE(key+24), LE(key+28),       LE(c+12) 
    };

	salsa20_words(wordout, in);
	for (i=0; i<64; ++i)
        out[i] = 0xff & (wordout[i/4] >> (8 * (i % 4)));
}

// enc/dec: xor a message with transformations of key
void salsa20(uint8_t *message, uint64_t mlen, uint8_t key[32], uint64_t nonce) 
{
	int i;
	uint8_t block[64];
	for (i=0; i<mlen; i++) 
    {
		if (i%64 == 0) 
            salsa20_block(block, key, nonce, i/64);
		message[i] ^= block[i%64];
	}
}

uint8_t getValue()
{
    uint8_t value = 0; 

    printf("\n\r");
    printf("Please insert a value:\n\r");
    scanf("%d", &value);

    return value;
} 

void printMessage(uint8_t msg[])
{
    for(uint8_t i=0; i<64; ++i)
        printf("%X ", msg[i]); 
    printf("\n\n");
}

void salsa_main()
{
    uint8_t key[32] = {1};
	uint8_t msg[64] = {0};                      //final message 
    uint64_t nonce  = 536;

    for(uint8_t i=0; i< ((uint8_t)( sizeof(msg) / sizeof(msg[0]))); i++)
    {
        msg[i] = getValue();
    }

    printf("\n\rOriginal Message: \n\r");
    printMessage(msg);                          //Print original message
    
    salsa20(msg, sizeof(msg), key, nonce);      //Encrypt original message

    printf("\n\rEncrypted Message: \n\r");
    printMessage(msg);                          //Print encrypted message

    salsa20(msg, sizeof(msg), key, nonce);      //Decrypt message

    printf("\n\rDecrypted Message: \n\r");
    printMessage(msg);                          //Print decrypted message
}
