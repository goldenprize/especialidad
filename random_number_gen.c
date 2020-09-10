/*=============================================================================
 |  Assignment: Random Number Generator
 |
 |  Author:     Gerardo Arriaga Ramirez

 |  Language:   C

 |  To Compile: The compiler used was the gcc compiler. 
                In order to compile it, first install the gcc compiler. 
                Open a cmd, in it type: 
                    gcc random_number_gen.c -o random_number_gen
                It should Generate an .exe file
 |
 |  Class:      Ingenieria de Software en Ambientes Embebidos

 |  Instructor: Luis Julian Dominguez
 
 |  Due Date:   10/9/20
 *===========================================================================*/

#include <stdio.h>
#include <stdlib.h> 
#include <time.h>

/*------------------------------------------------- int GetMaxNumber() -----
    |  Function:    int GetMaxNumber()
    |
    |  Purpose:     Gets the input from the user to get the max number to be randomly generated
    |
    |  Parameters:
    |      parameter_name: N/A
    |
    |  Returns:     An integer value that contains the value inserted by the user 
    *-------------------------------------------------------------------*/

int GetMaxNumber()
{
    int value = 0; 

    printf("\n\r");
    printf("Please Insert the Max random number that you want to generate:\n\r");
    scanf("%d", &value);
    printf("\n\r");

    return value;
}

/*------------------------------------------------- int GenerateRandom(int limit) -----
    |  Function: int GenerateRandom(int limit)
    |
    |  Purpose:  Generates a random number
    |
    |  Parameters:
    |      parameter_name: (IN) int limit 
    |
    |  Returns:  Random number that was generated
    *-------------------------------------------------------------------*/
int GenerateRandom(int limit)
{
    srand(time(0)); 

    return rand() % limit;
}

/*------------------------------------------------- PrintRandomNumber() -----
    |  Function: PrintRandomNumber()
    |
    |  Purpose:  Prints the number that was randomly generated
    |
    |  Parameters:
    |      parameter_name: N/A
    |
    |  Returns:  N/A
    *-------------------------------------------------------------------*/
void PrintRandomNumber()
{
    int maxValue = 0;

    maxValue = GetMaxNumber();

    printf("Random number is: %d", GenerateRandom(maxValue) );
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
void main()
{
    PrintRandomNumber();
}


