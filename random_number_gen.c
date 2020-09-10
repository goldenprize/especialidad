
#include <stdio.h>
#include <stdlib.h> 
#include <time.h>

int GenerateRandom()
{
    srand(time(0)); 

    return rand();
}

void PrintRandomNumber()
{
    printf("Random number is: %d", GenerateRandom() );
}

void main()
{
    PrintRandomNumber();
}


