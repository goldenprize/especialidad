#include "trivium.h"
#include "salsa20.h"
#include "chacha.h"

void menu_1()
{
    printf("\nThis program is designed to encrypt files through:\n");
    printf("\tChacha algorithm\n");
    printf("\tSalsa20 algorithm\n");
    printf("\tTrivium algorithm\n");
}

void menu_2()
{
    printf("\nOptions:\n");
    printf("\tType 'c' to use the Chacha algorithm\n");
    printf("\tType 's' to use the Salsa20 algorithm\n");
    printf("\tType 't' to use the Trivium algorithm\n");
    printf("\nPress 0 to exit\n");
}

void main()
{
    uint8_t value = 0;

    menu_1();

	while((value = getchar()) != '0')
    {
        menu_2();

        switch(value)
        {
			case 't': 
				trivium_main();
				break;

			case 's':
				salsa_main();
				break;

            case 'c':
                chachacha_main();
                break;
		}
	}
}
