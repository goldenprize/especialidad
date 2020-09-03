/*=============================================================================
 |  Assignment: Greater Common Divisor
 |
 |  Author:     Gerardo Arriaga Ramirez

 |  Language:   C

 |  To Compile: The compiler used was the gcc compiler. 
                In order to compile it, first install the gcc compiler. 
                Open a cmd, in it type: 
                    gcc calculate_gcd.c -o calculate_gcd
                It should Generate an .exe file
 |
 |  Class:      Ingenieria de Software en Ambientes Embebidos

 |  Instructor: Luis Julian Dominguez
 
 |  Due Date:   3/9/20
 *===========================================================================*/

#include<stdio.h>

/*------------------------------------------------- int getValue() -----
    |  Function:    int getValue()
    |
    |  Purpose:     Gets the input from the user
    |
    |  Parameters:
    |      parameter_name: N/A
    |
    |  Returns:     An integer value that contains the value inserted by the user 
    *-------------------------------------------------------------------*/
int getValue()
{
    int value = 0; 

    printf("\n\r");
    printf("Please insert the value to calculate the GCD:\n\r");
    scanf("%d", &value);
    printf("\n\r");

    return value;
}

/*------------------------------------------------- void printResult(int value) -----
    |  Function: void printResult(int value)
    |
    |  Purpose:  Prints a given value
    |
    |  Parameters:
    |      parameter_name: (IN) int value 
    |
    |  Returns:  N/A 
    *-------------------------------------------------------------------*/
void printResult(int value)
{
    printf("\n\r");
    printf("The result of this GCD is:\n\r%d", value);
    printf("\n\r");
}

/*------------------------------------------------- void calculateGCD() -----
    |  Function: void calculateGCD()
    |
    |  Purpose:  Calculates the Greater Common Divisor by using the Euclidean Algorithm 
    |
    |  Parameters:
    |      parameter_name: N/A
    |
    |  Returns:  N/A 
    *-------------------------------------------------------------------*/
void calculateGCD()
{
    int a = 0, b = 0, q = 0, r = 0, result = 0; 

    a = getValue();         /* Get first input from user */

    b = getValue();         /* Get second input from user */

    if(a != 0 && b != 0)
    {
        do
        {
            r = a%b;            /* Get mod of a%b to obtain r */
            
            q = ((a - r) * 1/b);    
            r = a - (b * q);

            a = b;
            b = r;

            if(b != 0)
                result = b;     /* Store the actual result (r-1)*/
        }while(r > 0);

        printResult(result);    /* Print the result */
    }
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
    calculateGCD();         /* Call the function calculateGCD() in order to calculate the GCD */
}