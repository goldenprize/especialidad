#include "trivium.h"
#include "salsa20.h"

void main()
{
    uint8_t value = 0;

    printf("\nThis program is designed to encrypt files through:\n");
    printf("\tChacha algorithm\n");
    printf("\tSalsa20 algorithm\n");
    printf("\tTrivium algorithm\n");
    printf("\t=================\n");

	while((value = getchar()) != '0')
    {
        printf("\nOptions:\n");
        printf("\tType 'c' to use the Chacha algorithm\n");
        printf("\tType 's' to use the Salsa20 algorithm\n");
        printf("\tType 't' to use the Trivium algorithm\n");
        printf("\nPress 0 to exit\n");
        
        switch(value)
        {
			case 't': 
				trivium_main();
				break;

			case 's':
				salsa_main();
				break;
		}
	}
}
