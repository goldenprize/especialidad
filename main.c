#include "trivium.h"
#include "salsa20.h"

void main()
{
	uint8_t value = 0;

    uint8_t salsa = 0;

	while((value = getchar()) != '0')
    {
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
