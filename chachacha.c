/*=============================================================================
 |  Assignment: Implementation of the chacha algorithm
 |
 |  Author:     Gerardo Arriaga Ramirez
 |  Language:   C
 |  To Compile: The compiler used was the gcc compiler. 
                In order to compile it, first install the gcc compiler. 
                Open a cmd, in it type: 
                    gcc chachacha.c -o chacha
                It should Generate an .exe file
 |
 |  Class:      Ingenieria de Software en Ambientes Embebidos
 |  Instructor: Luis Julian Dominguez
 
 |  Due Date:   1/10/20
 *===========================================================================*/

#include "chacha.h"

/*------------------------------------------------- void chacha(uint32_t out[16], uint32_t const in[16]) -----
    |  Function void chacha(uint32_t out[16], uint32_t const in[16])
    |
    |  Purpose:  Performs the encryption of a data stream by using the chacha algorithm, and then prints the encrypted equivalent of the input 
	|				data stream 
    |
    |  Parameters:
    |      parameter_name: 	(IN) uint32_t out[16]
							(IN) uint32_t const in[16]
    |
    |  Returns:  N/A
    *-------------------------------------------------------------------*/ 
void chacha(uint32_t out[16], uint32_t const in[16])
{
	int i;
	uint32_t x[16];

	for (i = 0; i < 16; ++i)
		x[i] = in[i];
	
	// 20 roundss
	for (i = 0; i < ROUNDS; i += 2) 
	{
		QR(x[ 0], x[ 4], x[ 8], x[12]);	
		QR(x[ 5], x[ 9], x[13], x[ 1]);	
		QR(x[10], x[14], x[ 2], x[ 6]);	
		QR(x[15], x[ 3], x[ 7], x[11]);	

		QR(x[ 0], x[ 1], x[ 2], x[ 3]);	
		QR(x[ 5], x[ 6], x[ 7], x[ 4]);	
		QR(x[10], x[11], x[ 8], x[ 9]);	
		QR(x[15], x[12], x[13], x[14]);	
	}
	for (i = 0; i < 16; ++i)
    {
		out[i] = x[i] + in[i];

        printf("\n\rOut value: %x", out[i]);
    }
}

void test()
{
    printf("test");
}

/*------------------------------------------------- void main() -----
    |  Function: void main()
    |
    |  Purpose:  Main Function  
    |
    |  Parameters:
    |      parameter_name: N/A
    |
    |  Returns:  N/A 
    *-------------------------------------------------------------------*/
    
void chachacha_main()
{
    uint32_t out[16] = {0};
    
    uint32_t const in[16] = 
	{
        5, 2, 3, 4,
        5, 2, 3, 4,
        15, 2, 3, 4,
        12, 2, 3, 4
    };

    chacha(out, in);
}

